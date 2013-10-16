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

MyItem {
    minHeight: 120
    background: Color.create(ListItemData.color)
    Container {
        
        layout: StackLayout {
            orientation: LayoutOrientation.LeftToRight
        }
        Container {
            verticalAlignment: VerticalAlignment.Center
            minWidth: 60
            ImageView {
                imageSource: ListItemData.itemImage
            }
        }
        Label {
            leftMargin: 36
            text: ListItemData.title
            textStyle.fontStyle: FontStyle.Default
            textStyle.fontSize: FontSize.PointValue
            textStyle.fontWeight: FontWeight.Default
            textStyle.fontSizeValue: 14.0
        }
        verticalAlignment: VerticalAlignment.Center
        horizontalAlignment: HorizontalAlignment.Left
    
    }
}
