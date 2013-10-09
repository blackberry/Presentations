
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

// The Settings page; where some application settings can be made 
// to control the behaviour of the application.
Sheet {
	id: sheet
    Page {
        resizeBehavior: PageResizeBehavior.None
        titleBar: TitleBar {
            title: qsTr("Settings") + Retranslate.onLanguageChanged
            
            dismissAction: ActionItem {
                // Close the sheet and cancel changes that might have been done.
                title: qsTr("Cancel") + Retranslate.onLanguageChanged
                onTriggered: {
                    serverUrl.text = _appSettings.serverUrl;
                    simulateProblems.checked = _appSettings.simulateProblems;
                    loadSize.text = _appSettings.loadSize;
                    maxSize.text = _appSettings.maxSize;
                    sheet.close();
                }
            }

            acceptAction: ActionItem {
                title: qsTr("Save") + Retranslate.onLanguageChanged
                onTriggered: {
                    // Close the sheet and save changes.
                    _appSettings.serverUrl = serverUrl.text;
                    _appSettings.simulateProblems = simulateProblems.checked;
                    _appSettings.loadSize = loadSize.text;
                    _appSettings.maxSize = maxSize.text;
                    sheet.close();
                }
            }
        }

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

            // The Controls that are used to modify the settings.
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
                    text: qsTr("Weather Server Address:") + Retranslate.onLanguageChanged
                    textStyle.color: Color.create("#fafafa")
                }
                
                TextField {
                    id: serverUrl
                    horizontalAlignment: HorizontalAlignment.Fill
                    inputMode: TextFieldInputMode.Url
                    text: _appSettings.serverUrl
                    preferredWidth: 700
                }

				// Server problems toggle button, turns flaky server behaviour on and off.
                Container {
                    horizontalAlignment: HorizontalAlignment.Fill
                    topPadding: 25

                    layout: DockLayout {
                    }

                    Label {
                        text: qsTr("Simulate Server Problems:") + Retranslate.onLanguageChanged
                        textStyle.color: Color.create("#fafafa")
                        horizontalAlignment: HorizontalAlignment.Left
                        verticalAlignment: VerticalAlignment.Center
                    }

                    ToggleButton {
                        id: simulateProblems
                        checked: _appSettings.simulateProblems
                        horizontalAlignment: HorizontalAlignment.Right
                        verticalAlignment: VerticalAlignment.Center
                    }
                }

				// The list load chunk size determine how many items that are requested each time.
                Container {
                    horizontalAlignment: HorizontalAlignment.Fill
                    topPadding: 25

                    layout: DockLayout {
                    }

                    Label {
                        horizontalAlignment: HorizontalAlignment.Left
                        verticalAlignment: VerticalAlignment.Center
                        text: qsTr("List Load Chunk Size:") + Retranslate.onLanguageChanged
                        textStyle.color: Color.create("#fafafa")
                    }

                    TextField {
                        id: loadSize
                        horizontalAlignment: HorizontalAlignment.Right
                        verticalAlignment: VerticalAlignment.Center
                        inputMode: TextFieldInputMode.NumbersAndPunctuation
                        text: _appSettings.loadSize
                        preferredWidth: 350
                    }
                }

				// The max list size determine how many items the service will be able to provide.
                Container {
                    horizontalAlignment: HorizontalAlignment.Fill
                    topPadding: 25

                    layout: DockLayout {
                    }

                    Label {
                        horizontalAlignment: HorizontalAlignment.Left
                        verticalAlignment: VerticalAlignment.Center
                        text: qsTr("Max List Size:") + Retranslate.onLanguageChanged
                        textStyle.color: Color.create("#fafafa")
                    }

                    TextField {
                        id: maxSize
                        horizontalAlignment: HorizontalAlignment.Right
                        verticalAlignment: VerticalAlignment.Center
                        inputMode: TextFieldInputMode.NumbersAndPunctuation
                        text: _appSettings.maxSize
                        preferredWidth: 350
                    }
                }
            }
        }
    }
}