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

WeatherDataSource::WeatherDataSource(QObject *parent) :
        QObject(parent), mRevision(0)
{
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
    // ValuesTable is a list of lists for adding a batch of data to the database.
    QVariantList valuesTable;

    // Update the data revision since we are adding new data to the data base.
    int revision = incrementRevision();

    QVariantList entryValues;

    // Store the retrieved values for adding to data base.
    entryValues << QVariant(city);
    entryValues << QVariant(region);
    entryValues << QVariant(15);
    entryValues << QVariant(30);;
    entryValues << QVariant(22);;
    entryValues << QVariant(1);;
    entryValues << QVariant("2013-12-24");
    entryValues << revision;
    valuesTable << QVariant::fromValue(entryValues);


    // Insert the data into the data base here.
    QString query = "INSERT INTO weather (city, region, templo, temphi, tempaverage, icon, date, revision_id) VALUES (:city, :region, :templo, :temphi, :tempaverage, :icon, :date, :revision_id)";
    mSqlConnector->executeBatch(query, valuesTable);
}
