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
    property alias title : controlLabel.text
    property alias text: positionLabel.text
    
    layout: StackLayout {
        orientation: LayoutOrientation.LeftToRight
    }
    leftPadding: 10
    Label {
        id: controlLabel
        layoutProperties: StackLayoutProperties {
            spaceQuota: 1
        }
        text: ""
        textStyle.color: Color.White
        textStyle.base: SystemDefaults.TextStyles.BodyText
    }
    Label {
        id:  positionLabel
        layoutProperties: StackLayoutProperties {
            spaceQuota: 1
        }
        text: ""
        textStyle.color: Color.White
        textStyle.base: SystemDefaults.TextStyles.BodyText
    }
}
