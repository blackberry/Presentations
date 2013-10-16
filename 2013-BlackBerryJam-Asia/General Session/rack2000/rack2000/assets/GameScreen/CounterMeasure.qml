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
    property bool isAvailable: false
    id: countermeassure
    opacity: 0
    property int animPadding: 200
    property variant hideCurve: StockCurve.CubicIn
    property bool lastLeft: false;
    signal counterMeasureDone(bool player1);
    
    Label {
        text: qsTr("Countermeassure!")
        horizontalAlignment: HorizontalAlignment.Center
        multiline: true
        
        textStyle {
            fontSize: FontSize.PointValue
            color: Color.create("#ffffff")
            fontWeight: FontWeight.W100
            fontSizeValue: 20
        }
    }

    animations: [
        ScaleTransition {
            id: showAnim
            fromX: 0
            fromY: 0
            toX: 1
            toY: 1
            duration: 400
            easingCurve: StockCurve.BounceOut
            onStarted: {
                countermeassure.isAvailable = true;
                countermeassure.opacity = 1.0
            }
        },
        ScaleTransition {
            id: dismissAnim
            toX: 0
            toY: 0
            duration: 400
            easingCurve: StockCurve.BounceIn
            
            onEnded: {
                countermeassure.opacity = 0.0
                countermeassure.translationX = 0;
                countermeassure.translationY = 0
            }
        },
        ParallelAnimation {
            id: hideAnim
            
            ScaleTransition {
                fromX: 1
                fromY: 1
                toX: 0.1
                toY: 0.1
                duration: 3 * animPadding
                easingCurve: hideCurve
            }
            TranslateTransition {
            	toY: -320 // Device dependent
                duration: 2 * animPadding
                delay: 1 * animPadding
                easingCurve: hideCurve
            }
            TranslateTransition {
                id: hideCounterXpos
                toX: 100 // Device dependent
                duration: 2 * animPadding
                delay: 1 * animPadding
                easingCurve: hideCurve              
            }
            FadeTransition {
                toOpacity: 0
                duration: 1 * animPadding
                delay: 3 *animPadding
            }

            onStarted: {
                countermeassure.isAvailable = false;
            }
            
            onEnded: {
                countermeassure.opacity = 0
                countermeassure.translationX = 0;
                countermeassure.translationY = 0
                counterMeasureDone(lastLeft)
            }
        }
    ]

    function show(){
        showAnim.play()
    }
        
    function  hide(player1) {
        if(player1) {
            hideCounterXpos.toX = 100
        } else {
            hideCounterXpos.toX = -100
        }
        hideAnim.play();
        lastLeft = player1;
    }
    
    function dismiss() {
        
        if(hideAnim.state == AnimationState.Stopped || 
            hideAnim.state == AnimationState.Ended){
            dismissAnim.play();
        }
    }
}
