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
import bb.cascades 1.0
import "../messages"

// The Weather page; where weather data is presented in a list with custom items.
Page {
    id: weather
    property alias city: titleBar.title
    property alias weatherData: weatherList.dataModel
    property variant error: weatherList.dataModel.errorCode

    titleBar: TitleBar {
        id: titleBar
        title: "Göteborg"
        visibility: ChromeVisibility.Overlay
    }
    
    Container {
        Container {
            id: listviewContainer
            
            layout: DockLayout {
            }

            // An activity indicator that will be shown when weather data is loaded
            ActivityIndicator {
                id: dataLoadIndicator
                preferredWidth: 400
                preferredHeight: 400
                horizontalAlignment: HorizontalAlignment.Center
                running: weatherList.dataModel.initialLoad;
            }

            // The list of weather forecasts
            ListView {
                id: weatherList
                                
                // An XML model is also available to populate the preview.
                dataModel: XmlDataModel {source: "../models/weather.xml"}

                listItemComponents: [
                    ListItemComponent {
                        type: "item"
                        WeatherItem {
                        }
                    },
                    ListItemComponent {
                        type: "loadItem"
                        WeatherLoadItem {
                        }
                    }
                ]

                attachedObjects: [
                    ListScrollStateHandler {
                        onAtEndChanged: {
                            if (atEnd) {
                                // When we are at the end of the list request more data.
                                weatherList.dataModel.requestMoreDataFromNetwork();
                            }
                        }
                    }
                ]
            }
        }
        
        ErrorMessage {
            id: errorMessage
            horizontalAlignment: HorizontalAlignment.Fill
            verticalAlignment: VerticalAlignment.Top
            visible: false
        }
    }

    // onErrorChanged is automagically bound to the "property variant error" and is fired when it changes
    onErrorChanged: {
        var showErrorPage = true;
        var errorText = ""
        
        if(error == WeatherModel.ServerError) {
            errorText = qsTr("Sorry no fish, there was an error on the server.")
        } else if(error == WeatherModel.ServerBusy){
            errorText = qsTr("Hmm, looks like the server is busy right now, please try again later.")
        } else if (error == WeatherModel.JsonError) {
            errorText = qsTr("Something strange is going on, I can not give you any data right now.")
        } else if (error == WeatherModel.InvalidCity) {
            errorText = qsTr("You seem lost, the city you are looking for is not there, check the map again.")
        } else if (error == WeatherModel.NoError || error == undefined) {
            showErrorPage = false
        }

        if(showErrorPage) {
            errorMessage.message = errorText
            errorMessage.show.play();
            listviewContainer.setVisible(false);
            titleBar.visibility = ChromeVisibility.Visible; // when we display the error page, we don't want it hiding behind the titlebar
        }
    }
    
    function resetToTop(){
        weatherList.scrollToPosition(ScrollPosition.Beginning, ScrollAnimation.None);
    }
}
