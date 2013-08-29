
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

NavigationPane {
    id: navigationPane

    Page {
        titleBar: TitleBar {
            // Localized text with the dynamic translation and locale updates support
            title: qsTr("Before and After A11y") + Retranslate.onLocaleOrLanguageChanged
        }

        Container {
            layout: DockLayout {
            }

            Label {
                horizontalAlignment: HorizontalAlignment.Center
                verticalAlignment: VerticalAlignment.Center
                textStyle.fontSize: FontSize.Large

                text: qsTr("Have a look at the action menu to see things that were commonly done without consideration for A11y.  Each page also has solutions to properly address A11y.") + Retranslate.onLocaleOrLanguageChanged
                multiline: true
            }
        }

        actions: [
            ActionItem {
                title: qsTr("Hardcoded Font Size") + Retranslate.onLocaleOrLanguageChanged
                ActionBar.placement: ActionBarPlacement.InOverflow

                onTriggered: {
                    navigationPane.push(fontsizePageDefinition.createObject());
                }
            },
            ActionItem {
                //"Please tap the Confirm or Cancel Buttons"
                title: qsTr("Finger Instructions") + Retranslate.onLocaleOrLanguageChanged
                ActionBar.placement: ActionBarPlacement.InOverflow
                
                onTriggered: {
                    navigationPane.push(fingerInstructionsPageDefinition.createObject());
                }
            },
            ActionItem {
                //shower...hot or cold?
                title: qsTr("Color Information Only") + Retranslate.onLocaleOrLanguageChanged
                ActionBar.placement: ActionBarPlacement.InOverflow

                onTriggered: {
                    navigationPane.push(colorOnlyPageDefinition.createObject());
                }
            },
            ActionItem {
                //turn...left or right?
                title: qsTr("Image Information Only") + Retranslate.onLocaleOrLanguageChanged
                ActionBar.placement: ActionBarPlacement.InOverflow

                onTriggered: {
                    navigationPane.push(imageOnlyPageDefinition.createObject());
                }
            },
            ActionItem {
                //login and password in a weird layout for alignment.
                title: qsTr("No Labeling Relationships") + Retranslate.onLocaleOrLanguageChanged
                ActionBar.placement: ActionBarPlacement.InOverflow

                onTriggered: {
                    navigationPane.push(labellingRelationPageDefinition.createObject());
                }
            },
            ActionItem {
                //Controls with onTouch
                title: qsTr("Controls with onTouch") + Retranslate.onLocaleOrLanguageChanged
                ActionBar.placement: ActionBarPlacement.InOverflow

                onTriggered: {
                    navigationPane.push(controlsWithOnTouchPageDefinition.createObject());
                }
            },
            ActionItem {
                //list items that do nothing when you touch the wrong area.
                title: qsTr("No Custom A11y Object") + Retranslate.onLocaleOrLanguageChanged
                ActionBar.placement: ActionBarPlacement.InOverflow

                onTriggered: {
                    navigationPane.push(customControlPageDefinition.createObject());
                }
            },
            ActionItem {
                //box that does nothing
                title: qsTr("Custom Control in C++") + Retranslate.onLocaleOrLanguageChanged
                ActionBar.placement: ActionBarPlacement.InOverflow

                onTriggered: {
                    // A second Page is created and pushed when this action is triggered.
                    navigationPane.push(customControlCPlusPlusPageDefinition.createObject());
                }
            },
            ActionItem {
                // "you have too many @s in your email address"
                title: qsTr("Referencing Punctuation") + Retranslate.onLocaleOrLanguageChanged
                ActionBar.placement: ActionBarPlacement.InOverflow

                onTriggered: {
                    // A second Page is created and pushed when this action is triggered.
                    navigationPane.push(punctuationReferencePageDefinition.createObject());
                }
            },
            ActionItem {
                // "RIM" vs "Research In Motion"
                title: qsTr("Acronyms") + Retranslate.onLocaleOrLanguageChanged
                ActionBar.placement: ActionBarPlacement.InOverflow

                onTriggered: {
                    // A second Page is created and pushed when this action is triggered.
                    navigationPane.push(acronymsPageDefinition.createObject());
                }
            }
        ]

    }

    attachedObjects: [
        // Definition of the second Page, used to dynamically create the Page above.
        ComponentDefinition {
            id: fontsizePageDefinition
            source: "FontSize_Intro.qml"
        },
        ComponentDefinition {
            id: fingerInstructionsPageDefinition
            source: "FingerInst_Intro.qml"
        },
        ComponentDefinition {
            id: colorOnlyPageDefinition
            source: "ColorOnly_Intro.qml"
        },
        ComponentDefinition {
            id: imageOnlyPageDefinition
            source: "ImageOnly_Intro.qml"
        },
        ComponentDefinition {
            id: labellingRelationPageDefinition
            source: "LabellingRel_Intro.qml"
        },
        ComponentDefinition {
            id: controlsWithOnTouchPageDefinition
            source: "ControlsWithOnTouch_Intro.qml"
        },
        ComponentDefinition {
            id: customControlPageDefinition
            source: "CustomA11yObject_Intro.qml"
        },
        ComponentDefinition {
            id: customControlCPlusPlusPageDefinition
            source: "CustomCtrlInCPlusPlus_Intro.qml"
        },
        ComponentDefinition {
            id: punctuationReferencePageDefinition
            source: "PunctuationReference_Intro.qml"
        },
        ComponentDefinition {
            id: acronymsPageDefinition
            source: "Acronyms_Intro.qml"
        }
    ]

    onPopTransitionEnded: {
        // Destroy the popped Page once the back transition has ended.
        page.destroy();
    }
}
