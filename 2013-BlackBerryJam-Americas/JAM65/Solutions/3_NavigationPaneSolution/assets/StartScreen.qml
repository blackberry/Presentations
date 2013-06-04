/* Copyright (c) 2013 Research In Motion Limited.
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

        ImageView {
            imageSource: "asset:///images/bg.png"
        }

        ImageView {
            imageSource: "asset:///images/logo.png"
        }

        Container {
            verticalAlignment: VerticalAlignment.Center
            horizontalAlignment: HorizontalAlignment.Center
            topPadding: 230

            Button {
                text: "Start"
            }
            Button {
                text: "Settings"
            }
            Button {
                text: "Highscore"
                // Add a singal handler for navigation here
                onClicked: {
                    var page = highScoreScreen.createObject()
                    nav.push(page);
                }
            }
            Button {
                text: "About"
            }
        }

        ImageView {
            verticalAlignment: VerticalAlignment.Bottom
            imageSource: "asset:///images/bg_turtle.png"
        }
    }
}
