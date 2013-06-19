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
        id: heartRate
        layout: DockLayout {
        }

        background: Color.Black
        ForeignWindowControl {
            id: viewfinder
            objectName: "vfWindow"
            windowId: "viewfinder"
            visible: showCamera.checked
            verticalAlignment: VerticalAlignment.Center
            horizontalAlignment: HorizontalAlignment.Center
            updatedProperties: WindowProperty.Position | WindowProperty.Size | WindowProperty.Visible
            onWindowAttached: {
             	Heartbeat.onWindowAttached(windowHandle, windowGroup, windowId);   
            }
        }
        
        Label {
            id: avgLabel
            text: Heartbeat.avgrate.toFixed(0) + " bpm"
            verticalAlignment: VerticalAlignment.Center
            horizontalAlignment: HorizontalAlignment.Center
            textStyle.fontSize: FontSize.XXLarge
            textStyle.color: Color.DarkGreen
        }
        
        Button {
            id: startButton
            text: "Start"
            visible: !Heartbeat.started
            verticalAlignment: VerticalAlignment.Bottom
            horizontalAlignment: HorizontalAlignment.Center
            onClicked: {
                if (Heartbeat.startCamera()) {
                    // started
                }
            }
        }
        Button {
            id: stopButton
            text: "Stop"
            visible: Heartbeat.started
            verticalAlignment: VerticalAlignment.Bottom
            horizontalAlignment: HorizontalAlignment.Center
            onClicked: {
                Heartbeat.stopCamera();
                showCamera.checked = false;
            }
        }
        Container {
            horizontalAlignment: HorizontalAlignment.Center
            verticalAlignment: VerticalAlignment.Top
            visible: Heartbeat.started

            layout: StackLayout {
                orientation: LayoutOrientation.LeftToRight
            }
            background: Color.Gray
            leftPadding: 30.0
            rightPadding: 30.0
            topPadding: 10.0
            bottomPadding: 10.0
            topMargin: 10.0
            Label {
                text: "show camera"                
                verticalAlignment: VerticalAlignment.Center
                textStyle.color: Color.White

            }
	        ToggleButton {
	            id: showCamera
                verticalAlignment: VerticalAlignment.Center
                checked: false
            }
	    }
    }
}
