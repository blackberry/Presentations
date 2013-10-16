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

Container {
    id: player
    property alias playerImage: rocker.imageSource
    property int initialTranslation: 40
    property int position: 0;
    property int screenHeight: 720
    
    property bool isReady: false
    property bool isConnected: false
    property bool reverseCounterMeasure: false
    property bool allowDeformation: true

    property int score: 0
    property variant color
    property string playerString: "player1"
    property real oldY: 0.0
    property real oldAngle:0.0
    
    // Circle segment properties set from outside
    property real circleRadius
    property real circleOffsetX
    property real circleOffsetY


    preferredWidth: 43
    preferredHeight: 196            
    translationX: initialTranslation
    verticalAlignment: VerticalAlignment.Top
    horizontalAlignment: HorizontalAlignment.Left
    opacity: 0
    clipContentToBounds: false

    layout: DockLayout {
    }
    
    onIsConnectedChanged: {
        if(isConnected){
            opacity = 1;
        } else {
            opacity = 0;
        }
    }
    
    onPositionChanged: {
        if (reverseCounterMeasure) {
            translationY = screenHeight - position;
        } else {
            translationY = position;
        }
        
        // Introduce some restrictions to the position (needed in the math below)
        if (translationY > 720 - preferredHeight/2) {
            translationY = 720 - preferredHeight/2;
        }

        if (translationY < -preferredHeight/2) {
            translationY = -preferredHeight/2;
        }

		// Calculate the position on a displaced circle (the rocker track)
        var sign = (horizontalAlignment==HorizontalAlignment.Left)?1:-1
        var yPos = (translationY + preferredHeight/2);
        var xPos = Math.sqrt(Math.pow(circleRadius, 2) - Math.pow(yPos - circleOffsetY, 2)) + circleOffsetX 
        
        var angle = 90 - Math.acos(((yPos - circleOffsetY)/circleRadius)) * 180/ Math.PI
        
        rotationZ = sign * angle
        translationX = sign * xPos;
        oldAngle = rotationZ;
        
        if(reverseCounterMeasure){
            oldY = screenHeight - position;
        } else {
            oldY = position;
        }
    }
    
    ImageView {
        id: rocker
        horizontalAlignment: HorizontalAlignment.Fill
        verticalAlignment: VerticalAlignment.Fill
        
        animations: [
            ParallelAnimation {
                id: returnBallAnim
                SequentialAnimation {
                    TranslateTransition {
                        toX:  - 10 * (Math.abs(initialTranslation) / initialTranslation)
                        duration: 100
                        easingCurve: StockCurve.CubicOut
                        delay: 20
                    }
                    TranslateTransition {
                        toX: 0
                        duration: 100
                        easingCurve: StockCurve.CubicInOut
                    }
                }
                SequentialAnimation {
                    ScaleTransition {
                        toX: 0.9
                        toY: 1.1
                        duration: 100
                        easingCurve: StockCurve.CubicOut
                        delay: 20
                    }
                    ScaleTransition {
                        toX: 1
                        toY: 1
                        duration: 100
                        easingCurve: StockCurve.CubicInOut
                    }
                }
                onEnded: {
                    allowDeformation = true;
                }
            }
        ]
    }

    function hitTest(y){
        if (y > translationY && y < translationY + preferredHeight) {
            allowDeformation = false;            
            returnBallAnim.play()
            return true
        }
        return false
    }
}
