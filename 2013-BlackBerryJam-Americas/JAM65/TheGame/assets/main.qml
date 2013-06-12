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

NavigationPane {
    id: navigationPane
    peekEnabled: true
    StartScreen {
        id: startScreen
        onGoGame: {
            OrientationSupport.supportedDisplayOrientation = SupportedDisplayOrientation.DisplayPortrait
            var page = gameScreen.createObject();
            navigationPane.push(page);
            gameManager.notifyGameStart();
            
            // TODO:: workaround since peek is stealing touch events from the game
            navigationPane.peekEnabled = false
            page.screenClosed.connect(gameSceenClosed);
        }
        onGoSettings: {
            settingsSheet.open();
        }
        onGoHighscore: {
            var page = highscoreScreen.createObject();
            navigationPane.push(page);
        }
        onGoAbout: {
            var page = aboutScreen.createObject();
            navigationPane.push(page);
        }
        
        // TODO:: workaround since peek is stealing touch events from the game
        function gameSceenClosed() {
            navigationPane.peekEnabled = true
        }
    }

    // when setting up is done, start playing the intro song
    onCreationCompleted: {
        soundManager.play("song1");
    }
    attachedObjects: [
        ComponentDefinition {
            id: gameScreen
            source: "GameScreen.qml"
        },
        ComponentDefinition {
            id: highscoreScreen
            source: "HighscoreScreen.qml"
        },
        ComponentDefinition {
            id: aboutScreen
            source: "AboutScreen.qml"
        },
        Sheet {
            peekEnabled: true
            id: settingsSheet
            SettingsScreen {
                onCloseSheet: {
                    settingsSheet.close();
                }
            }
        }
    ]
}
