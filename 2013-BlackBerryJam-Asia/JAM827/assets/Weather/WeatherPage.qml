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
import bb.system 1.2
import utils 1.0
import "../messages/"

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
            dataModel: loadModelDecorator

            onDataRequest: {
                weatherDataSource.requestWeatherData(item, requestOlderItems);
            }
        }
        
        ControlDelegate {
            id: errorDelegate
            source: "asset:///messages/Message.qml"
            horizontalAlignment: HorizontalAlignment.Fill
            delegateActive: false
        }
    }

    attachedObjects: [
        LoadModelDecorator {
            id: loadModelDecorator

            model: AsyncDataModel {
                id: weatherModel

                query: SqlDataQuery {
                    id: sqlDataQuery
                    source: "file:///" + _app.getHomeDirectory() + "/weatherhistory.db"
                    keyColumn: "weatherid"

                    // Make sure there is revision_id in the query, otherwise listview will not update smoothly
                    revisionColumn: "revision_id"
                    revisionQuery: "SELECT revision_id FROM revision WHERE table_name=:weather"

                    onDataChanged: {
                        console.log("data changed in SqlDataQuery");
                    }

                    onError: {
                        console.log("query error: " + code + ", " + message);
                    }
                }

                onLoaded: {
                    // If no items by this time make request for loading items.
                    var childCount = weatherModel.childCount(weatherList.rootIndexPath);
                    if (childCount == 0) {
                        weatherDataSource.requestWeatherData(undefined, true);
                    }
                }
            }
        },
        WeatherDataSource {
            id: weatherDataSource
            
            onWeatherChanged: {
                // Switch off loading items and tell the model that new data with revision is available.
                sqlDataQuery.emitDataChanged(revision);
                
                // Turn off UI components indicating data loading.
                resetLoadIndicators();
            }
            
            onNoMoreWeather: {
                resetLoadIndicators();
            }
            
            onErrorCodeChanged: {
                
                if (loadModelDecorator.loadingOldItems && errorCode != WeatherError.NoError && errorCode != undefined) {
                    if (errorCode == WeatherError.ServerError || errorCode == WeatherError.ServerBusy || errorCode == WeatherError.JsonError ) {
                        // Just show the user a hud to reload.
                        errorToast.body = qsTr("Oh ooh, an error occured. Please try again.");
                        errorToast.show();
                        
                        // Only do this if at end of the list.
                        loadModelDecorator.temporaryHideDecorator();          
                    } else if (errorCode == WeatherError.InvalidCity) {
                        // This is error is non-recoverable there is no data for the city. 
                        errorDelegate.delegateActive = true;
                        errorDelegate.control.message = qsTr("You seem lost, the city you are looking for is not there, check the map again.");
                    }
                    
                    resetLoadIndicators();
                } else {
                    // Never show the delegate if no error has occured.
                    errorDelegate.delegateActive = false;
                }
            }
            
            function requestWeatherData(item, requestOlderItems) {
                if (item == undefined) {
                    requestMoreDataFromNetwork(region, city, "", true);
                } else {
                    requestMoreDataFromNetwork(region, city, item.date, requestOlderItems);
                }
                loadModelDecorator.loadingOldItems = requestOlderItems;
            }
        },
        SystemToast {
            id: errorToast
        }
    ]

    onCreationCompleted: {
        if (homeWeather && city != "" && region != "") {
            initHomeWeather();
        }
    }
    
    function resetLoadIndicators() {
        weatherList.pullToRefresh.refreshDone();
        loadModelDecorator.loadingOldItems = false;
    }

    function initHomeWeather() {
        // The properties of a SqlDataQuery can only be set once, so we do it once we have the city and region.
        // The weather page is showing the home location the data depends on which city is set in the settings table.
        sqlDataQuery.bindValues = {
            "weather": "weather"
        }

        sqlDataQuery.query = "SELECT * FROM weather INNER JOIN settings ON weather.city=settings.city AND weather.region=settings.region ORDER by date DESC";
        sqlDataQuery.countQuery = "SELECT count(*) from weather INNER JOIN settings ON weather.city=settings.city AND weather.region=settings.region";

        // Connect to home city changed signal, in order to reload data if the home location changes.
        _appSettings.homeChanged.connect(onNewHome); //change function.
        weatherModel.load();
    }

    function onNewHome(home) {
        // The weatherpage is showing the home location weather and the user has selected a new home town, 
        region = home.region;
        city = home.city;
        
        // To trigger a reload of the data with the new hometown update the overall revision.
        sqlDataQuery.emitDataChanged(weatherDataSource.incrementRevision());
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
