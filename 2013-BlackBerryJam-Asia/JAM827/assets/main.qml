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
import "Weather"

// The different pages in the weather guesser is added to a TabbedPane.
TabbedPane {
    id: mainTab
    showTabsOnActionBar: true
    
    // The application's main menu with a 'Settings' button
    Menu.definition: MenuDefinition {
        settingsAction: SettingsActionItem {
            onTriggered : {
                settingsSheet.open();
            }
        }
    }

    
    Tab {
        title: qsTr("Home") + Retranslate.onLanguageChanged
        imageSource: "asset:///images/menuicons/icon_home.png"
        
        delegateActivationPolicy: TabDelegateActivationPolicy.ActivateImmediately
        
        delegate: Delegate {
            id: weatherPageDelegate
            
            WeatherPage {
                id: homeCityPage
                
                city: "Hong Kong"
                region: "Asia"
                homeWeather: true;
            }
        }
        
        onTriggered: {            
            // Check if the delegates object is defined, if so trigger a function to reset
            // the list in the weather page (the object) to the top position.
            if(weatherPageDelegate.object != undefined) {
                weatherPageDelegate.object.resetToTop();   
            }
        }        
    }
    
    // The city browse page where filtering is done based on continents.
    Tab {
        title: qsTr("Browse Cities") + Retranslate.onLanguageChanged
        imageSource: "asset:///images/menuicons/icon_browse.png"
                
        delegate: Delegate {
            source: "ContinentsTab.qml"
        }
    }
    
    // A tab where a list of favorite cities are shown
    Tab {
        title: qsTr("Favorites") + Retranslate.onLanguageChanged
        imageSource: "asset:///images/menuicons/icon_favorites.png"
        
        // For now always reload the Favorites tab since new cities might have been added.
        delegateActivationPolicy: TabDelegateActivationPolicy.ActivatedWhileSelected
        
        delegate: Delegate {
            source: "FavoriteTab.qml"
        } 
    }
    
    // An information page presenting some background of the application.
    Tab {
        title: qsTr("Info") + Retranslate.onLanguageChanged
        imageSource: "asset:///images/menuicons/icon_info.png"
        ActionBar.placement: ActionBarPlacement.InOverflow
        
        delegate: Delegate {
            source: "InfoTab.qml"
        }
    }

    attachedObjects: [
        SettingsSheet {
            id: settingsSheet
        }
    ]
}// TabbedPane