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
    
    // By creating an alias to the easing curves they can be set from the 
    // main.qml file as the selection changes.
    property alias showCurve: showOverlayAnim.easingCurve
    property alias hideCurve: hideOverlayAnim.easingCurve
    
    // Offset properties and placement of the overlay.
    property int overlayHeight: overlayImage.preferredHeight
    property int overlayOffset: 150
    property int initialOffset: overlayHeight - overlayOffset
    
    translationY: initialOffset
    
    layout: DockLayout {
    }
    
    // The image that will be animated.
    ImageView {
        id: overlayImage
        imageSource: "asset:///images/ovelay.png"
        preferredHeight: 820    // Can be retrieved via a LayoutUpdateHandler
    }
    
    animations: [
        TranslateTransition {
            id: showOverlayAnim
            toY: 0
            duration: 800
            
            // This easing will be altered via property aliasing, see above.
            easingCurve: StockCurve.QuarticOut
        },
        TranslateTransition {
            id: hideOverlayAnim
            toY: initialOffset
            duration: 800
            
            // This easing will be altered via property aliasing, see above.
            easingCurve: StockCurve.Linear
        }
    ]
    
    onTouch: {
        // Trigger the hide/show animation depending on the position of the image.
        if (event.touchType == TouchType.Down) {
            if (translationY > 0) {
                showOverlayAnim.play();
            } else {
                hideOverlayAnim.play();
            }
        }
    }
}