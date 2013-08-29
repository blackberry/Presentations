
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
import bb.system 1.0

Page {
    titleBar: TitleBar {
        title: qsTr("After A11y: Incorporate redundant means") + Retranslate.onLocaleOrLanguageChanged
    }
    Container {
        layout: DockLayout {
        }

        Container {
            verticalAlignment: VerticalAlignment.Center
            horizontalAlignment: HorizontalAlignment.Center
            layout: StackLayout {
                orientation: LayoutOrientation.TopToBottom
            }
            Label {
                text: qsTr("How would you like your tea?") + Retranslate.onLocaleOrLanguageChanged
            }
            Container {
                horizontalAlignment: HorizontalAlignment.Center
                layout: StackLayout {
                    orientation: LayoutOrientation.LeftToRight
                }
                Button {
                    imageSource: "asset:///cold.png"
                    text: qsTr("Iced") + Retranslate.onLocaleOrLanguageChanged
                    preferredWidth: 80
                    attachedObjects: [
                        SystemToast {
                            id: icedtea
                            body: qsTr("Your iced tea will be ready shortly!") + Retranslate.onLocaleOrLanguageChanged
                        }
                    ]
                    onClicked: {
                        icedtea.show()
                    }

                }
                Button {
                    imageSource: "asset:///hot.png"
                    text: qsTr("Hot") + Retranslate.onLocaleOrLanguageChanged
                    preferredWidth: 80
                    attachedObjects: [
                        SystemToast {
                            id: hottea
                            body: qsTr("Your hot tea will be ready shortly!") + Retranslate.onLocaleOrLanguageChanged
                        }
                    ]
                    onClicked: {
                        hottea.show()
                    }
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
