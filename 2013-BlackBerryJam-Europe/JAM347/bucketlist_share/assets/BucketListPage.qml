/* Copyright (c) 2012 Research In Motion Limited.
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

// Import the bucketmodel set as a type in the application constructor
import com.bucketlist.bucketdata 1.0

Page {
    id: bucketListPage
//    property variant incomingBucketItem: _theApp.bucketItemTitle
    

    //The slot used when the application is invoked with a .buk file.    
    function onIncomingBucketItem() 
    {
        addNew.open();
        addNew.text = _theApp.bucketItemTitle;
    }

    // Signal that tells the application that the BBM status should be updated
    signal newBBMStatus(string message, string icon)
    titleBar: TitleBar {
        id: segmentedTitle
        kind: TitleBarKind.Segmented

        // The segmented control decides which filter should be set on the
        // dataModel used by the photo bucket list.
        options: [
            Option {
                text: "Todo"
                value: "todo"
            },
            Option {
                text: "Finished"
                value: "finished"
            },
            Option {
                text: "Chickened out"
                value: "chickened"
            }
        ]
        onSelectedValueChanged: {
            // When a new Option is selected the dataModel of the ListView, the bucketModels
            // filter is set and the list is repopulated.
            bucketModel.filter = selectedValue;
            if (selectedValue != "todo") {
                bucketListPage.actionBarVisibility = ChromeVisibility.Hidden
            } else {
                bucketListPage.actionBarVisibility = ChromeVisibility.Visible
            }
        }
    }
    Container {
        topPadding: 20

        // The ListView is a separate QML component kept in BucketList.qml
        BucketList {
            id: bucketList
            property alias bucketModel: bucketModel
            attachedObjects: [
                // The bucket model is a non visible object so it is set up as an attached object.
                // The model itself is a QListDataModel defined in bucketmodel.h and registered
                // as a type in the creation of the application.
                BucketModel {
                    id: bucketModel

                    // The path to the JSON file with initial data, this file will be moved to
                    // the data folder on the first launch of the application (in order to
                    // be able to get write access).
                    jsonAssetPath: "app/native/assets/models/bucket.json"

                    // The filtering is initially set to "todo" to show items which has not
                    // been checked off the list so far.
                    filter: "todo"
                }
            ]
        }
    }

    // Attached objects of the Bucket List Page
    attachedObjects: [
        EditSheet {
            // A sheet is used to add new items to the list, which is the same sheet used to edit items
            id: addNew
            onSaveBucketItem: {
                bucketModel.addBucketItem(text);
                bucketList.scrollToPosition(ScrollPosition.Top, ScrollAnimation.Default);
            }
        },
        ComponentDefinition {
            // A Component definition of the Page used to display more details on the Bucket item.
            id: bucketPage
            source: "BucketPage.qml"
        }
    ]
    actions: [
        ActionItem {

            // An ActionItem for adding more items to the list
            title: "Add"
            imageSource: "asset:///images/add.png"
            ActionBar.placement: ActionBarPlacement.OnBar

            // If there are 100 items in the list, we're at the max number of items, we cannot add more items.
            enabled: ! bucketModel.bucketIsFull
            onTriggered: {
                addNew.open();
                addNew.text = "";
            }
        },
        InvokeActionItem {
            title: "Start BBM Chat"
            query {
                mimeType: "text/plain"
                invokeTargetId: "sys.bbm.sharehandler"
                invokeActionId: "bb.action.SHARE"
            }
        }
    ]
    onCreationCompleted: {
        // Connect the list signal to this page signal to reemit it when it is triggered
        bucketList.newBBMStatus.connect(newBBMStatus);
        
        _theApp.incomingBucketItem.connect(onIncomingBucketItem);
        
    }
}
