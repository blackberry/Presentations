/* Copyright (c) 2013 Research In Motion Limited.
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
    id: highScoreItem
    leftPadding: 50
    rightPadding: 50

    Container {
        layout: StackLayout {
            orientation: LayoutOrientation.LeftToRight
        }
        Label {
            text: ListItemData.position
            //altenative: text: highScoreItem.ListItem.indexPath[0] + 1
            textStyle.base: SystemDefaults.TextStyles.TitleText
            textStyle.color: Color.create("#fafafa")
            layoutProperties: StackLayoutProperties {
                spaceQuota: 2
            }
        }
        Label {
            text: ListItemData.name
            textStyle.base: SystemDefaults.TextStyles.TitleText
            textStyle.color: Color.create("#fafafa")
            layoutProperties: StackLayoutProperties {
                spaceQuota: 4
            }
        }
        Label {
            text: ListItemData.moves
            textStyle.color: Color.create("#fafafa")
            layoutProperties: StackLayoutProperties {
                spaceQuota: 2
            }
        }
        Label {
            text: ListItemData.time
            textStyle.color: Color.create("#fafafa")
            layoutProperties: StackLayoutProperties {
                spaceQuota: 2
            }
        }
    }

    Divider {
    }
}
