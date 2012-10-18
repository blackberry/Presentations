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
    id: page
    property string theSound
    property alias theImagePath: imageView.imageSource
    Container {
        layout: StackLayout {
        }
        horizontalAlignment: HorizontalAlignment.Fill
        ImageView {
            id: imageView
            imageSource: "asset:///images/drums.png"
            horizontalAlignment: HorizontalAlignment.Center
        }
        ImageButton {
            topMargin: 90
            defaultImageSource: "asset:///images/big_play_regular.png"
            pressedImageSource: "asset:///images/big_play_pressed.png"
            onClicked: {
                soundpartyApp.playSound(theSound);
            }
            horizontalAlignment: HorizontalAlignment.Center
        }
    }
}
