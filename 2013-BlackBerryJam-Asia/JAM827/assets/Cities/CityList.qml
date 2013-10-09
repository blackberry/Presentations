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

ListView {
    id: cityList
    
    property bool actAsFavoriteList: true;
    
    // The city list can emit two signals: one for removing a city from the list and
    // another for updating the home city.
    signal changeFavoriteCity(variant id);
    signal updateHomeCity(string region, string city);
        
    listItemComponents: [
        ListItemComponent {
            type: "header"
            Header {
                title: ListItemData.header
            }
        },
        ListItemComponent {
            type: ""
            
            StandardListItem {
                id: cityItem
                title: ListItemData.city
                
                // Item context actions are items long-pressed on the menu so
                // actions will be shown.
                contextActions: [
                    ActionSet {
                        title: ListItemData.city
                        subtitle: qsTr("City actions") + Retranslate.onLanguageChanged
                        
                        ActionItem {
                            property bool actAsFavoriteList: cityItem.ListItem.view.actAsFavoriteList 
                            title: (actAsFavoriteList) ? qsTr("Remove from favorites") + Retranslate.onLanguageChanged :qsTr("Add to favorites") + Retranslate.onLanguageChanged
                            imageSource: "asset:///images/menuicons/icon_favorites.png"
                            enabled: (actAsFavoriteList) ? true : !ListItemData.favorite
                            
                            onTriggered: {
                                cityItem.ListItem.view.changeFavoriteCity(ListItemData.citiesid);
                            }
                        }
                        
                        ActionItem {
                            title: qsTr("Home city") + Retranslate.onLanguageChanged
                            imageSource: "asset:///images/menuicons/icon_home.png"
                            
                            onTriggered: {
                                // Emit signal that will make the current city the home city.
                                cityItem.ListItem.view.updateHomeCity(ListItemData.region, ListItemData.city);
                            }
                        }
                    }
                ]
            }
        }
    ]
}
