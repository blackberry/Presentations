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
#ifndef _WEATHERMODEL_H_
#define _WEATHERMODEL_H_

#include <bb/cascades/QListDataModel>

// The weather model is based on the QListDataModel template, which in turn
// implements the abstract DataModel class.
typedef bb::cascades::QListDataModel<QVariant> WeatherListModel;

/* WeatherModel Description:
 *
 * The weather model class is used by the weather history application to handle
 * all weather data. It connects to a "weather service" that is simulated
 * by requesting a file from the Internet, the file reply is passed to the
 * JsonDataAccess class which is used to set up the model.
 */
class WeatherModel: public WeatherListModel
{
    Q_OBJECT

    /**
     * The city property determines which weather data file is requested
     * from the remote server.
     */
    Q_PROPERTY(QString city READ city WRITE setCity NOTIFY cityChanged)

    /**
     * The region property determines which region should be used for the city.
     */
    Q_PROPERTY(QString region READ region WRITE setRegion NOTIFY regionChanged)

public:

    /**
     * This is our constructor. This class inherits from GroupDataModel. The parent object is set,
     * reply is initialized, and ssl error signal handling is setup.
     *
     * @param parent The parent object, if not set, @c 0 is used
     */
    WeatherModel(QObject *parent = 0);

    /**
     * Returns the item type for the data item at indexPath.
     *
     * @param indexPath The index path to the data item.
     * @return A string representing the type of the item.
     */
    QString itemType (const QVariantList &indexPath);

    /**
     * This function sets the city to obtain mock weather data for.
     *
     * @param city The name of the city
     */
    void setCity(QString city);

    /**
     * This function returns the name of the city that the mock weather data is for.
     *
     * @return The name of the current set city for mock weather data
     */
    QString city();

    /**
     * This function sets the region where the city is situated.
     *
     * @param region The name of the region
     */
    void setRegion(QString region);

    /**
     * This function returns the name of the region the city is in.
     *
     * @return The name of the current region the city is in
     */
    QString region();

    /**
     * A call to request more data for the current city is made via a
     * call to the requestMoreDataFromNetwork function.
     */
     void requestMoreDataFromNetwork();

signals:

    /**
     * Signal emitted when the city property has changed value.
     */
    void cityChanged(QString city);

    /**
     * Signal emitted when the region property has changed value.
     */
    void regionChanged(QString city);

private:
    // The current city for obtaining mock weather data
    QString mCity;
    QString mRegion;
};

#endif // ifndef _WEATHERMODEL_H_
