/* Copyright (c) 2013 Research In Motion Limited.
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

#include "favoritemodel.h"
#include <bb/data/DataAccessReply>
#include <bb/data/SqlConnection>

using namespace bb::cascades;

#define UPDATE_FAVORITES 4
#define INITIAL_LOAD_ID 10

const char* const FavoriteModel::mCityDatabase = "data/weatherhistory.db";

FavoriteModel::FavoriteModel(const QStringList &keys, const QString& connectionName, QObject *parent) :
        GroupDataModel(keys, parent)
{
    // Before we can open the database, we have to make sure it is in the read/write "data" folder.
    copyDbToDataFolder("weatherhistory.db");

    // Set up SQL connection.
    mSqlConnector = new SqlConnection(mCityDatabase, connectionName, this);

    loadFavoriteCities();
}

FavoriteModel::~FavoriteModel()
{
    if (mSqlConnector->isRunning()) {
        mSqlConnector->stop();
        mSqlConnector->deleteLater();
    }
}

bool FavoriteModel::copyDbToDataFolder(const QString databaseName)
{
    // Since we need read and write access to the database, it has
    // to be moved to a folder where we have access to it. First,
    // we check if the file already exists (previously copied).
    QString dataFolder = QDir::homePath();
    QString newFileName = dataFolder + "/" + databaseName;
    QFile newFile(newFileName);

    if (!newFile.exists()) {
        // If the file is not already in the data folder, we copy it from the
        // assets folder (read only) to the data folder (read and write).
        // Note: On a debug build, you will be able to write to a data base
        // in the assets folder but that is not possible on a signed application.
        QString appFolder(QDir::homePath());
        appFolder.chop(4);
        QString originalFileName = appFolder + "app/native/assets/models/sql/" + databaseName;
        QFile originalFile(originalFileName);

        if (originalFile.exists()) {
            return originalFile.copy(newFileName);
        } else {
            qDebug() << "Failed to copy file data base file does not exists.";
            return false;
        }
    }

    return true;
}

bool FavoriteModel::validReply(DataAccessReply *reply)
{
    if(reply->hasError()) {
        qWarning() << "Non valid SQL reply: " << reply->id() << ", SQL error: " << reply;
    }
    return true;
}

void FavoriteModel::addFavoriteCity(QString region, QString city)
{

    if (!region.isEmpty() && !city.isEmpty()) {
        // First check so that the entry is not already in the list
        QString selectQuery;
        QTextStream(&selectQuery) << "SELECT * FROM favorites WHERE region='" << region
                << "' AND name='" << city << "'";
        DataAccessReply selectReply = mSqlConnector->executeAndWait(selectQuery, UPDATE_FAVORITES);
        QVariantList cityData = selectReply.result().value<QVariantList>();

        if (validReply(&selectReply) && cityData.size() == 0) {
            // After setting a new city as favorite, we load the corresponding item data
            // and add it to the model. This will automatically make it appear in the list.
            QString query;
            QTextStream(&query) << "INSERT INTO favorites (region, name) VALUES ('" << region
                    << "', '" << city << "')";
            DataAccessReply reply = mSqlConnector->executeAndWait(query, UPDATE_FAVORITES);

            if (validReply(&reply)) {
                selectReply = mSqlConnector->executeAndWait(selectQuery, UPDATE_FAVORITES);

                if (validReply(&selectReply)) {
                    QVariantList cityData = selectReply.result().value<QVariantList>();

                    // Check if the city is already in the list, if its not we add it to the model.
                    // The first() function used since in our city data there is only one instance of each city.
                    QVariantMap addedItem = cityData.first().toMap();
                    insert(addedItem);
                }
            }
        }
    }
}

void FavoriteModel::removeFavoriteCity(QVariant id, QVariant removeIndexPath)
{
    // Update the database.
    if (id.canConvert(QVariant::Int)) {
        // Update the SQL table with the new isFavorite boolean for the city.
        QString query;
        QTextStream(&query) << "DELETE FROM favorites WHERE favoritesid=" << id.toInt();
        DataAccessReply reply = mSqlConnector->executeAndWait(query, UPDATE_FAVORITES);

        if (validReply(&reply)) {
            // Update model by removing the item with the corresponding city name.
            this->removeAt(removeIndexPath.toList());
        }
    }
}

void FavoriteModel::loadFavoriteCities()
{
    // First of all, we clear the model to make sure no items are lingering.
    this->clear();

    // Request all cities which have their favorite property set to true.
    QString query = "select * from favorites";

    // For the favorites list, we do not load the data asynchronously. We assume that
    // there will not be a huge number of favorite cities, thus executeAndWait is used.
    DataAccessReply reply = mSqlConnector->executeAndWait(query, INITIAL_LOAD_ID);

    if (validReply(&reply)) {
        QVariantList resultList = reply.result().value<QVariantList>();

        if (resultList.size() > 0) {
            // Insert the data into the model.
            this->insertList(resultList);
        }
    }
}

