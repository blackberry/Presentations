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

// A page used to present a list of cities for a specific continent.
// The real data is managed in code and read from an SQL database.
Page {
    id: continentsCitiesPage
    property NavigationPane nav;
    property string continent;
        
    titleBar: TitleBar {
        id: title
        title: continent
    }
    
    Container {
        layout: DockLayout {
        }
        
        CityList {
            id: cityList
            dataModel: cityModel
            
            onTriggered: {
                if( indexPath.length > 1 ) {
                    // When a non header item is selected, we push the Weather page for the city.
                    var chosenItem = dataModel.data(indexPath);
                    var weatherPage = weatherPageDefinition.createObject();
                    weatherPage.setLocation(chosenItem.region, chosenItem.city);
                    nav.push(weatherPage);
                }
            }
        }
    }
        
    attachedObjects: [
        AsyncHeaderDataModel {
            id: cityModel
            
            query: SqlHeaderDataQuery {
                id: cityQuery
                source: "file:///" + _app.getHomeDirectory() + "/weatherhistory.db";

                onError: {
                    console.log("query error: " + code + ", " + message);
                }
            }
        },
        ComponentDefinition {
            id: weatherPageDefinition
            source: "asset:///Weather/WeatherPage.qml"
        }
    ]
    
    function setLocation(cityContinent) {  
        // This page is created via a ComponentDefinition so we wait until we know the continent
        // before we can set up the SqlDataQuery properties (they can only be set once).
        continent = cityContinent;
        
        // The page is showing weather for a specfic city and region.
        cityQuery.bindValues = {
            "continent": continent
        }
        
        if(continent == "All cities") {
            // Special case, the all continetnts category was selected.
            cityQuery.query = "SELECT * FROM cities ORDER by city";
            cityQuery.countQuery = "SELECT count(*) FROM cities";
            cityQuery.headerQuery = "SELECT substr(city, 1, 1) AS header, count(*) FROM cities GROUP BY header";
            
        } else {
            // Set up a query to get cities belonging to a specific continent.
            cityQuery.query = "SELECT * FROM cities WHERE region=:continent ORDER by city";
            cityQuery.countQuery = "SELECT count(*) FROM cities WHERE region=:continent";
            cityQuery.headerQuery = "SELECT substr(city, 1, 1) AS header, count(*) FROM cities WHERE region=:continent GROUP BY header";            
        }

		// Load the data.        
        cityModel.load();
    }
}
