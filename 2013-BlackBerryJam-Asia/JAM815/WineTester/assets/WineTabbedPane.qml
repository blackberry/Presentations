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

/*
 * WineTabbedPane: Main application's container
 */
import bb.cascades 1.0
import "RenderRules.js" as RenderRules

TabbedPane {
    id: homeTabbedPane
    showTabsOnActionBar: RenderRules.TabsDisplayedOnActionBar()

    activeTab: winesTab

    Menu.definition: MenuDefinition {
        actions: [
            ActionItem {
                id: logoutActionItem
                objectName: "logoutActionItem"
                title: "Logout"
                imageSource: "asset:///images/logout.png"
                onTriggered: {
                    _homeState.logoutRequest();
                }
            }
        ]
    }
    Tab {
        id: winesTab
        title: qsTr("Our Wines")
        imageSource: "asset:///images/wine_tab.png"
        WinePageController {
            
        }
    }
    Tab {
        id: mapTab
        title: qsTr("Map")
        imageSource: "asset:///images/Map-icon.png"
        MapPage {
        }
    }
    Tab {
        id: contactTab
        title: qsTr("Info")
        imageSource: "asset:///images/call_us.png"
        ContactUsPage {
        }
    }
}
