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
NavigationPane {
    id: nav
    Page {
        titleBar: TitleBar {
            title: "Restaurants of Awesome"
        }
        Container {
            ListView {
                dataModel: XmlDataModel {
                    source: "data.xml"
                }
                onTriggered: {
                    var chosenItem = dataModel.data(indexPath);
                    var contentpage = itemPageDefinition.createObject();

                    contentpage.data = chosenItem
                    nav.push(contentpage);
                }
                listItemComponents: [
                    ListItemComponent {
                        type: "item"
                        StandardListItem {
                            title: ListItemData.name
                            description: ListItemData.location
                            imageSource: "asset:///images/"+ListItemData.image
                            
                        }
                    }
                ]
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
