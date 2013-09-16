
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
        title: qsTr("Before A11y: Finger Instructions") + Retranslate.onLocaleOrLanguageChanged
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
                //BAD: instructions to tap.  When screenreader is on these instructions will not work.
                text: qsTr("<html>You won a million dollars! Please <b>tap the Continue Button</b> with your finger to learn how you can receive your money.</html>") + Retranslate.onLocaleOrLanguageChanged
                multiline: true
            }
            Button {
                text: qsTr("Continue") + Retranslate.onLocaleOrLanguageChanged
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

    actions: ActionItem {
        title: qsTr("After A11y") + Retranslate.onLocaleOrLanguageChanged
        ActionBar.placement: ActionBarPlacement.OnBar

        onTriggered: {
            navigationPane.push(fixPageDefinition.createObject());
        }
    }

    attachedObjects: [
        // Definition of the second Page, used to dynamically create the Page above.
        ComponentDefinition {
            id: fixPageDefinition
            source: "FingerInst_After.qml"
        }
    ]

}
