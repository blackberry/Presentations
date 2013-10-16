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
    property alias text: counterText.text;
    property int textAlign: TextAlign.Left
    signal counterMeasureTextDone();
    
    Label {        
        id: counterText
        property int sign: 1
        horizontalAlignment: HorizontalAlignment.Fill
        visible: false;
        
        textStyle {
            color: Color.create("#ffffff")
            textAlign: textAlign
            fontWeight: FontWeight.W100
            fontSize: FontSize.PointValue
            fontSizeValue: 11
        }
        
        animations: [
            SequentialAnimation {
                id: showIt
                
                TranslateTransition {
                    fromX: -600 * counterText.sign
                    toX: 0
                    duration: 300
                    easingCurve: StockCurve.CubicOut                
                }
                TranslateTransition {
                    fromX: 0
                    toX: - 600 * counterText.sign
                    delay: 400                   
                    duration: 300
                    easingCurve: StockCurve.CircularIn
                }
                
                onEnded: {
                    counterMeasureDone();
                }           
            }
        ]
    }

	function showText(left) {
	    if(left) {
            counterText.sign = 1;
	    } else {
            counterText.sign = -1;
	    }
		showIt.play();
        counterText.visible = true;
    }
}