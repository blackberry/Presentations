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
    imageSource: "asset:///images/centrum.png"

    animations: [
        SequentialAnimation {
            id: showAnim

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
                duration: 500
                easingCurve: StockCurve.BounceOut
            }
            FadeTransition {
                toOpacity: 0
                delay: 700
                duration: 150
            }
        }
    ]
    function show() {
        showAnim.play()
    }
}
