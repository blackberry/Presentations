
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
        title: qsTr("Before A11y: No Custom A11y Object") + Retranslate.onLocaleOrLanguageChanged
    }
    Container {
        ListView {
            dataModel: XmlDataModel {
                source: "data.xml"
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
                                ImageView {
                                    verticalAlignment: VerticalAlignment.Center
                                    imageSource: "asset:///" + ListItemData.image
                                }
                                Container {
                                    //BAD: accessibility property is not set.
                                    horizontalAlignment: HorizontalAlignment.Fill
                                    layoutProperties: StackLayoutProperties {
                                        spaceQuota: 4
                                    }
                                    Label {
                                        id: mainLabelDelete
                                        text: ListItemData.name
                                        textStyle.fontSize: FontSize.Medium
                                    }
                                    Label {
                                        id: subLabelDelete
                                        text: ListItemData.description
                                        textStyle.fontSize: FontSize.XSmall
                                    }
                                }
                                Button {
                                    horizontalAlignment: HorizontalAlignment.Right
                                    verticalAlignment: VerticalAlignment.Center
                                    text: "!"
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
            source: "CustomA11yObject_After.qml"
        }
    ]

}
