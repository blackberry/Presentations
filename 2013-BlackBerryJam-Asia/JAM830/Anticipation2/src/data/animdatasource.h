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

#ifndef _CityDataSource_H_
#define _CityDataSource_H_

#include <QObject>
#include <bb/data/SqlConnection>


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
class AnimDataSource: public QObject
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
    AnimDataSource(QObject *parent = 0);
    ~AnimDataSource();

    /**
     *
     *
     * @param id
     * @param favorite
     */
    Q_INVOKABLE void addInfo(const QString title, const QString band, int rating);

    Q_INVOKABLE void markAsOld(int id);

    /**
     * Returns the current global revision of the database plus one.
     *
     * @return An integer with next revision.
     */
    Q_INVOKABLE int incrementRevision();

signals:

    /**
     * Signal emitted when new cities have been added to the database.
     */
    void dataChanged(int revision);


private slots:

    /**
     * Function used to connect to the SqlConnectors reply signal, this is
     * when data base queries have been executed.
     *
     * @param DataAccessReply reply The Database reply
     */
    void onSqlConnectorReply(const bb::data::DataAccessReply& reply);

private:
    Q_SLOT void fakeNetworkDelay();

    // Data base connector.
    bb::data::SqlConnection *mSqlConnector;

    // The last accessed data revision.
    int mRevision;

};

#endif // ifndef _CityDataSource_H_
