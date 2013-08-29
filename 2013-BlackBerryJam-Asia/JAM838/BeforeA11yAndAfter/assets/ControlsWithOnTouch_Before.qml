
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

import bb.cascades 1.2

Page {
    titleBar: TitleBar {
        title: qsTr("Before A11y: Controls with OnTouch") + Retranslate.onLocaleOrLanguageChanged
    }
    Container {
        layout: DockLayout {
        }

        Container {
            leftPadding: 50
            rightPadding: 50
            horizontalAlignment: HorizontalAlignment.Center
            verticalAlignment: VerticalAlignment.Center
            layout: StackLayout {
            }
            Label {
                text: qsTr("Touch me please!") + Retranslate.onLocaleOrLanguageChanged
                textStyle.fontSize: FontSize.XXLarge
                //BAD: this won't get called when screen reader is on
                onTouch: {
                    response.visible = true;
                }
            }
            Label {
                id: response
                text: qsTr("You touched it!") + Retranslate.onLocaleOrLanguageChanged
                textStyle.fontSize: FontSize.XXLarge
                visible: false
            }
        }
    }

    actions: ActionItem {
        title: qsTr("After A11y") + Retranslate.onLocaleOrLanguageChanged
        ActionBar.placement: ActionBarPlacement.OnBar

        onTriggered: {
            navigationPane.push(fixPageDefinition.createObject());
        }
    }

    attachedObjects: [
        ComponentDefinition {
            id: fixPageDefinition
            source: "ControlsWithOnTouch_After.qml"
        }
    ]

}
