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
    // An example of a custom titlebar as a Custom QML component.
    titleBar: CustomTitleBar {
        customTitle: "Highscore"
    }

    Container {
        layout: DockLayout {
        }
        
        ImageView {
            imageSource: "asset:///images/bg.png"
            scalingMethod: ScalingMethod.AspectFill
        }
        
        Container {            
            HighScoreListTitle {
            }

            // 1. Add a list ListView.
            // 2. Specify the datamodel property to a XmlDataModel with "highscoredb/highscore.xml" as source
            // 3. Create a QML component HighScoreItem.qml in a seperate file and use it as a ListItemComponent.
            // 4. Try to mimick the look of items in the presentation as closely as you can.
            ListView {
                dataModel: XmlDataModel {
                    source: "highscoredb/highscore.xml"
                }
                listItemComponents: [
                    ListItemComponent {
                        type: "item"
                        HighScoreItem {
                        }
                    }
                ]
            }
        }
    }
}
