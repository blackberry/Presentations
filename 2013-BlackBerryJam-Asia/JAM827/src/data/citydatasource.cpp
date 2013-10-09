/* Copyright (c) 2013 BlackBerry Limited.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#include "citydatasource.h"
#include "globalconnection.h"
#include "appsettings.h"

#include <bb/data/DataAccessReply>
#include <bb/data/JsonDataAccess>
#include <bb/data/SqlConnection>

using namespace bb::cascades;

CityDataSource::CityDataSource(QObject *parent) :
        QObject(parent), mReply(0), mRevision(0)
{
    // Connect to the sslErrors signal to the onSslErrors() function. This will help us see what errors
    // we get when connecting to the address given by server url.
    connect(&mAccessManager, SIGNAL(sslErrors ( QNetworkReply * , const QList<QSslError> & )), this,
            SLOT(onSslErrors ( QNetworkReply * , const QList<QSslError> & )));

    // Get the global sql connection.
    mSqlConnector = GlobalConnection::instance()->sqlConnection();

    connect(mSqlConnector, SIGNAL(reply(const bb::data::DataAccessReply&)), this,
            SLOT(onSqlConnectorReply(const bb::data::DataAccessReply&)));
}

CityDataSource::~CityDataSource()
{
}

int CityDataSource::incrementRevision()
{
    QVariantList dataList;
    dataList << "cities";
    mSqlConnector->executeAndWait("UPDATE revision SET revision_id = revision_id + 1 WHERE table_name=:cities",
            dataList, 1);

    DataAccessReply reply = mSqlConnector->executeAndWait("SELECT revision_id FROM revision WHERE table_name=:cities", dataList, 2);

    QVariantList resultList = reply.result().toList();
    QVariantMap resultMap = resultList.at(0).toMap();
    mRevision = resultMap["revision_id"].toInt();

    return mRevision;
}

void CityDataSource::loadNetworkReplyDataIntoDataBase(QVariantList data)
{
    // Iterate over all the items in the received data.
    QVariantList::Iterator item = data.begin();

    // ValuesTable is a list of lists for adding a batch of data to the database.
    QVariantList valuesTable;

    // Update the data revision since we are adding new data to the data base.
    int revision = incrementRevision();

    while (item != data.end()) {
        QVariantList entryValues;
        QVariantMap itemMap = (*item).toMap();

        // Store the retrieved values for adding to data base.
        entryValues << itemMap["name"];
        entryValues << itemMap["region"];
        entryValues << itemMap["name"];
        entryValues << itemMap["region"];
        entryValues << itemMap["name"];
        entryValues << itemMap["region"];
        entryValues << revision;
        valuesTable << QVariant::fromValue(entryValues);
        ++item;
    }

    // This here is a bit tricky, if the city is already in the data base we do not want to fully replace it
    // since it might have favorite set to true so we need to insert a new one only if the city does not exist.
    QString query ("INSERT OR REPLACE INTO cities (citiesid, city, region, favorite, revision_id) VALUES ((SELECT citiesid FROM cities WHERE city=:name AND region=:region LIMIT 1), :name, :region, (SELECT favorite FROM cities WHERE city=:name AND region=:region LIMIT 1), :revision_id)");

    mSqlConnector->executeBatch(query, valuesTable);
}

void CityDataSource::onHttpFinished()
{
    JsonDataAccess jda;
    QVariantList citiesData;
    int httpStatus = -1; // controls the final behavior of this function

    if (mReply->error() == QNetworkReply::NoError) {
        citiesData = jda.load(mReply).value<QVariantList>();

        if (jda.hasError()) {
            bb::data::DataAccessError error = jda.error();
            qDebug() << "JSON loading error:" << error.errorType() << " : " << error.errorMessage();
            httpStatus = -2;
        } else {
            httpStatus = 200;
        }
    } else {
        // If there was an error loading the data via the network, we will log it and
        // display an error message.
        QVariant statusCode = mReply->attribute(QNetworkRequest::HttpStatusCodeAttribute);

        if (statusCode.isValid()) {
            int status = statusCode.toInt();
            QString reason =mReply->attribute(QNetworkRequest::HttpReasonPhraseAttribute).toString();

            qDebug() << "Network request to " << mReply->request().url().toString() <<" failed with http status "<< status << " " << reason;
        } else {
            qDebug() << "Network request to " << mReply->request().url().toString() <<
                            " failed, make sure you are connected to the same network as the server you are trying to reach";
        }
    }

    // Now behave
    switch (httpStatus) {
        case 200:
            loadNetworkReplyDataIntoDataBase (citiesData);
            break;
        default:
            // TODO: emit a signal that alerts watchers of an error
            break;
    }

    // The reply is not needed now so we call deleteLater() function since we are in a slot.
    mReply->deleteLater();
    mReply = 0;
}

void CityDataSource::onSslErrors(QNetworkReply * reply, const QList<QSslError> & errors)
{
    // Ignore all SSL errors to be able to load from JSON file from the secure address.
    // It might be a good idea to display an error message indicating that security may be compromised.
    //
    // The errors we get are:
    // "SSL error: The issuer certificate of a locally looked up certificate could not be found"
    // "SSL error: The root CA certificate is not trusted for this purpose"
    // Seems to be a problem with how the server is set up and a known QT issue QTBUG-23625

    reply->ignoreSslErrors(errors);
}

void CityDataSource::onSqlConnectorReply(const bb::data::DataAccessReply& reply)
{
    if (reply.hasError()) {
        qWarning() << "WeatherModel: Database error, " << reply.errorType() << " "
                << reply.errorMessage() << " id: " << reply.id();
    } else {
        if (reply.id() == 0) {
            emit citiesChanged(mRevision);
        }
    }
}

void CityDataSource::setFavorite(QVariant id, bool favorite)
{
    // Update the database.
    if (id.canConvert(QVariant::Int)) {
        int revision = incrementRevision();

        // Update the SQL table with the new isFavorite boolean for the city.
        QVariantList dataList;
        dataList << favorite << revision << id.toInt();

        QString query = "UPDATE cities SET favorite=:favorite, revision_id=:revision_id WHERE citiesid=:id";
        mSqlConnector->execute(query, dataList);
    }
}

void CityDataSource::requestMoreDataFromNetwork(const QString continent)
{
    QUrl path;

    if (continent.contains("All")) {
        // We have one special case where we load "All Cities", if this
        // is selected we do not load all cities at once but instead load
        // an initial batch, then we continue to load the rest in another thread.
        path = AppSettings::prepareServerUrl("resources/cities");
    } else {
        // When loading a specific continent, we load the entire list synchronously.
        path = AppSettings::prepareServerUrl("resources/regions/" + continent);
    }

    mReply = mAccessManager.get(QNetworkRequest(path));

    // Connect to the reply finished signal to httpFinsihed() Slot function.
    connect(mReply, SIGNAL(finished()), this, SLOT(onHttpFinished()));
}

