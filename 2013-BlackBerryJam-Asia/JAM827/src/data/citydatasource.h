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
#ifndef _CityDataSource_H_
#define _CityDataSource_H_

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
class CityDataSource: public QObject
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
    CityDataSource(QObject *parent = 0);
    ~CityDataSource();

    /**
     *
     *
     * @param id
     * @param favorite
     */
    Q_INVOKABLE void setFavorite(QVariant id, bool favorite);

    /**
     * Call this function to request city data.
     *
     * @param continent The continent for which to retrieve cities.
     */
    Q_INVOKABLE void requestMoreDataFromNetwork(const QString continent);

    /**
     * Returns the current global revision of the database plus one.
     *
     * @return An integer with next revision.
     */
    Q_INVOKABLE int incrementRevision();

signals:
    /**
     * Signal emitted when a change has happened in the favorites data base.
     *
     * @param the current revision number
     */
    void favoritesChanged(int revision);

    /**
     * Signal emitted when new cities have been added to the database.
     */
    void citiesChanged(int revision);


private slots:
    /**
     * This Slot function is called when the network request to the
     * "weather service" is complete.
     */
    void onHttpFinished();

    /**
     * This Slot function is connected to the mAccessManager sslErrors signal. This function
     * allows us to see what errors we get when connecting to the address given by mServerUrl.
     *
     * @param reply The network reply
     * @param errors SSL Error List
     */
    void onSslErrors(QNetworkReply * reply, const QList<QSslError> & errors);

    /**
     * Function used to connect to the SqlConnectors reply signal, this is
     * when data base queries have been executed.
     *
     * @param DataAccessReply reply The Database reply
     */
    void onSqlConnectorReply(const bb::data::DataAccessReply& reply);

private:
    /**
     * This Helper function used to store the data recieved from the network.
     *
     * @param data The QVariantList with weather data that is to be added to the database.
     */
    void loadNetworkReplyDataIntoDataBase(QVariantList data);

    // The network parameters; used for accessing a file from the Internet
    QNetworkAccessManager mAccessManager;
    QNetworkReply *mReply;

    // Data base connector.
    SqlConnection *mSqlConnector;

    // The last accessed data revision.
    int mRevision;

};

#endif // ifndef _CityDataSource_H_
