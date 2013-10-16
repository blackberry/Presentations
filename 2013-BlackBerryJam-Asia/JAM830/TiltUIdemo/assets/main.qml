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
import QtMobility.sensors 1.2

Page {
    property int tiltAngle: 0

    content: Container {
        background: Color.create("#1d1f1e")

        verticalAlignment: VerticalAlignment.Fill
        horizontalAlignment: HorizontalAlignment.Fill

        attachedObjects: [
            RotationSensor {
                active: true
                onReadingChanged: {
                    // When we tilt the device backwards, we add a sense of stiffness of 8 degrees
                    // before it actually changes the tiltAngle property (a.k.a. dead zone)
                    var deadZoneAngle = 8

                    if (tiltAngle >= reading.x && tiltAngle > (reading.x + deadZoneAngle)) {
                        tiltAngle = reading.x;
                    } else if (tiltAngle < reading.x) {
                        tiltAngle = reading.x;
                    }
                }

            }
        ]

        Container {
            layout: DockLayout {
            }

            preferredHeight: 430
            verticalAlignment: VerticalAlignment.Fill
            horizontalAlignment: HorizontalAlignment.Fill
            bottomPadding: 30

            Label {
                text: "<html><b>Tilt</b> device</html>"
                textStyle.fontSizeValue: 29.5
                textStyle.fontFamily: "Slate"
                textStyle.fontSize: FontSize.PointValue
                textStyle.textAlign: TextAlign.Center
                
                verticalAlignment: VerticalAlignment.Bottom
                horizontalAlignment: HorizontalAlignment.Center
                
                textFit {
                    mode: LabelTextFitMode.FitToBounds
                    minFontSizeValue: 25
                    maxFontSizeValue: 29.5
                }
                
                opacity: (tiltAngle <= 10) ? 1 : 0
            }

            Label {
                text: "<html>Maximum <b>tilt</b></html>"
                textStyle.fontSizeValue: 29.5
                textStyle.fontFamily: "Slate"
                textStyle.fontSize: FontSize.PointValue
                textStyle.textAlign: TextAlign.Center
                
                verticalAlignment: VerticalAlignment.Bottom
                horizontalAlignment: HorizontalAlignment.Center
                
                textFit {
                    mode: LabelTextFitMode.FitToBounds
                    minFontSizeValue: 25
                    maxFontSizeValue: 29.5
                }
                opacity: (tiltAngle > 60) ? 1 : 0
            }
        }

        Container {
            horizontalAlignment: HorizontalAlignment.Fill

            ColoredBlock {
                preferredHeight: 255
                backgroundColor: Color.create("#fa575a")
                opacity: (tiltAngle > 50) ? 1 : 0
            }
            ColoredBlock {
                preferredHeight: 220
                backgroundColor: Color.create("#fda099")
                opacity: (tiltAngle > 40) ? 1 : 0
            }
            ColoredBlock {
                preferredHeight: 180
                backgroundColor: Color.create("#ffffff")
                opacity: (tiltAngle > 30) ? 1 : 0
            }
            ColoredBlock {
                preferredHeight: 145
                backgroundColor: Color.create("#d9f4cb")
                opacity: (tiltAngle > 20) ? 1 : 0
            }
            ColoredBlock {
                preferredHeight: 110
                backgroundColor: Color.create("#89c2af")
                opacity: (tiltAngle > 10) ? 1 : 0
            }
        }
    }
}
