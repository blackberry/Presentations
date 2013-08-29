
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
        title: qsTr("Before A11y: No Labeling Relationships") + Retranslate.onLocaleOrLanguageChanged
    }
    ScrollView {
        Container {
            leftPadding: 50
            rightPadding: 50
            horizontalAlignment: HorizontalAlignment.Center
            layout: StackLayout {
            }
            Container {
                Label {
                    text: qsTr("Lets define a box and a circle!") + Retranslate.onLocaleOrLanguageChanged
                    textStyle.base: SystemDefaults.TextStyles.BigText
                    multiline: true
                }
                Divider {

                }
                Label {
                    text: qsTr("Box") + Retranslate.onLocaleOrLanguageChanged
                    horizontalAlignment: HorizontalAlignment.Center
                }

                Label {
                    text: qsTr("Width") + Retranslate.onLocaleOrLanguageChanged
                }
                Slider {
                    //BAD: screen reader does not know what this slider refers to
                    horizontalAlignment: HorizontalAlignment.Center
                    fromValue: 0
                    toValue: 100
                    value: 50
                }
                Label {
                    text: qsTr("Height") + Retranslate.onLocaleOrLanguageChanged
                }
                Slider {
                    //BAD: screen reader does not know what this slider refers to
                    horizontalAlignment: HorizontalAlignment.Center
                    fromValue: 0
                    toValue: 100
                    value: 50
                }
                Divider {
                }
                Label {
                    text: qsTr("Circle Radius") + Retranslate.onLocaleOrLanguageChanged
                }
                Slider {
                    //BAD: screen reader does not know what this slider refers to
                    horizontalAlignment: HorizontalAlignment.Center
                    fromValue: 0
                    toValue: 100
                    value: 50
                }
                Divider {
                }
                Container {
                    horizontalAlignment: HorizontalAlignment.Fill
                    layout: DockLayout {
                    }
                    Label {
                        horizontalAlignment: HorizontalAlignment.Left
                        verticalAlignment: VerticalAlignment.Center
                        text: qsTr("Animated") + Retranslate.onLocaleOrLanguageChanged
                    }
                    CheckBox {
                        horizontalAlignment: HorizontalAlignment.Right
                    }
                }
                Divider {
                }

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
            source: "LabellingRel_After.qml"
        }
    ]
}
