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
    topPadding: ListItem.view.maxHeight - contentContainer.preferredHeight//904

    Container {
        id: contentContainer
        background: Color.create("#66000000")
        leftPadding: 30
        rightPadding: 30
        bottomPadding: 30
        topPadding: 30
        horizontalAlignment: HorizontalAlignment.Fill
        preferredWidth: 768
        preferredHeight: 376
        layout: DockLayout {
        }

        Container {
            verticalAlignment: VerticalAlignment.Center
            Label {
                text: ListItemData.title
                textStyle.base: header.style
                bottomMargin: 0
            }

            Container {
                topMargin: 0
                layout: StackLayout {
                    orientation: LayoutOrientation.LeftToRight
                }
                Label {
                    text: ListItemData.day
                    textStyle.base: header.style
                }
                Container {
                    verticalAlignment: VerticalAlignment.Center
                    layout: DockLayout {
                    }
                    //                verticalAlignment: VerticalAlignment.Fill
                    ImageView {
                        verticalAlignment: VerticalAlignment.Center
                        imageSource: "asset:///images/icons/large/" + ListItemData.icon
                    }
                }
            }

        }
    }
    attachedObjects: [
        // attaching textStyleDefinitions in each item might not be the most optimal solution but with few item list like this it shouldn't affect much.
        TextStyleDefinition {
            id: header
            fontFamily: "sans-serif"
            fontSize: FontSize.PointValue
            fontSizeValue: 30
        },
        LayoutUpdateHandler {
            onLayoutFrameChanged: {
                if (root.ListItem.indexPath == 0) {
                    // set the property offset residing in ListView based on this items y position.
                	   root.ListItem.view.offset = layoutFrame.y;
                }
            }
        }
    ]
}