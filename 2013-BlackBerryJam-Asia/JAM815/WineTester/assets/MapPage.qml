/*
 * Copyright 2013 BlackBerry Limited.
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
import "actionbars"

Page {
	Container {
        background: mapsBackground.imagePaint
        
        WineActionBarContent {
        }

        ScrollView {
            id: scrollView
            horizontalAlignment: HorizontalAlignment.Fill
            verticalAlignment: VerticalAlignment.Fill
            scrollViewProperties {
                minContentScale: 1
                initialScalingMethod: ScalingMethod.AspectFit
                scrollMode: ScrollMode.Both
                pinchToZoomEnabled: true
            }
            Container {
                layout: DockLayout {
                }
                layoutProperties: StackLayoutProperties {
                    spaceQuota: 1
                }
                maxHeight: _features.getDeviceHeight() - 120 //allow for action bar
                maxWidth: _features.getDeviceWidth()
                minWidth: _features.getDeviceWidth()
                horizontalAlignment: HorizontalAlignment.Fill
                verticalAlignment: VerticalAlignment.Fill
                ImageView {
                    id: imageView
                    horizontalAlignment: HorizontalAlignment.Center
                    verticalAlignment: VerticalAlignment.Center
                    imageSource: "asset:///images/Ottawa_map.png"
                }
            }
        }
        
        attachedObjects: [
            ImagePaintDefinition {
                id: mapsBackground
                imageSource: "asset:///images/grey_tile.amd"
                repeatPattern: RepeatPattern.XY
            }
        ]
    }
}
