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

// A custom item used in the list on the WeatherPage for Today item
Container {
    id: weatherItem
    property string temphi: ListItemData.temphi
    property string templo: ListItemData.templo

    // The date is presented above the grphical illustration of the weather.
    Header {
        title: ListItemData.date.toDateString()
    }

    Container {
        horizontalAlignment: HorizontalAlignment.Fill
                
        layout: DockLayout {
        }

        // Icon image filler
        ImageView {
            horizontalAlignment: HorizontalAlignment.Left
            imageSource: "asset:///images/icons/" + images_sub_path + "/" + ListItemData.icon + "_fill.png"
        }
        
        // Icon image
        ImageView {
            horizontalAlignment: HorizontalAlignment.Right
            imageSource: "asset:///images/icons/" + images_sub_path + "/" + ListItemData.icon + ".png"
        }

        Container {
            leftPadding: 77
            verticalAlignment: VerticalAlignment.Center
            horizontalAlignment: HorizontalAlignment.Left
            Label {
                multiline: true
                text: "<html><span style='color:white;font-size:large'><div>hi  <span style='font-size:xx-large'>" + temphi + "\u00B0</span></div><div>lo   " + templo + "\u00B0</div></span></html>"
            }
        } // Container
    } // Container
    
    // Orientation related code: in landscape we want to display smaller items
    property string images_sub_path: "big"
    
    /**
     * @param orientation Must be a value of UIOrientation
     */
    function handleOrientationChange(orientation) {
        if (orientation == UIOrientation.Landscape) {
            images_sub_path = "small";
        }
        else {
            images_sub_path = "big";
        }
    }
        
    onCreationCompleted: {
        handleOrientationChange(OrientationSupport.orientation); // pass the current orientation
    }

    attachedObjects: [
        OrientationHandler {
            onOrientationAboutToChange: {
                handleOrientationChange(orientation); // pass the new orientation
            }
        }
    ]
}// Container