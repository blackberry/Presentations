
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
        // Localized text with the dynamic translation and locale updates support
        title: qsTr("Before A11y: Image Information Only") + Retranslate.onLocaleOrLanguageChanged
    }

    Container {
        layout: DockLayout {
        }
        Container {
            horizontalAlignment: HorizontalAlignment.Center
            verticalAlignment: VerticalAlignment.Center
            layout: StackLayout {
                orientation: LayoutOrientation.TopToBottom
            }
            Label {
                textStyle.base: SystemDefaults.TextStyles.BigText
                text: qsTr("You're almost there!") + Retranslate.onLocaleOrLanguageChanged
                horizontalAlignment: HorizontalAlignment.Center
                multiline: true
            }
            Container {
                horizontalAlignment: HorizontalAlignment.Center
                layout: StackLayout {
                    orientation: LayoutOrientation.LeftToRight
                }
                Label {
                    textStyle.base: SystemDefaults.TextStyles.BigText
                    text: qsTr("Turn ") + Retranslate.onLocaleOrLanguageChanged
                    verticalAlignment: VerticalAlignment.Center
                }
                ImageView {
                    //BAD: screen reader can't figure out what to tell the user.
                    imageSource: "asset:///left.png"
                    verticalAlignment: VerticalAlignment.Center
                }
            }
        }
    }

    actions: ActionItem {
        title: qsTr("After A11y") + Retranslate.onLocaleOrLanguageChanged
        ActionBar.placement: ActionBarPlacement.OnBar

        onTriggered: {
            // A second Page is created and pushed when this action is triggered.
            navigationPane.push(fixPageDefinition.createObject());
        }
    }

    attachedObjects: [
        // Definition of the second Page, used to dynamically create the Page above.
        ComponentDefinition {
            id: fixPageDefinition
            source: "ImageOnly_After.qml"
        }
    ]
}
