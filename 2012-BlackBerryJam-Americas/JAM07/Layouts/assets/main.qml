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
    attachedObjects: [
        DockLayout {
            id: dock
        },
        StackLayout {
            id: stack
            
        }
    ]
    Container {
        id: mainContainer
        layout: stack
        ImageView {
            id: cow1
            imageSource: "asset:///image.png"
            horizontalAlignment: HorizontalAlignment.Center
            verticalAlignment: VerticalAlignment.Center
        }
        ImageView {
            id: cow2
            imageSource: "asset:///image.png"
            horizontalAlignment: HorizontalAlignment.Left
            verticalAlignment: VerticalAlignment.Center
        }
        ImageView {
            id: cow3
            imageSource: "asset:///image.png"
            horizontalAlignment: HorizontalAlignment.Center
            verticalAlignment: VerticalAlignment.Bottom
        }
        ImageView {
            id: cow4
            imageSource: "asset:///image.png"
            horizontalAlignment: HorizontalAlignment.Right
            verticalAlignment: VerticalAlignment.Center
        }
        ImageView {
            id: cow5
            imageSource: "asset:///image.png"
            horizontalAlignment: HorizontalAlignment.Center
            verticalAlignment: VerticalAlignment.Top
        }
        onTouch: {
            if (event.isUp()) {
                if (mainContainer.layout == dock) {
                    mainContainer.layout = stack
                    cow1.horizontalAlignment = HorizontalAlignment.Center
                    cow2.horizontalAlignment = HorizontalAlignment.Center
                    cow3.horizontalAlignment = HorizontalAlignment.Center
                    cow4.horizontalAlignment = HorizontalAlignment.Center
                    cow5.horizontalAlignment = HorizontalAlignment.Center
                } else {
                    mainContainer.layout = dock
                    cow1.horizontalAlignment = HorizontalAlignment.Center
                    cow2.horizontalAlignment = HorizontalAlignment.Left
                    cow3.horizontalAlignment = HorizontalAlignment.Center
                    cow4.horizontalAlignment = HorizontalAlignment.Right
                    cow5.horizontalAlignment = HorizontalAlignment.Center
                }
            }
        }
    }
}
