/* Copyright (c) 2013 Research In Motion Limited.
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


import bb.cascades 1.0

Page {
    id: details
    property variant data
    titleBar: TitleBar {
        id: titlebar
        title: data.name
    }
    ScrollView {
        Container {
            TextArea {
                editable: false
                textStyle.fontSize: FontSize.XLarge
                text: data.location
            }
            Button {
                id: menuButton
                text: "Display menu"
                onClicked: {
                    var menu = menuPage.createObject()
                    menu.data = details.data
                    nav.push(menu)
                }
            }
            TextArea {
                editable: false
                textStyle.fontSize: FontSize.Large
                text: data.description
            }
            TextArea {
                editable: false
                textStyle.fontSize: FontSize.Large
                text: data.phone
            }
            TextArea {
                editable: false
                textStyle.fontSize: FontSize.Large
                text: data.email
            }
            TextArea {
                editable: false
                textStyle.fontSize: FontSize.Large
                text: data.web
            }
        }
    }
    attachedObjects: [
        ComponentDefinition {
            id: menuPage
            source: "Menu.qml"
        }
    ]
    actions: [
        ActionItem {
            title: "Display Menu"
            imageSource: "asset:///images/ic_open.png"
            ActionBar.placement: ActionBarPlacement.OnBar
            onTriggered: {
                menuButton.clicked()
            }
            shortcuts: [
                Shortcut {
                    key: "m"
                }
            ]
        }
    ]
}