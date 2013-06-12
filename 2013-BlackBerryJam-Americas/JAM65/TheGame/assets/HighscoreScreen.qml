/*
 * Copyright (c) 2011, 2012 Research In Motion Limited.
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
    content: Container {
        layout: DockLayout {
        }

        // The background
        Background {
        }
        Container {
            GameHeader {
                title: "Highscore"
            }
            SegmentedControl {
                horizontalAlignment: HorizontalAlignment.Center
                id: segment1
                Option {
                    text: "Sorting by moves"
                    value: "Moves"
                    selected: _HighscoreDataModel.model.sortingKeys[1] == "moves"
                }
                Option {
                    text: "Sorting by time"
                    value: "Time"
                    selected: _HighscoreDataModel.model.sortingKeys[1] == "time"
                }
                onSelectedIndexChanged: {
                    if (segment1.selectedValue == "Moves") {
                        _HighscoreDataModel.model.sortingKeys = [
                            "level",
                            "moves",
                            "time"
                        ]
                    } else {
                        _HighscoreDataModel.model.sortingKeys = [
                            "level",
                            "time",
                            "moves"
                        ]
                    }
                }
            }
            Container {
                // Give some padding to the borders...
                leftPadding: 50
                rightPadding: 50

                // List header
                HighscoreListHeader {
                }
                ListView {
                    layout: StackListLayout {
                        headerMode: ListHeaderMode.Sticky
                    }

                    // XML data model for Design view
                    /*dataModel: XmlDataModel {
                     * source: "highscoredb/highscore.xml"
                     * }*/

                    // SQL data model
                    dataModel: _HighscoreDataModel.model
                    listItemComponents: [
                        ListItemComponent {
                            type: "item"
                            HighscoreListItem {
                                id: highscoreItem
                                contextActions: ActionSet {
                                    title: highscoreItem.ListItem.indexInSection + 1 + ". " + highscoreItem.ListItem.data["name"] + ", level " + highscoreItem.ListItem.data["level"]
                                    subtitle: "Time: " + highscoreItem.millisecondsToString(highscoreItem.ListItem.data["time"]) + ", Moves: " + highscoreItem.ListItem.data["moves"]
                                    ActionItem {
                                        title: "Delete highscore"
                                    }
                                    ActionItem {
                                        title: "Change name"
                                    }
                                }
                            }
                        },
                        ListItemComponent {
                            type: "header"
                            Header {
                                title: "Level " + ListItemData
                            }
                        }
                    ]
                }
            }
        }
    }
    paneProperties: NavigationPaneProperties {
        backButton: ActionItem {
            onTriggered: {
                soundManager.play("klick2");
                navigationPane.pop().destroy();
            }
        }
    }
}
