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

// This is the info page with some information about the application and a bit of decorations.

NavigationPane {
    id: navigation
    property int cloudBottomPaddingLandscape: 80
    property int cloudBottomPaddingDefault: 180
    
    Page {
        
        Container {
            background: backgroundPaint.imagePaint
            
            attachedObjects: [
                ImagePaintDefinition {
                    id: backgroundPaint
                    imageSource: "asset:///images/info/background.png"
                }
            ]
            
            layout: DockLayout {
            }
            
            // Cloud image
            Container {
                id: cloud
                leftPadding: 40
                bottomPadding: cloudBottomPaddingDefault
                verticalAlignment: VerticalAlignment.Bottom
                horizontalAlignment: HorizontalAlignment.Left
                
                ImageView {
                    imageSource: "asset:///images/info/cloud.png"
                }
            }
            // Sun image
            Container {
                rightPadding: 50
                topPadding: 70
                verticalAlignment: VerticalAlignment.Top
                horizontalAlignment: HorizontalAlignment.Right
                
                ImageView {
                    imageSource: "asset:///images/info/sun.png"
                }
            }
            // Descriptive Text
            Container {
                bottomPadding: 180
                horizontalAlignment: HorizontalAlignment.Center
                verticalAlignment: VerticalAlignment.Center
                
                TextArea {
                    text: "Welcome to the weather history. This little app shows you the recorded weather data from cities across the world."
                    enabled: false
                    editable: false
                    preferredWidth: 650                    
                    textStyle.base: titleTextItalicWhite.base
                }
            }
            // Footer information
            Container {
                rightPadding: 30
                bottomPadding: 30
                verticalAlignment: VerticalAlignment.Bottom
                horizontalAlignment: HorizontalAlignment.Right
                
                Label {
                    text: "Cascades sample app 2013 running on unknown screen."
                    textStyle.base: smallTextWhite.style
                }
            }
        }

        // The info page has an additional action, that is placed in the overflow menu to the right.
		// When the user pressed the "More info", they are directed to the MoreInfoPage.qml.
        actions: [
            ActionItem {
                title: "More Info"
                imageSource: "asset:///images/menuicons/icon_continents.png"
                
                onTriggered: {
                    var moreInfoPage = moreInfoPageDefinition.createObject();
                    navigation.push(moreInfoPage);
                }
            }
        ]
    }
    
    // Orientation related code: in landscape we want to change the cloud's bottomPadding
    /**
     * @param orientation Must be a value of UIOrientation
     */
    function handleOrientationChange(orientation) {
        if (orientation == UIOrientation.Landscape) {
            cloud.bottomPadding = cloudBottomPaddingLandscape;
        }
        else {
            cloud.bottomPadding = cloudBottomPaddingDefault;
        }
    }
    
    onCreationCompleted: {
        handleOrientationChange(OrientationSupport.orientation); // pass the current orientation
    }
        
    attachedObjects: [
        ComponentDefinition {
            id: moreInfoPageDefinition
            source: "MoreInfoPage.qml"
        }, 
        OrientationHandler {
            onOrientationAboutToChange: {
                handleOrientationChange(orientation); // pass the new orientation
            }
        }
    ]
}
