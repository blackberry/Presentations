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
        
        Container {
            layout: StackLayout {
            }
            horizontalAlignment: HorizontalAlignment.Fill
            verticalAlignment: VerticalAlignment.Fill
            
            Container {
                leftPadding: 20
                rightPadding: 20
                topPadding: 20
                TextArea {
                    editable: false
                    text: {
                        "Z10:\n-Tabs shown on action bar\n-Winelist selection via segmented control\n-Short listItem descriptions-More real-estate\n\nQ10:\n-Tabs hidden\n-Winelist selection via action bar\n-Short listItem descriptions\n\nZ30:\n-Tabs shown on action bar\n-Winelist selection via segmented control\n-Full listItem descriptions-Mo real-estate\n\n\nEverything Should Be Made as Simple as Possible, But Not Simpler\n--Albert Einstein"
                    }
                }
            }
        }
        attachedObjects: [
            ImagePaintDefinition {
                id: mapsBackground
                imageSource: "asset:///images/wood_tile.amd"
                repeatPattern: RepeatPattern.XY
            }
        ]
    }

}
