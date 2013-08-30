
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
        title: qsTr("After A11y: Component A11y Specialization") + Retranslate.onLocaleOrLanguageChanged
    }

    Container {
        layout: DockLayout {
        }

        Container {
            id: con
            leftPadding: 50
            rightPadding: 50
            horizontalAlignment: HorizontalAlignment.Center
            verticalAlignment: VerticalAlignment.Center
            layout: StackLayout {
            }

            gestureHandlers: [
                TapHandler {
                    onTapped: {
                        con.doWork();
                    }

                }
            ]

            accessibility: CustomA11yObject {
                role: A11yRole.Button
                name: tapme.text

                ComponentA11ySpecialization {
                    onActivationRequested: {
                        if (event.type == A11yComponentActivationType.Tap) {
                            event.consume();
                            con.doWork();
                        }
                    }
                }
            }

            function doWork() {
                response.visible = true;
            }

            Label {
                id: tapme
                text: qsTr("Tap me please!") + Retranslate.onLocaleOrLanguageChanged
                textStyle.fontSize: FontSize.XXLarge
                accessibilityMode: A11yMode.Collapsed
            }
            Label {
                id: response
                text: qsTr("You tapped it!") + Retranslate.onLocaleOrLanguageChanged
                textStyle.fontSize: FontSize.XXLarge
                visible: false
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
