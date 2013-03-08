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

#ifndef _CITYMODEL_H_
#define _CITYMODEL_H_

#include <bb/cascades/GroupDataModel>
#include <bb/data/JsonDataAccess>
#include <QtNetwork/QNetworkReply>

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
 * they belong to. It also has the option of loading items that has been marked as
 * favorites in the database. Manipulating the city's favorites flag can also be done
 * via the database.
 */
class CityModel: public bb::cascades::GroupDataModel
{
    Q_OBJECT

    Q_PROPERTY(QString continent READ continent WRITE setContinent
            NOTIFY continentChanged)

    /**
     * Property that will when the first batch is being loaded for the model
     */
    Q_PROPERTY(bool initialLoad READ initialLoad NOTIFY initialLoadChanged)


public:
    /**
     * The constructor is similar to the GroupDataModel constructor but in addition
     * it has a connectionName parameter used for setting up a unique SQL connection.
     *
     * @param keys Value to set on GroupDataModel::sortingKeys.
     * @param connectionName The name of the connection to the SQL database.
     * @param parent The data model owner or @c 0. Optional parameter; defaults to @c 0, if not specified.
     */
    CityModel(const QStringList &keys, QObject *parent = 0);
    ~CityModel();

    /**
     * This function sets a new continent based on the parameter passed.
     *
     * @param continent A QString of the name of the new continent
     */
    void setContinent(QString continent);

    QString continent();

    /**
     * This function returns the initial load status, will be true only as long
     * as the first batch is being loaded into the model.
     *
     * @return true if the first load is in progress and no items are present in the model yet
     */
    bool initialLoad();


    signals:
    void continentChanged();

    /**
     * Signal emitted when the loading status of the first chunk of data changes
     */
    void initialLoadChanged(bool initialLoad);

public slots:
    /**
     * This function completely resets the model and reloads all
     * data, used if for example the server connection address changed.
     */
    void reset();

private slots:
    /**
     * This Slot function is called when the network request to the
     * "weather service" is complete.
     */
    void httpFinished();

    /**
     * This Slot function is connected to the mAccessManager sslErrors signal. This function
     * allows us to see what errors we get when connecting to the address given by mServerUrl.
     *
     * @param reply The network reply
     * @param errors SSL Error List
     */
    void onSslErrors(QNetworkReply * reply, const QList<QSslError> & errors);

private:
    /**
     * This function sets the continent filter for the model. It will load all cities
     * belonging to a particular continent. Also it can load all cities if the continent
     * parameter contains "All"
     *
     * @param continent The name of the continent from which the model should be populated.
     */
    void changeContinent(QString continent);

    /**
     * Sets the status of the initial load and emits a signal if the status changed
     *
     * @parameter newStatus A boolean of the new status
     */
    void setInitialLoad(bool newStatus);

    // The network parameters; used for accessing a file from the Internet
    QNetworkAccessManager mAccessManager;
    QNetworkReply *mReply;

    QString mContinent;
    bool mInitialLoad;
};

#endif // ifndef _CITYMODEL_H_
