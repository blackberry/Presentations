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

// A container to display a full page message
Container {
    id: me
    property string text_: "Howdy!"
    property string message: "This is a message from your benefactors!"
    property bool error_: false
    property alias show: show
    property alias hide: hide

    layout: DockLayout { }
    background: backgroundPaint.imagePaint
    attachedObjects: [
        ImagePaintDefinition {
            id: backgroundPaint
            imageSource: "asset:///images/info/background.png"
        }
    ]
    animations: [
        FadeTransition {
            id: hide
            duration: 500
            easingCurve: StockCurve.CubicOut
            fromOpacity: 1.0
            toOpacity: 0.0
            onEnded: {
                me.setVisible(false);
            }
        }, 
        FadeTransition {
            id: show
            duration: 500
            easingCurve: StockCurve.CubicOut
            fromOpacity: 0.0
            toOpacity: 1.0
            onStarted: {
                me.setVisible(true);
            }
        }
    ]

    // The title and messages
    Container {
        leftPadding: 20
        rightPadding: 26
        verticalAlignment: VerticalAlignment.Fill
        horizontalAlignment: HorizontalAlignment.Fill
        layout: StackLayout {
        }
        
        // The title
        Container {        
            layoutProperties: StackLayoutProperties {
                spaceQuota: 1.0;
            }
            horizontalAlignment: HorizontalAlignment.Fill
            layout: DockLayout { }
            
            Label {    
                horizontalAlignment: HorizontalAlignment.Center
                verticalAlignment: VerticalAlignment.Bottom
                text: text_
                textStyle {
                    base: tsd.style
                }
                attachedObjects: [
                    TextStyleDefinition {
                        id: tsd
                        base: SystemDefaults.TextStyles.TitleText
                        textAlign: TextAlign.Center
                        color: Color.create("#fafafa")
                    }
                ]
            }
        }
        
        // The message
        Label {
            horizontalAlignment: HorizontalAlignment.Center
            verticalAlignment: VerticalAlignment.Top
            layoutProperties: StackLayoutProperties {
                spaceQuota: 2.0;
            }
            text: message
            multiline: true
            textStyle {
                base: defaultAlertLabelTextStyleDefinition.style
            }
            attachedObjects: [
                TextStyleDefinition {
                    id: defaultAlertLabelTextStyleDefinition
                    base: SystemDefaults.TextStyles.TitleText
                    textAlign: TextAlign.Center
                    fontWeight: FontWeight.W200
                    fontSize: FontSize.Small
                    color: Color.create("#fafafa")
                }
            ]
        }
    }
    
    // Background images (art)
    Container {
        verticalAlignment: VerticalAlignment.Fill
        horizontalAlignment: HorizontalAlignment.Fill
        topPadding: 100
        layout: DockLayout {
        }

        // Cloud image
        Container {
            leftPadding: 40
            bottomPadding: 180
            verticalAlignment: VerticalAlignment.Bottom
            horizontalAlignment: HorizontalAlignment.Left
            ImageView {
                imageSource: "asset:///images/info/cloud.png"
            }
        }

        // Sun image
        Container {
            rightPadding: 80
            verticalAlignment: VerticalAlignment.Top
            horizontalAlignment: HorizontalAlignment.Right
            layout: DockLayout {
            }
            
            // Sun
            ImageView {
                verticalAlignment: VerticalAlignment.Center
                horizontalAlignment: HorizontalAlignment.Center
                imageSource: "asset:///images/info/sun.png"
            }
    
            // Sadness
            ImageView {
                verticalAlignment: VerticalAlignment.Center
                horizontalAlignment: HorizontalAlignment.Center
                imageSource: "asset:///images/info/sad.png"
                visible: error_
            }
        }
    }
}