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

		// The heart image that we will implicitly animate.
        ImageView {
            id: heartImage
            imageSource: "asset:///heart.png"
            horizontalAlignment: HorizontalAlignment.Center
            verticalAlignment: VerticalAlignment.Center
        }

		// A touch signal handler that we use it to trigger a rotation change.
        onTouch: {
            if (event.touchType == TouchType.Down) {
                // Change the rotation by 90 degrees, this will trigger an implicit animation.
                heartImage.rotationZ = heartImage.rotationZ + 90
            }
        }
    }
}
