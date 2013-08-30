
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
        title: qsTr("After A11y: Provide labelledBy Relation") + Retranslate.onLocaleOrLanguageChanged
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
                    id: b
                    text: qsTr("Box") + Retranslate.onLocaleOrLanguageChanged
                    horizontalAlignment: HorizontalAlignment.Center
                }

                Label {
                    id: w
                    text: qsTr("Width") + Retranslate.onLocaleOrLanguageChanged
                }
                Slider {
                    horizontalAlignment: HorizontalAlignment.Center
                    fromValue: 0
                    toValue: 100
                    value: 50
                    accessibility {
                        labelledBy: [ b, w ]
                    }
                }
                Label {
                    id: h
                    text: qsTr("Height") + Retranslate.onLocaleOrLanguageChanged
                }
                Slider {
                    horizontalAlignment: HorizontalAlignment.Center
                    fromValue: 0
                    toValue: 100
                    value: 50
                    accessibility {
                        labelledBy: [ b, h ]
                    }
                }
                Divider {
                }
                Label {
                    id: c
                    text: qsTr("Circle Radius") + Retranslate.onLocaleOrLanguageChanged
                }
                Slider {
                    horizontalAlignment: HorizontalAlignment.Center
                    fromValue: 0
                    toValue: 100
                    value: 50
                    accessibility {
                        labelledBy: c
                    }
                }
                Divider {
                }
                Container {
                    horizontalAlignment: HorizontalAlignment.Fill
                    layout: DockLayout {
                    }
                    Label {
                        id: a_label
                        horizontalAlignment: HorizontalAlignment.Left
                        verticalAlignment: VerticalAlignment.Center
                        text: qsTr("Animated") + Retranslate.onLocaleOrLanguageChanged
                        accessibilityMode: A11yMode.Collapsed
                    }
                    CheckBox {
                        id: animated
                        horizontalAlignment: HorizontalAlignment.Right
                        accessibilityMode: A11yMode.Collapsed
                    }
                    accessibility: CustomA11yObject {
                        role: A11yRole.CheckBox
                        name: a_label.text
                        state {
                            checkable: true
                            checked: animated.checked
                        }

                        ComponentA11ySpecialization {
                            onActivationRequested: {
                                if (event.type == A11yComponentActivationType.Tap) {
                                    animated.checked = ! (animated.checked)
                                }
                            }
                        }
                    }

                }
                Divider {
                }

            }
        }
    }

    paneProperties: NavigationPaneProperties {
        backButton: ActionItem {
            title: qsTr("Introduction") + Retranslate.onLocaleOrLanguageChanged
            onTriggered: {
                navigationPane.pop();
                navigationPane.pop();
            }
            accessibility.name: qsTr("Back to the introduction") + Retranslate.onLocaleOrLanguageChanged
        }
    }
}
