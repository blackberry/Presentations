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

import bb.cascades 1.0
import QtQuick 1.0
import CustomTimer 1.0
import "actionbars"

Page {
    id: winesPageContainer
    Container {

        WineActionBarContent {
        }

        WineListsContainer {
            id: wineListsContainer
        }
    }

    attachedObjects: [
        ActionItem {
            id: whiteWines
            title: "White Wines"
            imageSource: "asset:///images/white_small.png"
            ActionBar.placement: ActionBarPlacement.OnBar
            onTriggered: {
                wineListsContainer.toggleWhiteWinesList()
                whiteWines.imageSource = "asset:///images/white_large.png"
                redWines.imageSource = "asset:///images/red_small.png"
            }
        },
        ActionItem {
            id: redWines
            title: "Red Wines"
            imageSource: "asset:///images/red_large.png"
            ActionBar.placement: ActionBarPlacement.OnBar
            onTriggered: {
                wineListsContainer.toggleRedWinesList()
                redWines.imageSource = "asset:///images/red_large.png"
                whiteWines.imageSource = "asset:///images/white_small.png"

            }
        }
    ]
    onCreationCompleted: {
        addAction(whiteWines, ActionBarPlacement.OnBar);
        addAction(redWines, ActionBarPlacement.OnBar);
    }

}
