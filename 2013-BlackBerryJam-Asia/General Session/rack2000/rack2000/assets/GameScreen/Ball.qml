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

ImageView {
    id: ball
    property bool running: false
    
    // Animations
    property alias wobble: wobble
    property alias bounce: bounce
    property alias newballAnim: newballAnim

	// Animation properites
    property int paddleToWallDuration: 0
    property int bounceDuration
    
    property int screenWidth: 1280
    property int screenHeight: 720
    
    property int bounceStartY: 0
    property int bounceStartX: 0
    
    property int bounceEndX: 0
    property int bounceEndY: 0
    
    property int ballSize: 40
    property int paddleWidth: 40
    
    // Circle segment properties set from outside
    property real circleRadius
    property real circleOffsetX
    property real circleOffsetY

    signal bounceEnded() 
    signal wallHit()   
    signal newBallEnded()
    signal newBall()

    imageSource: "asset:///images/ball.png"
    scaleX: 1
    scaleY: 1
    opacity: 0

    animations: [
        ParallelAnimation {
            id: wobble
            animations: [
                RotateTransition {
                    fromAngleZ: 0
                    toAngleZ: 360
                    duration: 600 + 400 + 400
                    easingCurve: StockCurve.ElasticOut
                },
                ScaleTransition {
                    toX: 1.1
                    duration: 600
                    easingCurve: StockCurve.BounceInOut

                },
                ScaleTransition {
                    toY: 1.2
                    duration: 400
                    easingCurve: StockCurve.BounceInOut
                },
                ScaleTransition {
                    toX: 0.8
                    delay: 600
                    duration: 400
                    easingCurve: StockCurve.BounceInOut

                },
                ScaleTransition {
                    toY: 0.9
                    delay: 400
                    duration: 400
                    easingCurve: StockCurve.BounceInOut

                },
                ScaleTransition {
                    toX: 1.0
                    delay: 600 + 400
                    duration: 400
                    easingCurve: StockCurve.BounceInOut

                },
                ScaleTransition {
                    toY: 1.0
                    delay: 400 + 400
                    duration: 400
                    easingCurve: StockCurve.BounceInOut

                }
            ]
        },
        TranslateTransition {
            id: bounceStraight
            duration: 10
            toX: bounceEndX
            toY: bounceEndY

            easingCurve: StockCurve.Linear
            onStarted: {
                bounceStartY = translationY;
                bounceStartX = translationX;
            }
            onEnded: {
                bounceEnded();
            }
        },
        SequentialAnimation {
            id: bounce
            TranslateTransition {
                id: bounceToWall
                easingCurve: StockCurve.Linear
            }
            TranslateTransition {
                id: bounceFromWall
                toX: bounceEndX
                toY: bounceEndY
                
                easingCurve: StockCurve.Linear
            }
            onStarted: {
                bounceStartY = bounceToWall.toY;
                bounceStartX = bounceToWall.toX;
            }

            onEnded: {
                bounceEnded();
            }
        },
        ParallelAnimation {
            id: death
            TranslateTransition {
             	id: deathTranslation   
                easingCurve: StockCurve.Linear
                duration: paddleToWallDuration
            }
            
            FadeTransition {
                toOpacity: 0
                duration: deathTranslation.duration
                easingCurve: StockCurve.QuadraticOut
            }
            
            onEnded: {
                wallHit();
                newballAnim.play();
            }
        },
        SequentialAnimation {
            id: newballAnim
            // Set up new ball below
            TranslateTransition {
                id: newball
                easingCurve: StockCurve.Linear
                duration: 10
                toY: (screenHeight - ballSize) / 2
                toX: (screenWidth - ballSize) / 2
            }

            ScaleTransition {
                toX: 0.0
                toY: 0.0
                duration: 10
            }

            FadeTransition {
                toOpacity: 1
                duration: 10
            }
            
            ScaleTransition {
                id: scaleAnimation2
                toX: 1.0
                toY: 1.0
                delay: 700
                duration: 500
                easingCurve: StockCurve.BounceOut
            }

            // Dummy delay before serve
            FadeTransition {
                fromOpacity: 1
                toOpacity: 1
                delay: 400
            }

			onStarted: {
                newBall();
       		}
            onEnded: {
                translationY = newball.toY
                bounceDuration = 2000
                newBallEnded();
            }
        }
    ]

    function stopTheBall() {
        death.stop();
        bounce.stop();
        bounceStraight.stop();
        newballAnim.stop();
    }    
    
    function resetTheBall() {
        var xoffset = 0;
        
        if (translationX > (screenWidth / 2)) {
            deathTranslation.toX = screenWidth - (40 - 5);
            xoffset = screenWidth - translationX - 10;
        } else {
            deathTranslation.toX = -5;
            xoffset = translationX - 10;
        }
        
        // The total movement from the missed paddle is xoffset px, yoffset correspond to the additional movement in y.
        var directionAngle = (Math.atan(Math.abs(bounceEndX - bounceStartX) / (bounceEndY - bounceStartY)));
        var yoffset = xoffset / Math.tan(directionAngle);
        deathTranslation.toY = bounceEndY + yoffset;
        
        paddleToWallDuration = xoffset * (bounceDuration / Math.abs(bounceEndX - bounceStartX));
        
        death.play();
    }
    
    function calculateBallMovement() {
                
        // Make the ball go faster and faster.
        bounceDuration = bounceDuration - 100;

        if(translationY > screenHeight / 2 - 100 && translationY < screenHeight / 2 + 100) {
            // Straight movement
            bounceStraight.duration = bounceDuration;
            var yEnd = Math.floor(Math.random() * (screenHeight - 0 - 200)) + 100;
            var xEnd = getXAtCircleSegment(yEnd);
            
            bounceEndX = xEnd;
            bounceEndY = yEnd ;
            
            bounceStraight.play();
        } else {
            //Distance from screen to surrounding box
            var boxOffset = 0;
            
            // 1. Find the place where the ball with hit the side wall in x-direction.
            //    A random place bewteen the middle and a quarter of the lenght of the field. 
            var xWallHit = Math.floor(Math.random() * screenWidth/8) + screenWidth/4;
            if (translationX > screenWidth / 2) {
                xWallHit = screenWidth - xWallHit
            }
            
            var xWallDistance = xWallHit;
            if(translationX > screenWidth / 2) {
                xWallDistance = screenWidth - xWallHit;
            }
            
            // 2. Set up the y position of the wall, which side did the ball hit.
            var yWallHit = (translationY > screenHeight/2) ? screenHeight - boxOffset - ballSize: boxOffset;
            
            var yWallDistance = (translationY - boxOffset);
            if(translationY > screenHeight / 2) {
                yWallDistance = screenHeight - (translationY - boxOffset);
            }
            
            // 3. Calculate the angle, so that we can calculate the reflected ball path.
            var reflectionAngle = Math.atan(yWallDistance/xWallDistance);
            
            // 4. Calculate the end y position using the reflection angle, this will not 
            // be perfect since we will hit a circle segment but is close enough
            var yEnd = Math.abs(screenWidth - xWallDistance) * Math.sin(reflectionAngle);
            var xEnd = getXAtCircleSegment(yEnd);

            bounceToWall.toX = xWallHit;
            bounceToWall.toY = yWallHit;
            
            bounceEndX = xEnd;
            bounceEndY = yEnd ;
            
            bounceToWall.duration = bounceDuration * (xWallDistance/screenWidth);
            bounceFromWall.duration = bounceDuration * (1 - (xWallDistance/screenWidth));
            
            bounce.play();
        }
    
        wobble.play();
    }
    
    function getXAtCircleSegment(y) {
        var yPos = (y + ballSize/2);
        var xPos = Math.sqrt(Math.pow(circleRadius, 2) - Math.pow(yPos - circleOffsetY, 2)) + circleOffsetX;
        var xEnd = 0;
        
        if(translationX == (screenWidth - ballSize) / 2) {
          // Flip a coin for the serve
          var coinFlip = Math.random();
          if(coinFlip > 0.5){
              xEnd = xPos + paddleWidth; //Size of paddle and paddle translation  
          } else {
              xEnd = screenWidth - xPos - (paddleWidth + ballSize); //Size of paddle, paddle translation and ball  
          }
        } else if (translationX > screenWidth / 2) {
            xEnd = xPos + paddleWidth; //Size of paddle and paddle translation
        } else {
            xEnd = screenWidth - xPos - (paddleWidth + ballSize); //Size of paddle, paddle translation and ball
        }

        return xEnd;
    }
}