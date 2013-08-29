
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
        title: qsTr("Before A11y: Hardcoded Font Size") + Retranslate.onLocaleOrLanguageChanged
    }

    Container {
        layout: DockLayout {

        }

        Container {
            verticalAlignment: VerticalAlignment.Center
            horizontalAlignment: HorizontalAlignment.Center

            Label {
                text: qsTr("5 point") + Retranslate.onLocaleOrLanguageChanged
                //BAD: useage of  FontSize.PointValue; text won't change if system font size is increased
                textStyle.fontSize: FontSize.PointValue
                textStyle.fontSizeValue: 5
                horizontalAlignment: HorizontalAlignment.Center

            }
            Label {
                text: qsTr("6 point") + Retranslate.onLocaleOrLanguageChanged
                //BAD: useage of  FontSize.PointValue; text won't change if system font size is increased
                textStyle.fontSize: FontSize.PointValue
                textStyle.fontSizeValue: 6
                horizontalAlignment: HorizontalAlignment.Center
            }
            Label {
                text: qsTr("7 point") + Retranslate.onLocaleOrLanguageChanged
                //BAD: useage of  FontSize.PointValue; text won't change if system font size is increased
                textStyle.fontSize: FontSize.PointValue
                textStyle.fontSizeValue: 7
                horizontalAlignment: HorizontalAlignment.Center
            }
            Label {
                text: qsTr("8 point") + Retranslate.onLocaleOrLanguageChanged
                //BAD: useage of  FontSize.PointValue; text won't change if system font size is increased
                textStyle.fontSize: FontSize.PointValue
                textStyle.fontSizeValue: 8
                horizontalAlignment: HorizontalAlignment.Center
            }
            Label {
                text: qsTr("9 point") + Retranslate.onLocaleOrLanguageChanged
                //BAD: useage of  FontSize.PointValue; text won't change if system font size is increased
                textStyle.fontSize: FontSize.PointValue
                textStyle.fontSizeValue: 9
                horizontalAlignment: HorizontalAlignment.Center
            }
            Label {
                text: qsTr("12 point") + Retranslate.onLocaleOrLanguageChanged
                //BAD: useage of  FontSize.PointValue; text won't change if system font size is increased
                textStyle.fontSize: FontSize.PointValue
                textStyle.fontSizeValue: 12
                horizontalAlignment: HorizontalAlignment.Center
            }
            Label {
                text: qsTr("16 point") + Retranslate.onLocaleOrLanguageChanged
                //BAD: useage of  FontSize.PointValue; text won't change if system font size is increased
                textStyle.fontSize: FontSize.PointValue
                textStyle.fontSizeValue: 16
                horizontalAlignment: HorizontalAlignment.Center
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
            source: "FontSize_After.qml"
        }
    ]

}
