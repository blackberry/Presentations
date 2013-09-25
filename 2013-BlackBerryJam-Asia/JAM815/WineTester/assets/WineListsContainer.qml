/*
 * Copyright 2013 BlackBerry Limited.
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
import QtQuick 1.0
import CustomTimer 1.0

/*
 * This container contains both wine listviews, animations and toggle functions.
 */
Container {
    Container {
        horizontalAlignment: HorizontalAlignment.Fill
        verticalAlignment: VerticalAlignment.Fill
        layout: DockLayout {}
        
        //White wines listview
        ListView {
            id: whiteWinesListView
            objectName: "whiteWinesListView"
            visible: false
            dataModel: _homeState.whiteWinesDataModel
            scrollRole: ScrollRole.Main
            listItemComponents: [                        
                WineListItem {
                }
            ]
            animations: [
                FadeTransition {
                    id: listViewWhiteFadeIn
                    duration: 750
                    easingCurve: StockCurve.CubicOut
                    fromOpacity: 0.0
                    toOpacity: 1.0
                    onStarted: whiteWinesListView.visible = true
                },
                FadeTransition {
                    id: listViewWhiteFadeOut
                    duration: 750
                    easingCurve: StockCurve.CubicOut
                    fromOpacity: 1.0
                    toOpacity: 0.0
                    onEnded: whiteWinesListView.visible = false
                }
            ]
        }
        //White wines listview
        ListView {
            id: redWinesListView
            objectName: "redWinesListView"
            visible: false
            dataModel: _homeState.redWinesDataModel
            scrollRole: ScrollRole.Main
            listItemComponents: [
                WineListItem {
                }
            ]
            animations: [
                FadeTransition {
                    id: listViewRedFadeIn
                    duration: 750
                    easingCurve: StockCurve.CubicOut
                    fromOpacity: 0.0
                    toOpacity: 1.0
                    onStarted: redWinesListView.visible = true
                },
                FadeTransition {
                    id: listViewRedFadeOut
                    duration: 750
                    easingCurve: StockCurve.CubicOut
                    fromOpacity: 1.0
                    toOpacity: 0.0
                    onEnded: redWinesListView.visible = false
                }
            ]
        }
    }
    function toggleRedWinesList() {
        listViewRedFadeIn.play()
        listViewWhiteFadeOut.play()
    }
    function toggleWhiteWinesList() {
        listViewRedFadeOut.play()
        listViewWhiteFadeIn.play()
    }
    onCreationCompleted: {
        toggleRedWinesList()
    }
}
