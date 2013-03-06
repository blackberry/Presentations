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

// The Settings page; where the weather server address can be specified.
Page {
    resizeBehavior: PageResizeBehavior.None
    titleBar: TitleBar {
        title: "Settings"
        dismissAction: ActionItem {
            title: "Cancel"
            onTriggered: {
                serverUrl.text = _app.serverUrl;
                simulateProblems.checked = _app.simulateProblems;
                loadSize.text = _app.loadSize;
                maxSize.text = _app.maxSize;
                hackThreshold.text = _app.hackThreshold;
                settingsPage.close();
            }
        }

        acceptAction: ActionItem {
            title: "Save"
            onTriggered: {
                _app.serverUrl = serverUrl.text;
                _app.simulateProblems = simulateProblems.checked;
                _app.loadSize = loadSize.text;
                _app.maxSize = maxSize.text;
                _app.hackThreshold = hackThreshold.text;
                settingsPage.close();
            }
        }
    } // titleBar

    
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

        // Background images (art)
        Container {
            verticalAlignment: VerticalAlignment.Fill
            horizontalAlignment: HorizontalAlignment.Fill
            topPadding: 100
            layout: DockLayout {
            }
            layoutProperties: StackLayoutProperties {
                spaceQuota: 1
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
                ImageView {
                    imageSource: "asset:///images/info/sun.png"
                }
            }
        }

        // The form fields
        Container {
            topPadding: 20
            leftPadding: 20
            rightPadding: 26
            verticalAlignment: VerticalAlignment.Fill
            horizontalAlignment: HorizontalAlignment.Fill
            
            layout: StackLayout {
            }
            
            Label {
                horizontalAlignment: HorizontalAlignment.Left
                text: "Weather Server Address:"
                textStyle.color: Color.create("#fafafa")
            }
            TextField {
                id: serverUrl
                horizontalAlignment: HorizontalAlignment.Fill
                inputMode: TextFieldInputMode.Url
                text: _app.serverUrl
                preferredWidth: 700
            }
            
            Container {
                horizontalAlignment: HorizontalAlignment.Fill
                topPadding: 25
                
                layout: DockLayout {
                }
                
                Label {
                    text: "Simulate Server Problems:"
                    textStyle.color: Color.create("#fafafa")
                    horizontalAlignment: HorizontalAlignment.Left
                    verticalAlignment: VerticalAlignment.Center
                }
                
                ToggleButton {
                    id: simulateProblems
                    checked: _app.simulateProblems
                    horizontalAlignment: HorizontalAlignment.Right
                    verticalAlignment: VerticalAlignment.Center
                }
            }
            
            Container {
                horizontalAlignment: HorizontalAlignment.Fill
                topPadding: 25
                
                layout: DockLayout {
                }
                
                Label {
                    horizontalAlignment: HorizontalAlignment.Left
                    verticalAlignment: VerticalAlignment.Center
                    text: "List Load Chunk Size:"
                    textStyle.color: Color.create("#fafafa")
                }
                
                TextField {
                    id: loadSize
                    horizontalAlignment: HorizontalAlignment.Right
                    verticalAlignment: VerticalAlignment.Center
                    inputMode: TextFieldInputMode.NumbersAndPunctuation
                    text: _app.loadSize
                    preferredWidth: 350
                }
            }
            
            Container {
                horizontalAlignment: HorizontalAlignment.Fill
                topPadding: 25
                
                layout: DockLayout {
                }
                
                Label {
                    horizontalAlignment: HorizontalAlignment.Left
                    verticalAlignment: VerticalAlignment.Center
                    text: "Max List Size:"
                    textStyle.color: Color.create("#fafafa")
                }
                
                TextField {
                    id: maxSize
                    horizontalAlignment: HorizontalAlignment.Right
                    verticalAlignment: VerticalAlignment.Center
                    inputMode: TextFieldInputMode.NumbersAndPunctuation
                    text: _app.maxSize
                    preferredWidth: 350
                }
            }

            // Hack threshold, see WeatherHistoryApp::hackThreshold for description
            Container {
                horizontalAlignment: HorizontalAlignment.Fill
                topPadding: 25
                layout: DockLayout {
                }
                Label {
                    horizontalAlignment: HorizontalAlignment.Left
                    verticalAlignment: VerticalAlignment.Center
                    text: "Hack Threshold:"
                    textStyle.color: Color.create("#fafafa")
                }
                TextField {
                    id: hackThreshold
                    horizontalAlignment: HorizontalAlignment.Right
                    verticalAlignment: VerticalAlignment.Center
                    inputMode: TextFieldInputMode.NumbersAndPunctuation
                    text: _app.hackThreshold
                    preferredWidth: 350
                }
            }
        } // end form fields Containers
    }    // end Page level Container

    /**
     * Handles the changes to application settings.
     */
    function onServerUrlChanged() {
        serverUrl.text = _app.serverUrl;
    }
    
    function onSimulateProblemsChanged() {
        simulateProblems.checked = _app.simulateProblems;
    }

    function onLoadSizeChanged() {
        loadSize.text = _app.loadSize;
    }
    
    function onMaxSizeChanged() {
        maxSize.text = _app.maxSize;
    }
    
    function onHackThresholdChanged() {
        hackThreshold.text = _app.hackThreshold;
    }
    
    onCreationCompleted: {
        // Connect a handler function to the settings property changed signals
        _app.serverUrlChanged.connect(onServerUrlChanged);
        _app.simulateProblemsChanged.connect(onSimulateProblemsChanged);
        _app.loadSizeChanged.connect(onLoadSizeChanged);
        _app.maxSizeChanged.connect(onMaxSizeChanged);
        _app.hackThresholdChanged.connect(onHackThresholdChanged);
    }
}
