/* Copyright (c) 2013 BlackBerry.
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
import "GameScreen"
import "ControlScreen"

NavigationPane {
    id: nav
    property bool gameServer: false
    property Page controlPage;
    property Page serverPage;

    Menu.definition: MenuDefinition {
        actions: [
            ActionItem {
                // Use this action to enter a one device play mode for testing the UI.
                title: qsTr("Test Mode")
                onTriggered: {
                    serverPage = testScreenDef.createObject()
                    gameServer = true;
                    nav.push(serverPage);
                }
            },
            ActionItem {
                // Used to reset the game while hosting a game.
                title: qsTr("Reset")
                enabled: (serverPage != undefined)
                onTriggered: {
                    if(gameServer) {
                        serverPage.resetGame();
                    }
                }
            },
            ActionItem {
                // Toggle sound effects on and off.
                title: qsTr("Toggle Sound");
                enabled: (serverPage != undefined)
                onTriggered: {
                    if(gameServer) {
                        serverPage.soundOn = !serverPage.soundOn; 
                    }
                }
            }
        ]
    }

    Page {
        id: startPage

        Container {
            background: Color.create("#1d1f1e")

            layout: DockLayout {
            }

            Container {
                horizontalAlignment: HorizontalAlignment.Center
                verticalAlignment: VerticalAlignment.Center

                GameText {
                }

                Container {
                    topMargin: 90
                    horizontalAlignment: HorizontalAlignment.Center
                    opacity: 0
                    
                    GameButton {
                        horizontalAlignment: HorizontalAlignment.Fill
                        text: qsTr("CREATE")
                        
                        onClicked: {
                            gameServer = true;
                            serverPage = gameScreenDef.createObject()
                            nav.push(serverPage);
                        }
                    }
                    
                    GameButton {
                        horizontalAlignment: HorizontalAlignment.Fill
                        text: qsTr("JOIN")
                        
                        
                        onClicked: {
                            controlPage = controlScreenDef.createObject();
                            nav.push(controlPage);
                            controlPage.leaveGame.connect(onLeaveGame);
						}
                    }
                    
                    animations: FadeTransition {
                        id: showAnim
                        toOpacity: 1
                        duration: 200
                        delay: 100
                    }
                }
            }
        }
    }
    
    onCreationCompleted: {
        showAnim.play();
    }
    
    onPopTransitionEnded: {
        page.destroy();
        gameServer = false;
        OrientationSupport.supportedDisplayOrientation =  SupportedDisplayOrientation.All;
    }
    
    attachedObjects: [
        ComponentDefinition {
            id: gameScreenDef
            source: "asset:///GameScreen/GameScreen.qml"
        },
        ComponentDefinition {
            id: controlScreenDef
            source: "asset:///ControlScreen/ControlScreen.qml"
        },
        ComponentDefinition {
            id: testScreenDef
            source: "asset:///GameScreen/TestScreen.qml"
        },
        OrientationHandler {
            onOrientationChanged: {
            }
        }
    ]

    function onLeaveGame() {
        controlPage.leaveGame.disconnect(onLeaveGame);
        nav.pop();
    }
}