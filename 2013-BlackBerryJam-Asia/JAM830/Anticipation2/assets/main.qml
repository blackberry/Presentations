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

Page {
    property int dataRevision: 0;
    Container {
        layout: DockLayout {
        }

        PullInfoContainer {
            id: pullInfo
            height: pullToRefresh.preferredHeight
            opacity: 0
            
            onDone: {
                // Trigger the update on the model.
                animQuery.emitDataChanged(dataRevision);
            }
        }

        ListView {
            id: animList
            dataModel: animModel
            property int listWidth: 0

			// The PullToRefresh CustomControl in this case is just monitoring a state
            // the visual update is handled in the PullInfoContainer.
            leadingVisual: PullToRefresh {
                id: pullToRefresh
                control: animList
                preferredHeight: 256

                onRefreshActiveChanged: {
                    if (active) {
                        animDataSource.addAtTop();
                        pullInfo.loading();
                    }                
                }
                
                onVisibleFractionChanged: {
                		pullInfo.update(visibleFraction);
                }
            }
            
            // Snap back to the leading edge if less then the full leading visual is shown.
            leadingVisualSnapThreshold: 1

            
            // Two item types are used, "item" is used for all items created at start up. The
            // "newItem" is used for items that are created during runtime. This will make it possible
            // to run an animation as the item is shown the first time, note that once that animation is
            // over the item is marked as an old item so that the animation will not be run again.
            listItemComponents: [
                ListItemComponent {
                    type: "item"
                    Item {
                        title: ListItemData.title
                        bandName: ListItemData.band
                        colorIndex: ListItemData.infoid
                        rating: ListItemData.rating
                    }
                },
                ListItemComponent {
                    type: "newItem"
                    NewItem {
                        id: newItem
                        title: ListItemData.title
                        bandName: ListItemData.band
                        colorIndex: ListItemData.infoid
                        rating: ListItemData.rating
                        
                        onItemShowing: {
                            newItem.ListItem.view.scrollToTop();   
                        }
                        
                        onItemDone: {
                            newItem.ListItem.view.markAsOld(ListItemData.infoid);
                        }
                    }
                }
            ]
            
            function itemType(data, indexPath) {
                
                // The new data is marked as newItem so that a special item can be loaded
                // the first time it is shown (an animation is triggered when it is created).
                if (data.newItem == "true") {
                    return "newItem";
                }
                
                return "item";
            }

            function scrollToTop() {
                animList.scrollToPosition(ScrollPosition.Beginning, ScrollAnimation.Smooth);
            }
            
            function markAsOld(id){
                animDataSource.setAsOld(id);
            }

            attachedObjects: [
                LayoutUpdateHandler {
                    onLayoutFrameChanged: {
                        animList.listWidth = layoutFrame.width
                    }
                }
            ]
        }
    }
    
    onCreationCompleted: {
        animModel.load();
    }
    
    attachedObjects: [
        // An AsyncDataModel requests data via a SqlDataQuery (new since 10.2) and
        // populates our list with items.
        AsyncDataModel {
            id: animModel
            
            query: SqlDataQuery {
                id: animQuery
                source: "file:///" + _app.getHomeDirectory() + "/anim.db"
                query: "SELECT * FROM animinfo ORDER BY infoid DESC"
                keyColumn: "infoid"
                countQuery: "SELECT count(*) FROM animinfo"
                revisionColumn: "revision_id"
                revisionQuery: "SELECT revision_id FROM revision WHERE table_name='animinfo'"
            }
        },
        // A custom data source for adding some data to the database used
        // by the AsyncDataModel above.
        AnimDataSource {
            id: animDataSource
            
            onDataChanged: {
                // Data has been added hide the pulltorefresh control and
                // tell the animQuery to update data with the new revision number.
                dataRevision = revision
                pullToRefresh.refreshDone();
                pullInfo.refreshDone();
            }
            
            function addAtTop() {
                // We use an xml data model to create new items for our demo.
                // In reality this could correspond to making a network request for data.
                var item = xmlData.getNewItem();
                animDataSource.addInfo(item.title, item.band, item.rating);
            }
            
            function setAsOld(id) {
                // Call the data source 
                animDataSource.markAsOld(id);
            }
        },
        // An xml data model used for prototyping the adding of new data.
        XmlDataModel {
            id: xmlData
            property int counter: 0
            source: "asset:///models/anim.xml"
            
            function getNewItem(){
                var index = counter%childCount([]);
                var itemData = data([index]);
                counter = counter + 1;
                return itemData;
            }
        }
    ]
}