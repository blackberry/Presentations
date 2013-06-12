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
    id: root
    signal goBack
    signal goGame
    signal goSettings
    signal goHighscore
    signal goAbout
    content: Container {
        layout: DockLayout {
        }
        property bool first: true;
        // The background
        Background {
        }
        ImageView {
            imageSource: "asset:///images/bg_gloss.amd"
            preferredHeight: 610
            horizontalAlignment: HorizontalAlignment.Fill
        }

        // The Logo
        Container {
            id: logoContainer
            horizontalAlignment: HorizontalAlignment.Center
            layout: DockLayout {
            }
            ImageView {
                id: logoView
                imageSource: "asset:///images/logo.png"
            }
        }

        // The Buttons
        Container {
            id: buttonContainer
            layout: StackLayout {
            }
            topPadding: 300
            horizontalAlignment: HorizontalAlignment.Fill
            verticalAlignment: VerticalAlignment.Center
            Button {
                horizontalAlignment: HorizontalAlignment.Center
                text: "Start"
                onClicked: {
                    soundManager.play("klick1");
                    gameManager.resetGame();
                    root.goGame();
                }
                opacity: cs.fullscreenMode ? 1 : 0
            }
            Button {
                horizontalAlignment: HorizontalAlignment.Center
                text: "Settings"
                onClicked: {
                    soundManager.play("klick1");
                    root.goSettings();
                }
                opacity: cs.fullscreenMode ? 1 : 0
            }
            Button {
                horizontalAlignment: HorizontalAlignment.Center
                text: "Highscore"
                onClicked: {
                    soundManager.play("klick1");
                    root.goHighscore();
                }
                opacity: cs.fullscreenMode ? 1 : 0
            }
            Button {
                horizontalAlignment: HorizontalAlignment.Center
                text: "About"
                onClicked: {
                    root.goAbout();
                }
                opacity: cs.fullscreenMode ? 1 : 0
            }
        }
        attachedObjects: [
            OrientationHandler {
                onOrientationChanged: {
                    if (uiOrientation == UiOrientation.Portrait) {

                        //in 1225 rc5 setting the toppPadding does not work
                        //so we cheat and use two different images (where logo.png)
                        //have a built in padding of 100 pixels
                        //feel free to try the topContainer.topPadding = 100 in later revisions
                        logoContainer.horizontalAlignment = HorizontalAlignment.Center
                        logoView.imageSource = "asset:///images/logo.png"
                        buttonContainer.horizontalAlignment = HorizontalAlignment.Center
                        buttonContainer.verticalAlignment = VerticalAlignment.Center
                        buttonContainer.topPadding = 100
                        buttonContainer.rightPadding = 50
                        showAnimalAnimation.play();
                    } else if (uiOrientation == UiOrientation.Landscape) {
                        logoContainer.horizontalAlignment = HorizontalAlignment.Left
                        logoView.imageSource = "asset:///images/logo_landscape.png"
                        buttonContainer.horizontalAlignment = HorizontalAlignment.Right
                        buttonContainer.verticalAlignment = VerticalAlignment.Top
                        buttonContainer.topPadding = 300
                        buttonContainer.rightPadding = 0
                        hideAnimalAnimation.play();
                    }
                }
            }
        ]
        // The Turtle
        Container {
            horizontalAlignment: HorizontalAlignment.Left
            verticalAlignment: VerticalAlignment.Bottom
            overlapTouchPolicy: OverlapTouchPolicy.Allow
            ImageView {
                imageSource: settings.theme == "Turtle" ? "asset:///images/bg_turtle.png" : "asset:///images/bg_monkey.png"
                translationX: settings.theme == "Turtle" ? 0 : -50
                translationY: settings.theme == "Turtle" ? 0 : 200
                overlapTouchPolicy: OverlapTouchPolicy.Allow
                opacity: cs.fullscreenMode ? 1 : 0
                animations: [
                    TranslateTransition {
                        id: hideAnimalAnimation
                        toX: -500
                        duration: 500
                    },
                    TranslateTransition {
                        id: showAnimalAnimation
                        toX: 0
                        duration: 500
                    }
                ]
            }
        }
    }
}
