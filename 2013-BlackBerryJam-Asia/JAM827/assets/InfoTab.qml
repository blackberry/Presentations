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

// This is the info page with some information about the application and a bit of decorations.
Page {
    property int cloudBottomPaddingLandscape: 80
    property int cloudBottomPaddingDefault: 180

    Container {
        background: backgroundPaint.imagePaint

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
                text: qsTr("Welcome to the weather history. This little app shows you the recorded weather data from cities across the world.") + Retranslate.onLanguageChanged
                enabled: false
                editable: false
                preferredWidth: 650
                textStyle.base: SystemDefaults.TextStyles.TitleText
                textStyle.fontStyle: FontStyle.Italic
            }
        }

        // Footer information
        Container {
            rightPadding: 30
            bottomPadding: 30
            verticalAlignment: VerticalAlignment.Bottom
            horizontalAlignment: HorizontalAlignment.Right

            Label {
                text: qsTr("Cascades sample app 2013.") + Retranslate.onLanguageChanged
                textStyle.base: SystemDefaults.TextStyles.SmallText
            }
        }

        attachedObjects: [
            ImagePaintDefinition {
                id: backgroundPaint
                imageSource: "asset:///images/info/background.png"
            }
        ]
    }

    // Orientation related code: in landscape we want to change the cloud's bottomPadding
    function handleOrientationChange(orientation) {
        if (orientation == UIOrientation.Landscape) {
            cloud.bottomPadding = cloudBottomPaddingLandscape;
        } else {
            cloud.bottomPadding = cloudBottomPaddingDefault;
        }
    }

    onCreationCompleted: {
        orientationHandler.orientationAboutToChange.connect(handleOrientationChange);
    }

    attachedObjects: [
        OrientationHandler {
            id: orientationHandler
        }
    ]

}