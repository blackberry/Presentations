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
    id: root
    topPadding: 2

	// when user starts scrolling the list start the item animation
    property bool draggingStarted: ListItem.view.draggingStarted
    onDraggingStartedChanged: {
        if (draggingStarted) {
            showAnim.play();
        } else {
            showAnim.stop();
            hideAnim.play();
        }
    }

    scaleX: 0.8
    scaleY: 0.8
    opacity: 0
    animations: [
        ParallelAnimation {
            id: showAnim
            ScaleTransition {
                fromX: 0.8
                toX: 1
                fromY: 0.8
                toY: 1
                duration: 800
                easingCurve: StockCurve.ElasticOut
            }
            FadeTransition {
                fromOpacity: 0
                toOpacity: 1
                duration: 200
            }
            // delayed animation based on index. Note, if a  hierarchical model is used this needs to be taken into account here. 
            delay: root.ListItem.indexInSection * 100
        },
        FadeTransition {
            id: hideAnim
            toOpacity: 0
            duration: 0
        }
    ]

    Container {
        id: innerContainer
        layout: StackLayout {
            orientation: LayoutOrientation.LeftToRight
        }
        background: Color.create("#66000000")
        leftPadding: 25
        rightPadding: 25
        topPadding: 35
        bottomPadding: 19
        horizontalAlignment: HorizontalAlignment.Fill

        Label {
            text: ListItemData.date
            textStyle.base: subHeader.style
            layoutProperties: StackLayoutProperties {
                spaceQuota: 3
            }
        }
        Container {
            layoutProperties: StackLayoutProperties {
                spaceQuota: 1
            }
            ImageView {
                imageSource: "asset:///images/icons/small/" + ListItemData.icon
                horizontalAlignment: HorizontalAlignment.Center
            }
        }
        Label {
            text: ListItemData.day
            textStyle.textAlign: TextAlign.Right
            textStyle.base: subHeader.style
            layoutProperties: StackLayoutProperties {
                spaceQuota: 1
            }
        }
        Label {
            text: ListItemData.night
            textStyle.textAlign: TextAlign.Right
            textStyle.base: subHeader.style
            layoutProperties: StackLayoutProperties {
                spaceQuota: 1
            }
        }

    }

	// attaching textStyleDefinitions in each item might not be the most optimal solution but with few item list like this it shouldn't affect much.
    attachedObjects: TextStyleDefinition {
        id: subHeader
        fontFamily: "sans-serif"
        fontSize: FontSize.XLarge
    }

}
