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
        // A more flexible way of handling background images is to paint the background using and
        // image paint. The paint will paint the entire Container background and a tiled image can be used.
        // Image Paints definitions are done as attached objects.
        background: backgroundPaint.imagePaint
        
        // An Example of a QML component, a custom title bar
        CustomTitleBar {
            title: "Highscore"
        }

		HighScoreListTitle {
		}

        // Add ListView, create a QML component in a seperate file and use it as list item
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
    
    attachedObjects: ImagePaintDefinition {
        id: backgroundPaint
        imageSource: "images/bg.png"
    }
}

