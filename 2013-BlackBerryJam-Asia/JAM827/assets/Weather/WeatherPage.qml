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
import utils 1.0

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
        AsyncDataModel {
            id: weatherModel

            query: SqlDataQuery {
                id: sqlDataQuery
                source: "file:///" + _app.getHomeDirectory() + "/weatherhistory.db"
                keyColumn: "weatherid"

                // Make sure there is revision_id in the query, otherwise listview will not update smoothly
                revisionColumn: "revision_id"
                revisionQuery: "SELECT revision_id FROM revision WHERE table_name=:weather"
            }

            onLoaded: {
                // If no items by this time make request for loading items.
                var childCount = weatherModel.childCount(weatherList.rootIndexPath);
                if (childCount == 0) {
                    weatherDataSource.requestWeatherData();
                }
            }
        },
        WeatherDataSource {
            id: weatherDataSource
            
            onWeatherChanged: {
                // Switch off loading items and tell the model that new data with revision is available.
                sqlDataQuery.emitDataChanged(revision);
             }
            
            function requestWeatherData() {
            	requestMoreDataFromNetwork(region, city);
            }
        }
    ]

    onCreationCompleted: {
        if (homeWeather && city != "" && region != "") {
            setLocation(region, city);
        }
    }

    function setLocation(weatherRegion, weatherCity) {
        // The properties of a SqlDataQuery can only be set once, so we do it once we have the city and region.
        region = weatherRegion;
        city = weatherCity;

        // The page is showing weather for a specfic city and region.
        sqlDataQuery.bindValues = {
            "weather": "weather",
            "city": city,
            "region": region
        }

        sqlDataQuery.query = "SELECT * FROM weather WHERE city=:city AND region=:region ORDER by date DESC";
        sqlDataQuery.countQuery = "SELECT count(*) from weather WHERE city=:city AND region=:region";
        weatherModel.load();
    }
    
    function resetToTop() {
        weatherList.scrollToPosition(ScrollPosition.Beginning, ScrollAnimation.None);
    }
}
