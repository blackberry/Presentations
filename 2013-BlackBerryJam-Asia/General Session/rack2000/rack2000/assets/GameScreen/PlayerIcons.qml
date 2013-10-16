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
    id: playerText
    property variant color;
    property string playerString;
    property bool rightToLeft: true;
    property string preparedIcon: "";
    property string preparedText: "";
    
    signal playerCounterMeasureDone();
    
    opacity: 0
    
    layout: StackLayout {
        orientation: rightToLeft?LayoutOrientation.RightToLeft:LayoutOrientation.LeftToRight
    }
    
    layoutProperties: StackLayoutProperties {
        spaceQuota: 1
    }
    
    CounterMeasureIcon {
        id: eye
        type: "eye"
        plateString: playerString
        verticalAlignment: VerticalAlignment.Center
        leftMargin: 20
        rightMargin: 20
        color: playerText.color
        opacity: 0.3
    }	
    
    CounterMeasureIcon {
        id: fan
        type: "fan"
        plateString: playerString
        verticalAlignment: VerticalAlignment.Center
        leftMargin: 20
        rightMargin: 20
        opacity: 0.3
    }
    
    CounterMeasureIcon {
     id: reverse
     type: "reverse"
     plateString: playerString
     verticalAlignment: VerticalAlignment.Center
     leftMargin: 20
     rightMargin: 20
     opacity: 0.3
     }
    
    CounterMeasureText {
        id: counterMeasureText           
        textAlign: (rightToLeft) ? TextAlign.Right : TextAlign.Left
        verticalAlignment: VerticalAlignment.Center
        text: preparedText;
        
        layoutProperties: StackLayoutProperties {
            spaceQuota: 1
        }
        
        onCounterMeasureTextDone: {
            playerCounterMeasureDone();
        }
    }
    
    // Returns false if the counter is already running.
    function setUpCounterMeasure(newCounterData) {
        var result = false;
        
        if(newCounterData.icon == "fan") {
            result = !fan.running;
        } else if(newCounterData.icon == "eye"){
            result = !eye.running;
        } else if(newCounterData.icon == "reverse"){
            result = !reverse.running;
        }
        
        if(result) {            
            preparedIcon = newCounterData.icon;
            preparedText = newCounterData.text;
        }
        
        return result;
    }
    
    function activateCounterMeasure(player1) {
        counterMeasureText.showText(player1);
        if(preparedIcon == "fan") {
            fan.trigger();
        } else if(preparedIcon == "eye") {
            eye.trigger();
        } else if(preparedIcon == "reverse") {
            reverse.trigger();
        }
    }
    
    function deActivateCounterMeasure(){
        fan.stop();
        eye.stop();
        reverse.stop();
    }
}
