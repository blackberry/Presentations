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

Page {
    Container {
        layout: DockLayout {
        }

        background: backgroundPaint.imagePaint
        attachedObjects: [
            ImagePaintDefinition {
                id: backgroundPaint
                imageSource: "asset:///images/background.png"
            }
        ]

        Container {

            // Scale up and rotate on Down and reset on Up. 
            onTouch: {
                if (event.isDown()) {
                    scaleX = 2;
                    scaleY = 2;
                    rotationZ = 45;
                } else if (event.isUp()) {
                    scaleX = 1;
                    scaleY = 1;
                    rotationZ = 0;
                }
            }

            horizontalAlignment: HorizontalAlignment.Center
            verticalAlignment: VerticalAlignment.Center
            layout: DockLayout {
            }

            ImageView {
                imageSource: "asset:///images/bubble.png"
            }

            Label {
                text: qsTr("Hello World")
                verticalAlignment: VerticalAlignment.Center
                horizontalAlignment: HorizontalAlignment.Center
                textStyle {
                    base: SystemDefaults.TextStyles.BigText
                    fontWeight: FontWeight.Bold
                    color: Color.create ("#ff5D5D5D")
                }
            }
        }
    }
} 
