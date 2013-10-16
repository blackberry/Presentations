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
import bb.multimedia 1.0
import com.rack2000 1.0

Page {
    property bool testMode: false
    property bool settingUpNewGame: false
    property alias playField: playField
    property alias countermeassureTimer: countermeassureTimer
    property alias soundOn: playField.soundOn
    
    actionBarVisibility: ChromeVisibility.Hidden

    PlayField {
        id: playField
        
        onHitPaddle: {
            if (!testMode) {
                if (player1) {
                    btController.chatManager.sendSPPMessage("ploink");
                } else {
                    btController.chatManager2.sendSPPMessage("ploink");
                }
            }
        }
        
        onLostGame: {
            countermeassureTimer.stop();
            if (! testMode) {
                if (player1) {
                    btController.chatManager.sendSPPMessage("BOOOH!");
                    btController.chatManager2.sendSPPMessage("SCORE!");
                } else {
                    btController.chatManager.sendSPPMessage("SCORE!");
                    btController.chatManager2.sendSPPMessage("BOOOH!");
                }
            }
            playField.dismissCounterMeasure();
            settingUpNewGame = true;
        }
        
        onNewGameStarted: {
            // Stop all Counter Measures here.
            playField.player1.reverseCounterMeasure = false;
            playField.player2.reverseCounterMeasure = false;
            _btArduino.sendMyMessage("0");
            
            countermeassureTimer.start();
            if (! testMode) {
                btController.chatManager.sendSPPMessage("PLAY!");
                btController.chatManager2.sendSPPMessage("PLAY!");
            }
            settingUpNewGame = false;
        }

        // Trigger test if UI is in test mode.
        property int dummyCounter: 0
        onTouch: {
            if (testMode && event.touchType == TouchType.Up) {
                dummyCounter = dummyCounter + 1;
                if (dummyCounter % 2 == 0)
                    triggerCounter(playField.player1);
                else
                    triggerCounter(playField.player2);
            }
        }
    }

    onCreationCompleted: {
        _app.createArduinoController();
        
        // Start bluetooth server and navigate to the game screen and wait for players
        btController.chatManager.startSPPServer("00001101-0000-1000-8000-00DEADBEEFAA");
        btController.chatManager2.startSPPServer("00001101-0000-1000-8000-00DEADBEEFAB");
        playField.counterMeasureDone.connect(counterMeasureDone);
    }
    
    function updatePosition(player, position) {        
        player.position = position
    }
    
    function startGame() {
        if (playField.player2.isReady && playField.player1.isReady) {
        	playField.startGame();
            btController.chatManager.sendSPPMessage("PLAY!");
            btController.chatManager2.sendSPPMessage("PLAY!");
            countermeassureTimer.start();
        }
    }
    
    function resetGame() {
        btController.chatManager.sendSPPMessage("WELCOME!");
        btController.chatManager2.sendSPPMessage("WELCOME!");
        playField.player1.isReady = false;
        playField.player2.isReady = false;
        countermeassureTimer.stop();
        playField.dismissCounterMeasure();
        _btArduino.sendMyMessage("0");
        
        playField.resetGame();
    }

    
    function resetPlayer(player) {
        player.isConnected = false;
        player.isReady = false;
        resetGame();
    }
    
    function startInstruction() {
        if (playField.player2.isConnected && playField.player1.isConnected) {
            playField.playersConnected();
        }
    }

    function triggerCounter(player) {
        var player1 = (player.playerString == "player1")?true:false;
        var nbrOfCounterMeasures = counterModel.childCount([]);        
        var counterData = counterModel.data([ (counterModel.currentCounter%(nbrOfCounterMeasures))])
        var result = playField.setUpPlayerTextCounterMeasure(player1, counterData);
                
        var counter = 0;
        while (counter < nbrOfCounterMeasures) {
            if(result) {
                break;
            }
            
            // The first selected counter is already running we have to try another one.
            counterData = counterModel.data([ counter ]);
            result = playField.setUpPlayerTextCounterMeasure(player1, counterData);
            counter = counter + 1;
        }

        if(result) {
            playField.hideCounterMeasure(player1);
            
            // Send message to trigger counter on the opposite player.
            if(counterData.type == "hw"){
                if(player1) {
                    _btArduino.sendMyMessage(counterData.player2Message);
                } else {
                    _btArduino.sendMyMessage(counterData.player1Message);    
                }                
            } else {
                if(player1){
                    playField.player2.reverseCounterMeasure = true;
                } else {
                    playField.player1.reverseCounterMeasure = true;
                }
            }
        } else {
            // All counters are running on the current player.
            playField.dismissCounterMeasure();
        }
        
        btController.chatManager.sendSPPMessage("PLAY!");
        btController.chatManager2.sendSPPMessage("PLAY!");
        counterModel.currentCounter = counterModel.currentCounter + 1;
    }
    
    function counterMeasureDone() {
        if(!settingUpNewGame) {
            // Counter measure has been shown, triggered and all animations are done start timer again.
            countermeassureTimer.start();            
        }
    }
    
    
    function handleMessage(manager, message, player) {
        if (message.indexOf("y:") !== -1) {
            var parseString = message.split(":");
            updatePosition(player, parseString[1]);
        } else if (message == player.playerString) {
            player.isConnected = true;
            manager.sendSPPMessage("WELCOME!");
            startInstruction();
        } else if (message == "isReady") {
            manager.sendSPPMessage("GET READY!");
            player.isReady = true
            startGame();
        } else if (message == "counter") {
            triggerCounter(player);
        }
    }
    
    attachedObjects: [
        XmlDataModel {
            id: counterModel
            property int currentCounter: 0;
          	source: "asset:///models/counter.xml"  
        },
        QTimer {
            id: countermeassureTimer
            singleShot: true
            interval: 2 * 1000 

            onTimeout: {
                // Show counter measure
                playField.showCounterMeasure();
                btController.chatManager.sendSPPMessage("COUNTER-MEASURE");
                btController.chatManager2.sendSPPMessage("COUNTER-MEASURE");
                countermeassureTimer.interval = (Math.random()*2.0)*1000 + 1 * 1000
            }
        },
        BTController {
            id: btController

            chatManager.onChatEnded: {
            	resetPlayer(playField.player1);
                chatManager.startSPPServer("00001101-0000-1000-8000-00DEADBEEFAA")
            }

            chatManager2.onChatEnded: {
                resetPlayer(playField.player2);
                chatManager2.startSPPServer("00001101-0000-1000-8000-00DEADBEEFAB")
            }

            chatManager.onChatHistoryChanged: {
                handleMessage(chatManager, chatHistory, playField.player1);
            }

            chatManager2.onChatHistoryChanged: {
                handleMessage(chatManager2, chatHistory, playField.player2);
            }
        }
    ]
}
