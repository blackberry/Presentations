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
import utils 1.0

ListView {
    id: weatherList
    property alias pullToRefresh: pullToRefresh
    signal dataRequest(variant item, bool requestOlderItems);

    // An XML model can be used for quickly prototyping the list.
    dataModel: XmlDataModel {
        source: "../models/weather.xml"
    }

    leadingVisual: PullToRefresh {
        id: pullToRefresh
        control: weatherList

        onRefreshActiveChanged: {
            if (active) {
                var item = weatherList.dataModel.data([ 0 ]);
                dataRequest(item, false);
            }
        }
    }

    listItemComponents: [
        ListItemComponent {
            type: "item"
            WeatherItem {
            }
        },
        ListItemComponent {
            type: "last_item"
            InlineActivityIndicator {
                id: recipeIndicator
                indicatorText: qsTr("Loading items") + Retranslate.onLanguageChanged
                indicatorRunning: ListItemData.loading
                indicatorVisble: true
            }
        }
    ]

    attachedObjects: [
        ListScrollStateHandler {
            onAtEndChanged: {
                if (atEnd && firstVisibleItem.length != 0 && loadModelDecorator.loadingOldItems == false) {
                    var childCount = weatherList.dataModel.childCount(weatherList.rootIndexPath);
                    if (childCount > 0) {
                        var item = weatherList.dataModel.data([ childCount - 2 ]);
                        dataRequest(item, true);
                    }
                }
            }
        }
    ]
}
