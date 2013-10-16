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

// This small demo shows how to trigger animations as the user peeks back on a list.
NavigationPane {
    id: nav
    Page {
        id: myPage
        onPeekedAtChanged: {
            mainList.secretPeek = peekedAt
        }
        Container {

            ListView {
                id: mainList
                
                // In order to access the peak property inside list items
                // it has to be set on the ListView, this way we can listen for changes
                // to peek inside items and trigger an animation as the user peaks to the list.
                property bool secretPeek: false

                dataModel: XmlDataModel {
                    source: "models/data.xml"
                }

                listItemComponents: [
                    ListItemComponent {
                        type: "header"

                        HeaderItem {
                        }
                    },
                    ListItemComponent {
                        type: "toDay"
                        
                        TodayItem {
                        }
                    },
                    ListItemComponent {
                        type: "protected"

                        ProtectedItem {
                        }
                    },
                    ListItemComponent {
                        type: "ageOf"
                        
                        AgeOfItem {
                        }
                    }
                ]
                
                ListItem.onInitializedChanged: {
                    // As listitems become ready we start an animation, this will trigger an 
                    // wave like animation on start up.
                    showAnimation.play()
                }
                
                onTriggered: {
                    var chosenItem = dataModel.data(indexPath);
                    var type = dataModel.itemType(indexPath);

					// Only trigger animation if the item is not protected and not a header item.
                    if (type != "protected" && type != "header") {
                        var contentpage = itemPageDefinition.createObject();
                        contentpage.pageColor = Color.create(chosenItem.color);
                        contentpage.itemPageTitle = chosenItem.title
                        nav.push(contentpage);
                    }
                }
            }
        }
    }
    
    attachedObjects: [
        ComponentDefinition {
            id: itemPageDefinition
            source: "ItemPage.qml"
        }
    ]
    
    onPopTransitionEnded: {
        page.destroy();
    }
}
