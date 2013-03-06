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
#include "weathermodel.h"
#include "weatherhistoryapp.h"
#include "logging.h"

using namespace bb::cascades;

WeatherModel::WeatherModel(QObject *parent)
{
	Q_UNUSED(parent);
}

QString WeatherModel::itemType(const QVariantList& indexPath)
{
    QString typeName("item");
    return typeName;
}

void WeatherModel::requestMoreDataFromNetwork()
{
    // Add some dummy items
    //     <item icon="3" templo="4" temphi="14" tempaverage="11" date="2012-01-01"/>

    QVariantMap map1;
    map1["icon"] = QString("1");
    map1["templo"] = QString("4");
    map1["temphi"] = QString("10");
    map1["tempaverage"] = QString("6");
    map1["date"] = QDate(2013, 02, 06);
    append(map1);

    QVariantMap map2;
    map2["icon"] = QString("3");
    map2["templo"] = QString("0");
    map2["temphi"] = QString("11");
    map2["tempaverage"] = QString("3");
    map2["date"] = QDate(2013, 02, 07);
    append(map2);

}

void WeatherModel::setCity(QString city)
{
    if (mCity.compare(city) != 0)
    {
        // Remove all the old data.
        this->clear();
        mCity = city;
        emit cityChanged(city);

        // The data for the city is requested.
        requestMoreDataFromNetwork();
    }
}

QString WeatherModel::city()
{
    return mCity;
}


void WeatherModel::setRegion(QString region)
{
    if (mRegion.compare(region) != 0) {
        mRegion = region;
        emit regionChanged(region);
    }
}

QString WeatherModel::region()
{
    return mRegion;
}
