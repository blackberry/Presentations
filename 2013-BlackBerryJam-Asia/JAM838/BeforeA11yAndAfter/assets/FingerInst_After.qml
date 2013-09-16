
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
        title: qsTr("After A11y: Abstract Instructions") + Retranslate.onLocaleOrLanguageChanged
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
                text: qsTr("Congratulations!") + Retranslate.onLocaleOrLanguageChanged
                textStyle.fontSize: FontSize.XXLarge
            }
            Label {
                text: qsTr("<html>You won a million dollars! </html>") + Retranslate.onLocaleOrLanguageChanged
                multiline: true
            }
            Button {
                text: qsTr("Get the Money!") + Retranslate.onLocaleOrLanguageChanged
                attachedObjects: [
                    SystemToast {
                        id: milliondollars
                        body: "Look behind you. There it is!"
                    }
                ]
                onClicked: {
                    milliondollars.show()
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
