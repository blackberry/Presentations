
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
        title: qsTr("After A11y: Custom A11y Object") + Retranslate.onLocaleOrLanguageChanged
    }
    Container {
        ListView {
            dataModel: XmlDataModel {
                source: "data.xml"
            }
            onTriggered: {
                console.log("**onTriggered() called!! indexPath: " + indexPath);
            }

            listItemComponents: [
                ListItemComponent {
                    type: "item"

                    CustomListItem {
                        highlightAppearance: HighlightAppearance.Frame
                        dividerVisible: true
                        content: [
                            Container {
                                layout: StackLayout {
                                    orientation: LayoutOrientation.LeftToRight
                                }
                                horizontalAlignment: HorizontalAlignment.Fill
                                accessibility: CustomA11yObject {
                                    role: A11yRole.List
                                    name: ListItemData.name
                                    description: ListItemData.description
                                }

                                ImageView {
                                    verticalAlignment: VerticalAlignment.Center
                                    imageSource: "asset:///" + ListItemData.image
                                    accessibilityMode: A11yMode.Collapsed

                                }
                                Container {
                                    horizontalAlignment: HorizontalAlignment.Fill
                                    layoutProperties: StackLayoutProperties {
                                        spaceQuota: 4
                                    }
                                    accessibilityMode: A11yMode.Collapsed
                                    Label {
                                        id: mainLabelDelete
                                        text: ListItemData.name
                                        textStyle.fontSize: FontSize.Medium
                                        accessibilityMode: A11yMode.Collapsed
                                    }
                                    Label {
                                        id: subLabelDelete
                                        text: ListItemData.description
                                        textStyle.fontSize: FontSize.XSmall
                                        accessibilityMode: A11yMode.Collapsed
                                    }
                                }
                                Button {
                                    horizontalAlignment: HorizontalAlignment.Right
                                    verticalAlignment: VerticalAlignment.Center
                                    text: "!"
                                    accessibility.name: qsTr("Punishment for ") + ListItemData.name + Retranslate.onLocaleOrLanguageChanged
                                    layoutProperties: StackLayoutProperties {
                                        spaceQuota: 1
                                    }
                                    attachedObjects: [
                                        SystemToast {
                                            id: catnip
                                            body: "No more catnip for you!"
                                        }
                                    ]
                                    onClicked: {
                                        catnip.show()
                                    }
                                }
                            }
                        ]
                    }
                }
            ]
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
