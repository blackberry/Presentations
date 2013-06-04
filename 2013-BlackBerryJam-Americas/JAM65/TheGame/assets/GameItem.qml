/*
 * Copyright (c) 2011, 2012 Research In Motion Limited.
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

Container {
    ImageView {
        id: imageViewID
        imageSource: correctImage ()
        function correctImage () {
            var imageSource = ""
            if (player) {
                imageSource = "asset:///images/block_main.png"
            } else {
                if (vertical) {
                    imageSource = "asset:///images/block_1x" + cellCount + ".png"
                } else {
                    imageSource = "asset:///images/block_" + cellCount + "x1.png"
                }
            }
            return imageSource
        }
    }
    minWidth: vertical ? 120 : cellCount * 120
    maxWidth: vertical ? 120 : cellCount * 120
    minHeight: vertical ? cellCount * 120 : 120
    maxHeight: vertical ? cellCount * 120 : 120
}
