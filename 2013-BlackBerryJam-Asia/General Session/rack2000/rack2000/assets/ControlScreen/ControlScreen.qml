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
import com.rack2000 1.0
import bb.multimedia 1.0
import "../"

Page {
    property alias controlColor: colorContainer.background
    property string deviceAddress: ""
    property bool connected: false;
    
    signal leaveGame()

    actionBarVisibility: ChromeVisibility.Hidden

    Container {
        id: controlContainer
        background: Color.create("#1d1f1e")
        
        layout: DockLayout {        
        }
        
        Container {
            id: colorContainer
            horizontalAlignment: HorizontalAlignment.Fill
            verticalAlignment: VerticalAlignment.Fill
            opacity: 0                
        }
        
        Container {
            horizontalAlignment: HorizontalAlignment.Center
            verticalAlignment: VerticalAlignment.Center
            
            GameText {
                id: gameText
                horizontalAlignment: HorizontalAlignment.Center
                opacity: 0.0
                maxHeight: 0
            }
            
            ControlText {                
                id: controlText
                topMargin: 100
                message: qsTr("<html><span style='font-size:14;color:#ef2672;font-weight:100'>CONNECT</span>"+
                "\n<span style='font-size:18;color:#ef2672;'>DEVICE</span></html>")
            }
        }

        onCreationCompleted: {
            controlColor = Color.create("#1d1f1e");
            OrientationSupport.supportedDisplayOrientation =  SupportedDisplayOrientation.DisplayPortrait; 
        }
        
        onTouch: {
            if (event.isDown()) {
                if (controlText.message.indexOf("WELCOME") != -1) {
                    btController.chatManager.sendSPPMessage("isReady");
                } else if (controlText.message.indexOf("COUNTER") != -1) {
                    btController.chatManager.sendSPPMessage("counter");
                }
            }
        }
    }
    
    onConnectedChanged: {
        if(connected) {
            controlText.message = qsTr("<html><span style='font-size:14;font-weight:100;color:#ffffff'></span>\n<span style='font-size:18;color:#ffffff;'>READY!</span></html>")
            gameText.monoChrome = true;
            colorContainer.opacity = 1;
            gameText.opacity = 1;
            gameText.maxHeight = undefined;
            accelertionSensor.start()
            sensorEvalTimer.start();            
        }
    }

    onCreationCompleted: {
        _app.createMacAddressHandler();
        _btAddressHandler.macAddressChanged.connect(connecToGame);
        _btAddressHandler.lostEvent.connect(lostEvent);
        _btAddressHandler.handOver.connect(handOver);
    }

    attachedObjects: [
        MediaPlayer {
            id: myPlayer
            sourceUrl: "asset:///sounds/lovely-thump.wav"
        },
        QTimer {
            id: sensorEvalTimer
            //singleShot: true
            interval: 80

            onTimeout: {
                if(deviceAddress != "") {
                    //10 if you want big hand movement
                    //               btController.chatManager.sendSPPMessage("y:" + (260 + accelertionSensor.y * 10));
                    //46 is good for hand-gaming
                    //                btController.chatManager.sendSPPMessage("y:" + (260 + accelertionSensor.y * 46));
                    
                    btController.chatManager.sendSPPMessage("y:" + (-60 + (accelertionSensor.y * 150)));                    
                }

            }
        },
        AccelerationSensor {
            id: accelertionSensor
        },
        BTController {
            id: btController

            chatManager.onChatEnded: {
                // The server disconnected leave the game.
                leaveGame();
            }

            chatManager.onChatHistoryChanged: {
                if (chatHistory.indexOf("ploink") != -1) {
                    myPlayer.play();
                } else {
                    controlText.message = qsTr("<html><span style='font-size:14;font-weight:100;color:#ffffff'></span>\n<span style='font-size:18;color:#ffffff;'>"+chatHistory+"</span></html>")
                }
            }
        }
    ]
    
    function lostEvent() {
        controlText.message = qsTr("<html><span style='font-size:12;font-weight:100;color:#ef2672'>LOST CONNECTION</span>"+
        "\n<span style='font-size:18;color:#ef2672;'>TRY AGAIN</span></html>")
    }

    function handOver() {
        controlText.message = qsTr("<html><span style='font-size:12;font-weight:100;color:#ef2672'>WAIT FOR IT</span>"+
        "\n<span style='font-size:18;color:#ef2672;'>CONNECTING</span></html>")
    }
	
    function connecToGame(serverAddress) {
        console.debug("!!! Try to connect " + connected +" " +serverAddress);
        if(!connected) {
            console.debug("connecToGame " + serverAddress);
            deviceAddress = serverAddress;            
            btController.setRemoteDevice(deviceAddress);
            
            if (btController.chatManager.connectToSPPService("00001101-0000-1000-8000-00DEADBEEFAA")) {
                //Player 1 connected
                controlColor = Color.create("#ef2672");
                btController.chatManager.sendSPPMessage("player1");
                connected = true;
                console.debug("!!! Connected to AA")
            } else if (btController.chatManager.connectToSPPService("00001101-0000-1000-8000-00DEADBEEFAB")) {
                //Player 2 connected
                controlColor = Color.create("#12d3dc");
                btController.chatManager.sendSPPMessage("player2");
                connected = true;
                console.debug("!!! Connected to AB")
            } else {
                controlText.message = qsTr("<html><span style='font-size:12;font-weight:100;color:#ef2672'>OH OOH</span>"+
                "\n<span style='font-size:18;color:#ef2672;'>RESTART APP</span></html>")
                console.debug("Could not connect to the game");
            }
        }
        return connected;
    }
}
