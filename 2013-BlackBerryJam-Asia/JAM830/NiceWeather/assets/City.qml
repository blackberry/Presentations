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

CityContainer {
    property alias modelUrl: dm.source
    property alias bg: bg.imageSource
    property alias bg2: bg2.imageSource

    layout: DockLayout {}
    
    // background image goes here
    ScrollView {
        id: backgroundView
        Container {
            layout: DockLayout {
            }
            ImageView {
                id: bg
            }
            ImageView {
                id: bg2
                opacity: 0
            }
        }
    }

    ListView {
        id: cityList
        // Lists tries to expand as much as possible and since it's located inside another listView it will get unlimited widht. 
        maxWidth: listWidth
        maxHeight: listHeight

        dataModel: XmlDataModel {
            id: dm
        }
        snapMode: SnapMode.LeadingEdge
        property bool draggingStarted: false
        scrollIndicatorMode: ScrollIndicatorMode.None

        attachedObjects: ListScrollStateHandler {
            onScrollingChanged: {
                if (scrolling) {
                    cityList.draggingStarted = true
                    bg2.opacity = 1
                } else if (atBeginning) {
                    cityList.draggingStarted = false
                    bg2.opacity = 0
                }
            }
        }

		// list offset. Currently set from HeaderItem.qml. Ideally something similar to visibleArea would be nice instead.
        property int offset
        onOffsetChanged: {
            // paralax-scrolling the background based on offset.
            backgroundView.scrollToPoint(0, - offset / 3, ScrollAnimation.None);
        }

        listItemComponents: [
            ListItemComponent {
                type: "item"
                ForeCastItem {
                }
            },
            ListItemComponent {
                type: "header"
                HeaderItem {
                }
            }
        ]
    }

    function showAnim() {
        show.play();
    }
    function hideAnim() {
        show.stop();
        hide.play();
        cityList.scrollToPosition(ScrollPosition.Beginning,ScrollAnimation.Smooth);
    }
    
    animations: [
        ParallelAnimation {
            id: show
            target: cityList
            FadeTransition {
                fromOpacity: 0
                toOpacity: 1
                duration: 500
                easingCurve: StockCurve.CubicOut
            }
            TranslateTransition {
                fromY: 300
                toY: 0
                duration: 500
                easingCurve: StockCurve.CubicOut
            }
        },
        ParallelAnimation {
            id: hide
            target: cityList
            FadeTransition {
                toOpacity: 0
                duration: 100
                easingCurve: StockCurve.CubicIn
            }
            TranslateTransition {
                fromY: 0
                toY: 300
                duration: 500
                easingCurve: StockCurve.CubicIn
            }
        }
    ]
}
