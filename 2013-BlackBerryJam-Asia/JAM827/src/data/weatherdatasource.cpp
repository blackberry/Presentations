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
#include "weatherdatasource.h"
#include "appsettings.h"
#include "globalconnection.h"

#include <bb/data/JsonDataAccess>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkReply>
#include <QDate>

WeatherDataSource::WeatherDataSource(QObject *parent) :
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

int WeatherDataSource::incrementRevision()
{
    QVariantList dataList;
    dataList << "weather";
    mSqlConnector->executeAndWait("UPDATE revision SET revision_id = revision_id + 1 WHERE table_name=:weather",
            dataList, 1);

    DataAccessReply reply = mSqlConnector->executeAndWait("SELECT revision_id FROM revision WHERE table_name=:weather", dataList, 2);

    QVariantList resultList = reply.result().toList();
    QVariantMap resultMap = resultList.at(0).toMap();
    mRevision = resultMap["revision_id"].toInt();

    qDebug() << "Revision is now " << mRevision;
    return mRevision;
}

void WeatherDataSource::loadNetworkReplyDataIntoDataBase(QVariantList weatherData)
{
    // Iterate over all the items in the received data.
    QVariantList::Iterator item = weatherData.begin();

    // ValuesTable is a list of lists for adding a batch of data to the database.
    QVariantList valuesTable;

    // Update the data revision since we are adding new data to the data base.
    int revision = incrementRevision();

    while (item != weatherData.end()) {
        QVariantList entryValues;
        QVariantMap itemMap = (*item).toMap();
        QDate itemDate = QDate::fromString(itemMap["date"].toString(), "yyyy M d");
        itemMap["date"] = QVariant(itemDate.toString("yyyy-MM-dd"));

        // Store the retrieved values for adding to data base.
        entryValues << itemMap["city"];
        entryValues << itemMap["region"];
        entryValues << itemMap["templo"].toInt();
        entryValues << itemMap["temphi"].toInt();
        entryValues << itemMap["tempaverage"].toInt();
        entryValues << itemMap["icon"].toInt();
        entryValues << itemMap["date"].toString();
        entryValues << revision;
        valuesTable << QVariant::fromValue(entryValues);
        ++item;
    }

    // Insert the data into the data base here.
    QString query = "INSERT INTO weather (city, region, templo, temphi, tempaverage, icon, date, revision_id) VALUES (:city, :region, :templo, :temphi, :tempaverage, :icon, :date, :revision_id)";
    mSqlConnector->executeBatch(query, valuesTable);
}

void WeatherDataSource::onHttpFinished()
{
    JsonDataAccess jda;
    QVariantList weatherDataFromServer;

    if (mReply->error() == QNetworkReply::NoError) {
        // Load the data.
        weatherDataFromServer = jda.load(mReply).value<QVariantList>();
        loadNetworkReplyDataIntoDataBase(weatherDataFromServer);
    } else {
        // An error occurred, try to get the http status code and reason
        QVariant statusCode = mReply->attribute(QNetworkRequest::HttpStatusCodeAttribute);
        QString reason = mReply->attribute(QNetworkRequest::HttpReasonPhraseAttribute).toString();

        qDebug() << "Network request to " << mReply->request().url().toString()
                << " failed with http status " << " " << reason;
    }

    // Handle error and send notification.

    // The reply is not needed now so we call deleteLater() function since we are in a slot.
    mReply->deleteLater();
    mReply = 0;
}

void WeatherDataSource::onSslErrors(QNetworkReply * reply, const QList<QSslError> & errors)
{
    // Ignore all SSL errors to be able to load from JSON file from the secure address.
    // It might be a good idea to display an error message indicating that security may be compromised.
    reply->ignoreSslErrors(errors);
}

void WeatherDataSource::onSqlConnectorReply(const bb::data::DataAccessReply& reply)
{
    if (reply.hasError()) {
        qWarning() << "WeatherModel: Database error, " << reply.errorType() << " "
                << reply.errorMessage() << " id: " << reply.id();
    } else {
        if (reply.id() == 0) {
            emit weatherChanged(mRevision);
        }
    }
}

void WeatherDataSource::requestMoreDataFromNetwork(const QString region, const QString city)
{
    // Only request data if there is currently no request being done.
    if (mReply == 0) {
        QString encodedCity = QUrl(city).toEncoded();
        QString encodedRegion = QUrl(region).toEncoded();
        QUrl path = AppSettings::prepareServerUrl(
                "resources/cities/" + encodedRegion + "/" + encodedCity, true);

        qDebug() << "GET " << path.toString();
        mReply = mAccessManager.get(QNetworkRequest(path));

        // Connect to the reply finished signal to httpFinsihed() Slot function.
        connect(mReply, SIGNAL(finished()), this, SLOT(onHttpFinished()));
    }
}
