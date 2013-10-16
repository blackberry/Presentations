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

Container {
    id: gameContainer

    property alias player1: player1
    property alias player2: player2
    
    // Theese are set to propert values for different screens in a LayoutUpdateHandler below.
    property int screenWidth: 1280
    property int screenHeight: 720
    property bool soundOn: false
    
    // Propeties for the player pads circular movement
    property real circleRadius: 720/2 // This relates to the image and not the device
    property real circleOffsetX:-720 * 0.24
    property real circleOffsetY: screenHeight/2

    background: Color.create("#1d1f1e")

	// Signals to the Game screen for triggering arduino countermeasures and updating controller devices.
    signal hitPaddle(bool player1)
    signal newGameStarted()
    signal lostGame(bool player1)
    signal counterMeasureDone()

    layout: DockLayout {
    }

    PlayerTrack {
        id: player1Track
        opacity: player1.opacity
        trackImageSource: "asset:///images/player1_track.png"
        deathImageSource: "asset:///images/player1-death.png"
        deathDuration: ball.paddleToWallDuration
        horizontalAlignment: HorizontalAlignment.Left
        verticalAlignment: VerticalAlignment.Center
    }
    
    PlayerTrack {
        id: player2Track
        opacity: player2.opacity
        trackImageSource: "asset:///images/player2_track.png"
        deathImageSource: "asset:///images/player2-death.png"
        deathDuration: ball.paddleToWallDuration
        horizontalAlignment: HorizontalAlignment.Right
        verticalAlignment: VerticalAlignment.Center
    }

    Regenerator {
        id: regenerator
        verticalAlignment: VerticalAlignment.Center
        horizontalAlignment: HorizontalAlignment.Center
        opacity: 0
    }

    // Counter measure presentation
    CounterMeasure {
        id: countermeassure
        horizontalAlignment: HorizontalAlignment.Center
        verticalAlignment: VerticalAlignment.Center

        onCounterMeasureDone: {
            if (player1) {
                player2Icons.activateCounterMeasure(player1);
            } else {
                player1Icons.activateCounterMeasure(player1);
            }
        }
    }

    // Icons
    Container {
        topPadding: 30
        layout: StackLayout {
            orientation: LayoutOrientation.LeftToRight
        }

        PlayerIcons {
            id: player1Icons
            rightPadding: 30
            color: player1.color
            rightToLeft: true
            playerString: player1.playerString
            onPlayerCounterMeasureDone: {
                counterMeasureDone();
            }
        }

        PlayerIcons {
            id: player2Icons
            leftPadding: 30
            color: player2.color
            rightToLeft: false
            playerString: player2.playerString

            onPlayerCounterMeasureDone: {
                counterMeasureDone();
            }
        }
    }

    // Score
    Container {
        layout: DockLayout{
        }
        verticalAlignment: VerticalAlignment.Fill
        horizontalAlignment: HorizontalAlignment.Fill

        PlayerScore {
            id: player1Score
            leftPadding: 50
            color: player1.color
            score: player1.score
            verticalAlignment: VerticalAlignment.Center
            horizontalAlignment: HorizontalAlignment.Left
        }

        PlayerScore {
            id: player2Score
            rightPadding: 50
            color: player2.color
            score: player2.score
            verticalAlignment: VerticalAlignment.Center
            horizontalAlignment: HorizontalAlignment.Right
        }
    }

    Container {
        id: playingfield
        
        horizontalAlignment: HorizontalAlignment.Fill
        verticalAlignment: VerticalAlignment.Fill

        layout: DockLayout {
        }

        // Game logics below
        function resetTheBall() {
            ball.resetTheBall();
        }

        function hitTestTheBalls() {
            if (ball.translationX > (screenHeight / 2)) {
                if (player2.hitTest(ball.translationY + ball.ballSize/2)) {
                    playingfield.calculateBallMovement();
                    hitPaddle(false);
                } else {
                    player1.score ++;
                    resetTheBall();
                    lostGame(false);
                    player2Track.playDeathCircle();
                }
            } else {
                if (player1.hitTest(ball.translationY + ball.ballSize/2)) {
                    playingfield.calculateBallMovement();
                    hitPaddle(true);
                } else {
                    player2.score ++;
                    resetTheBall();
                    lostGame(true);
                    player1Track.playDeathCircle();
                }
            }
        }

        function calculateBallMovement() {
            ball.calculateBallMovement();
            
            if (soundOn) {
                ploink.stop();
                ploink.play();
            }
        }

        Ball {
            id: ball
            screenHeight: gameContainer.screenHeight
            screenWidth: gameContainer.screenWidth
            circleRadius: gameContainer.circleRadius
            circleOffsetX: gameContainer.circleOffsetX
            circleOffsetY: gameContainer.circleOffsetY
            
            onBounceEnded: {
                playingfield.hitTestTheBalls();
            }

            onNewBallEnded: {
                playingfield.calculateBallMovement();
                newGameStarted();
            }
			
            onNewBall: {
                regenerator.show();
                                
                if (soundOn) {
                	newball2.play();
                }
            }
            
            onWallHit: {
                if (soundOn) {
                    crhas1.stop();
                    crhas1.play();
                }
            }
        }

        // Player 1 red rocker
        Player {
            id: player1
            color: Color.create("#ef2672")
            playerString: "player1"
            screenHeight: gameContainer.screenHeight
            circleRadius: gameContainer.circleRadius
            circleOffsetX: gameContainer.circleOffsetX
            circleOffsetY: gameContainer.circleOffsetY
            playerImage: "asset:///images/player1.png"
        }
        
        // Player 2 blue rocker
        Player {
            id: player2
            color: Color.create("#12d3dc")
            playerString: "player2"
            initialTranslation: -40
            screenHeight: gameContainer.screenHeight
            circleRadius: gameContainer.circleRadius
            circleOffsetX: gameContainer.circleOffsetX
            circleOffsetY: gameContainer.circleOffsetY
            playerImage: "asset:///images/player2.png"
            
            verticalAlignment: VerticalAlignment.Top
            horizontalAlignment: HorizontalAlignment.Right
        }
    }

    // Introduction instruction text
    IntroInstruction {
        id: instruction
    }

    onCreationCompleted: {
        OrientationSupport.supportedDisplayOrientation = SupportedDisplayOrientation.DisplayLandscape;
        if (soundOn) {
            silence.play();
        }
    }

    onLostGame: {
        //deActivateCounterMeasure();
    }
    
    onNewGameStarted: {
        deActivateCounterMeasure();
    }
    
    onSoundOnChanged: {
        if(soundOn){
            silence.play();    
        } else {
            silence.stop();
        }
        
    }

    attachedObjects: [
        MediaPlayer {
            id: ploink
            sourceUrl: "asset:///sounds/lovely-thump.wav"
        },
        MediaPlayer {
            repeatMode: RepeatMode.Track
            id: silence
            sourceUrl: "asset:///sounds/bg.wav"
        },
        MediaPlayer {
            id: newball1
            sourceUrl: "asset:///sounds/newball1.wav"
        },
        MediaPlayer {
            id: newball2
            sourceUrl: "asset:///sounds/newball2.wav"
        },
        MediaPlayer {
            id: crhas1
            sourceUrl: "asset:///sounds/sad-thump1.wav"
        },
        MediaPlayer {
            id: crhas2
            sourceUrl: "asset:///sounds/sad-thump2.wav"
        },
        LayoutUpdateHandler {
            onLayoutFrameChanged: {
                screenWidth = layoutFrame.width
                screenHeight = layoutFrame.height
            }
        }
    ]

    function startGame() {
        instruction.opacity = 0;
        player1Icons.opacity = 1;
        player2Icons.opacity = 1;
        player1Score.opacity = 1;
        player2Score.opacity = 1;

        ball.running = true;

        ball.newballAnim.play();
        if (soundOn) {
	        ploink.stop();
	        ploink.play();
        }
	} 
    
    function resetGame() {
        instruction.opacity = 1;
        player1Icons.opacity = 0;
        player2Icons.opacity = 0;
        player1Score.opacity = 0;
        player2Score.opacity = 0;
        player1.score = 0;
        player2.score = 0;
        
        ball.running = false;
        ball.opacity = 0;
        ball.stopTheBall();
    }    
    
    function showCounterMeasure(){
        countermeassure.show();
    }

    function setUpPlayerTextCounterMeasure(player1, counterData) {
        var result = false;
        if (player1) {
            result = player2Icons.setUpCounterMeasure(counterData);
        } else {
            result = player1Icons.setUpCounterMeasure(counterData);
        }

        return result;

    }

    function hideCounterMeasure(player1) {
        countermeassure.hide(player1);
    }

    function dismissCounterMeasure() {
        countermeassure.dismiss();
    }

    function deActivateCounterMeasure() {
        countermeassure.dismiss();
        player1Icons.deActivateCounterMeasure();
        player2Icons.deActivateCounterMeasure();
    }

    function playersConnected() {
        instruction.message = qsTr("<html><span style='color:#ffffff;font-size:14;font-weight:100'>TAP SCREEN TO PLAY!</span>\n<span style='color:#fc9359;font-size:18'>PLAYERS READY</span></html>")
    }
}
