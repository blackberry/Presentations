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

Container {
    id: root
    signal screenHidden();
    visible: false
    onCreationCompleted: {
        gameManager.gameOver.connect(root.show);
    }
    preferredHeight: 1280
    preferredWidth: 768
    layout: DockLayout {
    }
    Container {
        id: dimmer
        background: Color.Black
        horizontalAlignment: HorizontalAlignment.Fill
        verticalAlignment: VerticalAlignment.Fill
        opacity: 0.75
    }
    Container {
        id: panel
        layout: DockLayout {
        }
        ImageView {
            imageSource: "asset:///images/popup.amd"
            opacity: 1
            horizontalAlignment: HorizontalAlignment.Fill
            verticalAlignment: VerticalAlignment.Fill
        }
        ImageView {
            imageSource: "asset:///images/STICKER_numberone.png"
            rotationZ: 15
            translationX: -20
            translationY: 8
            horizontalAlignment: HorizontalAlignment.Right
        }
        Container {
            layout: DockLayout {
            }
            leftPadding: 80
            rightPadding: 110
            topPadding: 50
            bottomPadding: 130
            Container {
                TextArea {
                    textStyle.color: Color.Black
                    textStyle.base: SystemDefaults.TextStyles.BigText
                    textStyle.textAlign: TextAlign.Center
                    editable: false
                    text: "Level " + gameManager.currentLevel + " solved!"
                }
                TextArea {
                    textStyle.color: Color.Black
                    textStyle.base: SystemDefaults.TextStyles.BodyText
                    textStyle.textAlign: TextAlign.Center
                    editable: false
                    text: "Moves: " + _ScoreCounter.moves + "   |   Time: " + _ScoreCounter.seconds + " seconds"
                }
                Container {
                    bottomMargin: 50
                    horizontalAlignment: HorizontalAlignment.Center
                    Label {
                        id: playerName
                        textStyle.color: Color.Black
                        textStyle.fontWeight: FontWeight.Bold
                        textStyle.base: SystemDefaults.TextStyles.BigText
                        text: settings.getValueFor("player_name", "playerName")
                        onTouch: {
                            if (event.isUp()) {
                                playerName.visible = false;
                                textField.visible = true;
                                button.visible = true;
                            }
                        }
                    }
                    TextField {
                        property string newText: text
                        id: textField
                        text: settings.getValueFor("player_name", "playerName")
                        visible: false
                        onTextChanging: {
                            newText = text; // TODO:: should this really be needed?
                        }
                    }
                }
                Button {
                    id: button
                    text: "Change name"
                    visible: false
                    onClicked: {
                        // TODO:: need to hide the VKB here
                        button.visible = false;
                        playerName.visible = true;
                        textField.visible = false;
                        playerName.text = textField.newText;
                        _ScoreCounter.updateHighscoreName(playerName.text);
                    }
                    horizontalAlignment: HorizontalAlignment.Center
                }
                Button {
                    id: playAgain
                    text: "GO TO NEXT LEVEL"
                    onClicked: {
                        hide();
                        gameManager.notifyLoadNextLevel();
                    }
                    horizontalAlignment: HorizontalAlignment.Center
                }
                Button{
                    id:createScrapCard
                    text:"CREATE SCRAP CARD"
                    horizontalAlignment: HorizontalAlignment.Center
                    onClicked:{
                        cs.createWinnerScrapCard();
                    }
                }
            }
        }
    }
    animations: [
        ParallelAnimation {
            id: showTransition
            animations: [
                TranslateTransition {
                    target: panel
                    fromY: -800
                    toY: 200
                    duration: 1000
                    easingCurve: StockCurve.BounceOut
                },
                FadeTransition {
                    target: dimmer
                    fromOpacity: 0
                    toOpacity: 0.75
                }
            ]
            onStarted: {
                root.visible = true
            }
        },
        ParallelAnimation {
            id: hideTransition
            animations: [
                TranslateTransition {
                    target: panel
                    fromY: 200
                    toY: -800
                    duration: 500
                },
                FadeTransition {
                    target: dimmer
                    fromOpacity: 0.75
                    toOpacity: 0
                }
            ]
            onEnded: {
                root.visible = false
                screenHidden();
            }
        }
    ]
    function show() {
        showTransition.play();
        _ScoreCounter.updateHighscoreName(playerName.text);
    }
    function hide() {
        hideTransition.play();
    }
}
