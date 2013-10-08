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
import bb.cascades 1.2
import bb.cascades.datamanager 1.2

// The Weather page; where weather data is presented in a list with custom items.
Page {
    id: weather
    property string city: "Hong Kong"
    property string region
    property bool homeWeather: false

    titleBar: TitleBar {
        id: titleBar
        title: city
    }

    Container {
        WeatherList {
            id: weatherList
            dataModel: weatherModel
        }
    }

    attachedObjects: [
        SimpleQueryDataModel {
            id: weatherModel

            query: SqlDataQuery {
                id: sqlDataQuery
                source: "file:///" + _app.getHomeDirectory() + "/weatherhistory.db"
                query: "SELECT * FROM weather WHERE city=:city AND region=:region ORDER by date DESC";
                countQuery: "SELECT count(*) from weather WHERE city=:city AND region=:region"

                bindValues: {
                    "weather": "weather",
                    "city": city,
                    "region": region
                }
            }
        }
    ]

    onCreationCompleted: {
        weatherModel.load();
    }
    
    function resetToTop() {
        weatherList.scrollToPosition(ScrollPosition.Beginning, ScrollAnimation.None);
    }
}
