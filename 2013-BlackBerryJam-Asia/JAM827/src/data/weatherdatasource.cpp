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
        QObject(parent), mReply(0), mRevision(0), mErrorCode(WeatherError::NoError)
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
    int httpStatus = -1; // controls the final behavior of this function

    if (mReply->error() == QNetworkReply::NoError) {
        // Load the data using the reply QIODevice.
        weatherDataFromServer = jda.load(mReply).value<QVariantList>();

        if (jda.hasError()) {
            bb::data::DataAccessError error = jda.error();
            qDebug() << "JSON loading error:" << error.errorType() << " : " << error.errorMessage();
            httpStatus = -2;
        } else {
            httpStatus = 200;
        }
    } else {
        // An error occurred, try to get the http status code and reason
        QVariant statusCode = mReply->attribute(QNetworkRequest::HttpStatusCodeAttribute);
        QString reason = mReply->attribute(QNetworkRequest::HttpReasonPhraseAttribute).toString();

        if (statusCode.isValid()) {
            httpStatus = statusCode.toInt();
        }

        qDebug() << "Network request to " << mReply->request().url().toString()
                << " failed with http status " << httpStatus << " " << reason;
    }

    // Now behave
    switch (httpStatus) {
        case 200:
            loadNetworkReplyDataIntoDataBase(weatherDataFromServer);
            break;
        case 404:
            if (mCursor.index == 0) {
                // If we requested index 0 and didn't get an empty array it means the city does not exist and we should show an error
                setErrorCode(WeatherError::InvalidCity);
            } else {
                // If we get a 404 in the middle of a data set it simply means there is no more data
                emit noMoreWeather();
            }
            break;
        case 503:
            // TODO: perhaps try again a few times and eventually just stop? if we end up stopping and the list is empty, show an alert message. if the list isn't empty just stop fetching
            setErrorCode(WeatherError::ServerBusy);
            break;
        case -2:
            setErrorCode(WeatherError::JsonError);
            break;
        case 500:
        default:
            // The server crapped out, if we don't have any entries let the user know an error occurred, otherwise just stop fetching
            setErrorCode(WeatherError::ServerError);
            break;
    }

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

WeatherError::Type WeatherDataSource::errorCode()
{
    return mErrorCode;
}

void WeatherDataSource::setErrorCode(WeatherError::Type error)
{
    mErrorCode = error;
    emit errorCodeChanged(mErrorCode);
}

void WeatherDataSource::requestMoreDataFromNetwork(const QString region, const QString city,
        const QString date, bool requestOlderItems)
{
    // Only request data if there is currently no request being done.
    if (mReply == 0) {
        int dayOffset = 0;
        QString encodedCity = QUrl(city).toEncoded();
        QString encodedRegion = QUrl(region).toEncoded();
        QUrl path = AppSettings::prepareServerUrl("resources/cities/" + encodedRegion + "/" + encodedCity, true);

        // An empty date string mean request data with 0 days offset.
        if (!date.isEmpty()) {
            // Server requests is made with an offset as compared to today
            QDate today = QDate::currentDate();
            QDate compareDate = QDate::fromString(date, "yyyy-MM-dd");
            dayOffset = compareDate.daysTo(today);

            if (requestOlderItems) {
                // Add one to day offset to begin at the next date after the "date" parameter.
                dayOffset += 1;
            } else {
                // For newer items than date we need to know how many items are set to be requested each time.
                uint chunkSize = AppSettings::loadSize();
                dayOffset -= chunkSize;

                if (dayOffset <= 0) {
                    // A negative offset means future data, which is not possible, we have to
                    // adjust the chunksize and set the dayOffset to 0 i.e. today.
                    chunkSize = chunkSize + dayOffset;
                    dayOffset = 0;

                    if (chunkSize == 0) {
                        // If the adjusted chunk size become 0, all data up to current date has been received and we are done.
                        emit noMoreWeather();
                        return;
                    }
                    // Request a new path with the custom chunksize.
                    path = AppSettings::prepareServerUrl("resources/cities/" + encodedRegion + "/" + encodedCity, true, chunkSize);
                }
            }
        }

        // Add the offset for the server request, corresponds to from which day in the pas the request is made.
        path.addQueryItem("start", QString("%1").arg(dayOffset));

        // Reset the error code.
        setErrorCode(WeatherError::NoError);

        qDebug() << "GET " << path.toString();
        mReply = mAccessManager.get(QNetworkRequest(path));

        // Connect to the reply finished signal to httpFinsihed() Slot function.
        connect(mReply, SIGNAL(finished()), this, SLOT(onHttpFinished()));

        mCursor.index = dayOffset;
    }
}
