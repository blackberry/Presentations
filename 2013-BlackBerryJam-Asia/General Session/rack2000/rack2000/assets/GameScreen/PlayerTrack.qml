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
    property string trackImageSource
    property string deathImageSource
    property int deathDuration: 0
    layout: DockLayout {
    }
        
    ImageView {
        imageSource: trackImageSource
    }
    
    ImageView {
        opacity: 0
        imageSource: deathImageSource
        
        animations: [
            SequentialAnimation {
                id: deathCircle
                
                FadeTransition {
                    fromOpacity: 0
                    toOpacity: 1
                    duration: deathDuration/2
                    easingCurve: StockCurve.QuadraticOut
                }
                FadeTransition {
                    toOpacity: 0
                    duration: deathDuration/2
                }
            }
        ]
    }
    
    function playDeathCircle() {
        deathCircle.play();
    }
}
