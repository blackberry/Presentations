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
import thegame 1.0

Page {
    // TODO:: peek touch event stealing workaround
    signal screenClosed()
    content: Container {
        layout: DockLayout {
        }
        Background {
        }
        Container {
            GameHeader {
                title: "Level " + gameManager.currentLevel
            }
            Container {
                topMargin: 30
                bottomMargin: 30
                layout: DockLayout {
                }
                leftPadding: 20
                rightPadding: 20
                horizontalAlignment: HorizontalAlignment.Fill
                Label {
                    id: labelID
                    //text: "MOVES: 999"
                    text: "MOVES:" + _ScoreCounter.moves
                    textStyle.base: SystemDefaults.TextStyles.BigText
                    textStyle.color: Color.White
                }
                Label {
                    id: timier
                    //text: "TIME: 999"
                    text: "TIME:" + _ScoreCounter.seconds
                    textStyle.base: SystemDefaults.TextStyles.BigText
                    textStyle.color: Color.White
                    horizontalAlignment: HorizontalAlignment.Right
                }
            }
            Container {
                horizontalAlignment: HorizontalAlignment.Center
                Container {
                    layout: AbsoluteLayout {
                    }
                    ImageView {
                        imageSource: "asset:///images/gamegrid.png"
                        visible: settings.getValueFor("show_grid", "true")
                    }
                    ImageView {
                        imageSource: "asset:///images/gameborder.png"
                    }
                    Container {
                        layout: AbsoluteLayout {
                        }
                        layoutProperties: AbsoluteLayoutProperties {
                            positionX: 23
                            positionY: 29
                        }
                        GameModelView {
                        }
                    }
                }
            }
        }

        // container blocking touch events when game over popup appears
        Container {
            onCreationCompleted: {
                gameManager.gameOver.connect(blocker.block);
                gameManager.loadNextLevel.connect(blocker.unblock);
            }
            id: blocker
            visible: false
            horizontalAlignment: HorizontalAlignment.Fill
            verticalAlignment: VerticalAlignment.Fill
            onTouch: {
            }
            function block() {
                soundManager.play("bonus");
                myCustomDialog.open();
            }
            function unblock() {
            }
        }
    }
    attachedObjects: [
        Dialog {
            id: myCustomDialog
            GameOverScreen {
                id: gameOverScreen
                horizontalAlignment: HorizontalAlignment.Fill
                verticalAlignment: VerticalAlignment.Fill
                onScreenHidden: {
                    myCustomDialog.close();
                }
            }
        }
    ]
    paneProperties: NavigationPaneProperties {
        backButton: ActionItem {
            title: "Quit"
            onTriggered: {
                gameManager.notifyGameCancelled();
                soundManager.play("klick2");
                gameOverScreen.hide();
                // TODO:: peek touch event stealing workaround
                screenClosed();
                navigationPane.pop().destroy();
                OrientationSupport.supportedDisplayOrientation = SupportedDisplayOrientation.All
            }
        }
    }
    actions: [
        ActionItem {
            title: "Refresh"
            imageSource: "images/restartGame.png"
            onTriggered: {
                soundManager.play("klick2");
                gameManager.notifyResetGame();
            }
            ActionBar.placement: ActionBarPlacement.OnBar
        }
    ]
}
