/*
 * Copyright (c) 2011, 2012 Research In Motion Limited.
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

Container {
    id: listItemRoot
    property variant textColor: ListItem.active ? Color.Gray : Color.White
    Container {
        id: itemRoot
        layout: StackLayout {
            orientation: LayoutOrientation.LeftToRight
        }
        Label {
            verticalAlignment: VerticalAlignment.Center
            layoutProperties: StackLayoutProperties {
                spaceQuota: 15
            }
            textStyle.color: textColor
            textStyle.base: SystemDefaults.TextStyles.TitleText
            text: listItemRoot.ListItem.indexInSection + 1 + "."
        }
        Label {
            verticalAlignment: VerticalAlignment.Center
            layoutProperties: StackLayoutProperties {
                spaceQuota: 55
            }
            textStyle.color: textColor
            textStyle.base: SystemDefaults.TextStyles.TitleText
            text: ListItemData.name
        }
        Label {
            verticalAlignment: VerticalAlignment.Center
            layoutProperties: StackLayoutProperties {
                spaceQuota: 15
            }
            textStyle.color: textColor
            textStyle.textAlign: TextAlign.Left
            textStyle.base: SystemDefaults.TextStyles.BodyText
            text: ListItemData.moves
        }
        Label {
            verticalAlignment: VerticalAlignment.Center
            layoutProperties: StackLayoutProperties {
                spaceQuota: 20
            }
            textStyle.color: textColor
            textStyle.base: SystemDefaults.TextStyles.BodyText
            textStyle.textAlign: TextAlign.Right
            text: millisecondsToString (ListItemData.time)
        }
    }
    Divider {
    }
    function millisecondsToString (time) {
        var tenthsOfSecond = Math.floor (((time % 1000) / 100));
        var seconds = Math.floor (((time / 1000) % 60));
        var minutes = Math.floor ((time / 60000));
        return minutes + ":" + seconds + "." + tenthsOfSecond;
    }
}
