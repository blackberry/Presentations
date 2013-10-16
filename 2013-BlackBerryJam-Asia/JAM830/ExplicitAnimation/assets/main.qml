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

Page {
    Container {
        background: Color.create("#f1e3c0")
        layout: DockLayout {
        }

        // The heart image that we will explicitly animate.        
        ImageView {
            id: heartImage
            imageSource: "asset:///heart.png"
            horizontalAlignment: HorizontalAlignment.Center
            verticalAlignment: VerticalAlignment.Center
            
            animations: [
                // To make the heart pound we set up a sequential animation. 
                SequentialAnimation {
                    id: heartAnim
                    
                    // Once started the animation will continue forever (or until explicitly stopped).
                    repeatCount: AnimationRepeatCount.Forever
                    
                    // First the heart is scaled up to 1.3 times its original size.
                    ScaleTransition {
                        toX: 1.3
                        toY: 1.3
                        duration: 400 
                    }
                    
                    // When the scaling up is done, this animation will scale it back to 1.0
                    // again, note that this is a fast animation and that a non-default easing is used.
                    ScaleTransition {
                        toX: 1.0
                        toY: 1.0
                        duration: 200
                        easingCurve: StockCurve.QuadraticOut       
                    }
                }
            ]
        }
        
        onTouch: {
            // If the heart is touched and the animation is not running we start the animation.
            if(heartAnim.state != AnimationState.Playing) {
                heartAnim.play();   
            }
        }
    }
}
