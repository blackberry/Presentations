/* Copyright (c) 2012 Research In Motion Limited.
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
import bb.cascades 1.0

// creates one page with a label

Page {
    Container {
        layout: DockLayout {
        }
        ImageView {
            horizontalAlignment: HorizontalAlignment.Center
            verticalAlignment: VerticalAlignment.Center
            
            imageSource: "asset:///image.png"
            animations: [
                SequentialAnimation {
                    id: turner
                    animations: [
                        RotateTransition {
                            fromAngleZ: 0
                            toAngleZ: 90
                            duration: 1000
                        },
                        RotateTransition {
                            fromAngleZ: 90
                            toAngleZ: 0
                            duration: 1000
                        }
                    ]
                    onEnded: {
                        turner.play();
                    }
                }
            ]
            onCreationCompleted: {
                turner.play();
            }
        }
    }
}
