/* Copyright (c) 2013 BlackBerry Limited.
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
    layout: DockLayout {}
    verticalAlignment: VerticalAlignment.Fill
    horizontalAlignment: HorizontalAlignment.Fill
    preferredHeight: 150
    rightPadding: 30
    leftPadding: 30
    
    property variant backgroundColor: Color.Black
    
    onOpacityChanged: {
        if (opacity == 1) {
            showShineAnimation.play();
            showSlideAnimation.play();
        }
        else {
            hideSlideAnimation.play();
        }
    }
    
    Container {
        background: backgroundColor
        verticalAlignment: VerticalAlignment.Top
        horizontalAlignment: HorizontalAlignment.Fill
        translationY: 30
        maxHeight: 120
    
        Container {       
            minHeight: 725
            minWidth: 594     
            opacity: 0
             
            ImageView {
                imageSource: "asset:///images/shine.png"         
            }
            
            animations: [
                ParallelAnimation {
                    id: showShineAnimation
                    TranslateTransition {
                        fromY: 500
                        toY: -500
                        easingCurve: StockCurve.ExponentialOut
                        duration: 800       
                    }   
                    TranslateTransition {
                        fromX: 0
                        toX: -50
                        easingCurve: StockCurve.ExponentialOut
                        duration: 800       
                    }   
                    FadeTransition {
                        toOpacity: 0.8
                        easingCurve: StockCurve.ExponentialOut
                        duration: 1200
                    }
                }
            ]
        }
        
        animations: [
            TranslateTransition {
                id: showSlideAnimation
                fromY: 30
                toY: 0
                easingCurve: StockCurve.ExponentialOut
                duration: 800       
            },
            TranslateTransition {
                id: hideSlideAnimation
                fromY: 0
                toY: 30
                easingCurve: StockCurve.ExponentialOut
                duration: 250       
            }   
        ]

    }
}
