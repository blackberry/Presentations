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
                    } 
                    else if (tiltAngle < reading.x) {
                        tiltAngle = reading.x;
                    }
                }

            }
        ]        
        
        Container {
            layout: DockLayout {}
            preferredHeight: 530
            verticalAlignment: VerticalAlignment.Fill
            horizontalAlignment: HorizontalAlignment.Fill
            bottomPadding: 30
            
            Label {
                text: "Tilt device"
                textStyle.fontSizeValue: 20
                textStyle.fontSize: FontSize.PointValue
                textStyle.fontWeight: FontWeight.W100
                multiline: true 
                verticalAlignment: VerticalAlignment.Bottom
                horizontalAlignment: HorizontalAlignment.Center                
                
                opacity: (tiltAngle <= 20)  ? 1 : 0
            }        
            
            Label {
                text: "Max tilt"
                textStyle.fontSizeValue: 20
                textStyle.fontSize: FontSize.PointValue
                textStyle.fontWeight: FontWeight.W100
                verticalAlignment: VerticalAlignment.Bottom
                horizontalAlignment: HorizontalAlignment.Center
                
                opacity: (tiltAngle > 70) ? 1 : 0
            }        
        }
        
        Container {
            horizontalAlignment: HorizontalAlignment.Fill        
            
            ColoredBlock {
                backgroundColor: Color.create("#da2128")
                opacity: (tiltAngle > 60) ? 1 : 0
            }
            ColoredBlock {
                backgroundColor: Color.create("#f68b28")
                opacity: (tiltAngle > 50) ? 1 : 0
            }
            ColoredBlock {
                backgroundColor: Color.create("#ffe710")
                opacity: (tiltAngle > 40) ? 1 : 0
            }           
            ColoredBlock {
                backgroundColor: Color.create("#8cc747")
                opacity: (tiltAngle > 30) ? 1 : 0
            }            
            ColoredBlock {
                backgroundColor: Color.create("#00a95c")
                opacity: (tiltAngle > 20) ? 1 : 0
            }        
        }
    }
}
