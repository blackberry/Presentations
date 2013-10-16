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

CustomListItem {
    property alias title: itemLabel.text
    property alias bandName: bandLabel.text
    property int rating: 4
    property int colorIndex: 1
    property variant colorCode: [ "#63a69f", "#f2e1ac", "#f2836b", "#f2594b", "#cd2c24", "#eaa873", "#e88855", "c35d2e" ]
    property variant textColor: Color.create("#373737")

    dividerVisible: false
    minHeight: 256

    Container {
        horizontalAlignment: HorizontalAlignment.Fill
        verticalAlignment: VerticalAlignment.Fill
        topPadding: 17
        leftPadding: 36
        bottomPadding: leftPadding
        rightPadding: leftPadding
        background: Color.create(colorCode[colorIndex % (colorCode.length - 1)])

        layout: DockLayout {
        }

        Container {
            Label {
                id: itemLabel
                text: "Song title"
                verticalAlignment: VerticalAlignment.Center
                textStyle.fontSize: FontSize.PointValue
                textStyle.fontSizeValue: 16
                textStyle.color: textColor
                bottomMargin: 0
            }

            Label {
                id: bandLabel
                text: "Band Name"
                verticalAlignment: VerticalAlignment.Center
                textStyle.fontSize: FontSize.PointValue
                textStyle.fontSizeValue: 12
                textStyle.fontWeight: FontWeight.W300
                textStyle.color: textColor
                topMargin: 0                
            }

        }

        Container {
            verticalAlignment: VerticalAlignment.Bottom
            layout: StackLayout {
                orientation: LayoutOrientation.LeftToRight
            }
            ImageView {
                imageSource: (rating >= 1) ? "asset:///images/heart-filled.png" : "asset:///images/heart-outlined.png"
            }
            ImageView {
                imageSource: (rating >= 2) ? "asset:///images/heart-filled.png" : "asset:///images/heart-outlined.png"
            }
            ImageView {
                imageSource: (rating >= 3) ? "asset:///images/heart-filled.png" : "asset:///images/heart-outlined.png"
            }
            ImageView {
                imageSource: (rating >= 4) ? "asset:///images/heart-filled.png" : "asset:///images/heart-outlined.png"
            }
            ImageView {
                imageSource: (rating >= 5) ? "asset:///images/heart-filled.png" : "asset:///images/heart-outlined.png"
            }
        }
    }
}