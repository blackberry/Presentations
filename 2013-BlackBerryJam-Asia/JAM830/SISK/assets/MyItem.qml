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

Container {
    id: itemRoot
    background: Color.create(ListItemData.color)
    minHeight: 100
    leftPadding: 36
    rightPadding: leftPadding
    property bool peek: itemRoot.ListItem.view.secretPeek
    layout: DockLayout {
    }

    onPeekChanged: {
        if (peek) {
            peekAnimation.play()
        }
    }

    animations: [
        //Show animation
        ParallelAnimation {
            id: showAnimation
            animations: [
                FadeTransition {
                    fromOpacity: 0.0
                    toOpacity: 1.0
                    duration: 400
                    delay: itemRoot.ListItem.indexPath * 40
                    easingCurve: StockCurve.ExponentialOut
                },
                TranslateTransition {
                    fromY: (itemRoot.ListItem.indexPath * 50.0) + 50.0
                    toY: 0.0
                    duration: 500
                    easingCurve: StockCurve.ExponentialOut
                }
            ]
        },
        // Peek animation
        SequentialAnimation {
            id: peekAnimation
            animations: [
                ParallelAnimation {

                    FadeTransition {
                        toOpacity: 0.0
                        duration: 0
                    }

                    TranslateTransition {
                        toX: - (itemRoot.ListItem.indexPath * 50.0) - 50.0
                        duration: 0
                    }
                },
                ParallelAnimation {
                    FadeTransition {
                        fromOpacity: 0.0
                        toOpacity: 1.0
                        duration: 400
                        delay: itemRoot.ListItem.indexPath * 40
                        easingCurve: StockCurve.ExponentialOut
                    }
                    TranslateTransition {
                        fromX: - (itemRoot.ListItem.indexPath * 50.0) - 50.0
                        toX: 0.0
                        duration: 500
                        easingCurve: StockCurve.ExponentialOut
                    }
                }
            ]
        }

    ]
    onCreationCompleted: {
        showAnimation.play()
    }

    verticalAlignment: VerticalAlignment.Center
    horizontalAlignment: HorizontalAlignment.Center
    Divider {
    }
}
