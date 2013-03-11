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

#ifndef _FAVORITEMODEL_H_
#define _FAVORITEMODEL_H_

#include <bb/cascades/GroupDataModel>
#include <bb/data/SqlConnection>

using namespace bb::data;

namespace bb
{
    namespace data
    {
        class SqlConnection;
    }
}

/* CityModel Description:
 *
 * The city model class extends the GroupDataModel with functionality for
 * loading items from an SQL database. In our case, this approach solves the case of 
 * loading and setting up a model for a list of cities based on which continent
 * they belong to.
 */
class FavoriteModel: public bb::cascades::GroupDataModel
{
    Q_OBJECT

public:
    /**
     * The constructor is similar to the GroupDataModel constructor but in addition
     * it has a connectionName parameter used for setting up a unique SQL connection.
     *
     * @param keys Value to set on GroupDataModel::sortingKeys.
     * @param connectionName The name of the connection to the SQL database.
     * @param parent The data model owner or @c 0. Optional parameter; defaults to @c 0, if not specified.
     */
    FavoriteModel(const QStringList &keys, const QString& connectionName,
            QObject *parent = 0);
    ~FavoriteModel();

    /**
     * Adds a favorite city to the favorites model.
     *
     * @param region The name of the region where the city is situated.
     * @param city The name of the city which should be added to favorites.
     */
    Q_INVOKABLE void addFavoriteCity(QString region, QString city);

    /**
     * Removes a favorite city from the favorites model.
     *
     * @param id The database id of the entry that should be removed.
     * @param removeIndexPath The index path to the city item in the model.
     */
    Q_INVOKABLE void removeFavoriteCity(QVariant id, QVariant removeIndexPath);

private:
    /**
     * This function loads all items in the database where favorite=true to the model.
     */
    void loadFavoriteCities();

    /**
     * This function copies the bundled database file to the proper folder.
     * In order to write to a file in a signed application, the file has
     * to reside in the apps data folder.
     *
     * @param databaseName The name of the database (in assets/sql).
     * @return True if successful, false otherwise.
     */
    bool copyDbToDataFolder(const QString databaseName);


    /**
     * Checks if the DataAccessReply is valid.
     *
     * @return true if the reply was valid
     */
    bool validReply(DataAccessReply *reply);

    // Data base connector
    SqlConnection *mSqlConnector;

    static const char* const mCityDatabase;
};

#endif // ifndef _FAVORITEMODEL_H_
