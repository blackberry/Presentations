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

Page {
    id: theMenu
    property variant data
    titleBar: TitleBar {
        title: data.name
    }
    Container {
        ListView {
            dataModel: XmlDataModel {
                source: theMenu.data.menu 
            }
            property variant restaurant: theMenu.data.name
            listItemComponents: [
                ListItemComponent {
                    type: "item"
                    StandardListItem {
                        id: menuItem
                        title: ListItemData.name
                        description: ListItemData.description
                        contextActions: [
                            ActionSet {
                                InvokeActionItem {
                                    id: actionId
                                    title: "Share"
                                    query{
                                        invokeActionId: "bb.action.SHARE"
                                        mimeType: "text/plain"
                                        data: "I'm eating "+ListItemData.name + " @ " + menuItem.ListItem.view.restaurant
                                        onQueryChanged: {
                                        	actionId.query.updateQuery()                                            
                                        }
                                    }
                                }
                            }
                        ]
                    }
                }
            ]
            onTriggered: {
                var imagePage = imageDef.createObject()
                var chosenItem = dataModel.data(indexPath)
                imagePage.imageSource = "images/"+chosenItem.image
                nav.push(imagePage)
            }
        }
        attachedObjects: [
            ComponentDefinition {
                id: imageDef
                source: "ImagePage.qml"
            }
        ]
    }
}
