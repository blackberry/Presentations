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
    id: counterIcon
    property string plateString: "player1"
    property alias color: eyelid.background
    property string type: "eye"
    property bool running: false;

    layout: DockLayout {
    }

    ImageView {
        imageSource: "asset:///images/cm-plate-" + plateString + ".png"
    }

    ImageView {
        imageSource: "asset:///images/cm-" + type + ".png"
        horizontalAlignment: HorizontalAlignment.Center
        verticalAlignment: VerticalAlignment.Center

        animations: [
            RotateTransition {
                id: fanSpin
                repeatCount: AnimationRepeatCount.Forever
                fromAngleZ: 0
                toAngleZ: 360
                duration: 1000
                easingCurve: StockCurve.Linear
            }
        ]
    }

    Container {
        id: eyelid
        preferredWidth: 40
        preferredHeight: 40
        translationY: -40
        horizontalAlignment: HorizontalAlignment.Center

        animations: [
            SequentialAnimation {
                id: eyeblink
                repeatCount: AnimationRepeatCount.Forever
                TranslateTransition {
                    fromY: -30
                    toY: 0
                    duration: 70
                    delay: 800
                }
                TranslateTransition {
                    toY: -30
                    duration: 70
                }
                TranslateTransition {
                    fromY: -30
                    toY: 0
                    duration: 70
                    delay: 100
                }
                TranslateTransition {
                    toY: -30
                    duration: 70
                }
            }
        ]
    }
    
    function trigger() {
        opacity = 1.0;
        if (type == "eye") {
            eyeblink.play();
            eyelid.visible = true;
        } else if (type == "fan") {
            fanSpin.play()
        }
        running = true;
    }

    function stop() {
        opacity = 0.3;
        eyeblink.stop();
        eyelid.translationY = -40
        fanSpin.stop();
        running = false;
    }
}
