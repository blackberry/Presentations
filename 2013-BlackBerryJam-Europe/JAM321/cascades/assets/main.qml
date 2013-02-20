import bb.cascades 1.0

/*
 * Copyright (c) 2013 Research In Motion.
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
TabbedPane {
    showTabsOnActionBar: true
    tabs: [
        Tab {
            description: "HDMI Configuration"
            title: "HDMI"
            content: Page {
                Container {
                    id: outer
                    property int height
                    layout: StackLayout {
                    }
                    ForeignWindowControl {
                        id: embedded
                        preferredHeight: 435
                        preferredWidth: 768
                        layoutProperties: AbsoluteLayoutProperties {
                            positionX: 0
                            positionY: 0
                        }
                    }
                    Container {
                        horizontalAlignment: HorizontalAlignment.Fill
                        topMargin: 15
                        bottomMargin: 15
                        ImageView {
                            horizontalAlignment: HorizontalAlignment.Center
                            imageSource: "asset:hdmi.png"
                            opacity: screen.isHDMIAttached ? 1 : 0.25
                        }
                        Label {
                            text: screen.isHDMIAttached ? "Connected" : "Not connected"
                            horizontalAlignment: HorizontalAlignment.Center
                        }
                    }
                    Container {
                        layout: DockLayout {
                        }
                        verticalAlignment: VerticalAlignment.Fill
                        horizontalAlignment: HorizontalAlignment.Fill
                        ScrollView {
                            content: Container {
                                Button {
                                    text: "Start HDMI"
                                    onClicked: {
                                        screen.createHDMIWindow();
                                    }
                                    enabled: (! screen.hdmiRendering) && screen.isHDMIAttached
                                }
                                Button {
                                    text: "Start Embedded"
                                    onClicked: {
                                        screen.createEmbeddedWindow();
                                    }
                                    enabled: ! screen.embeddedRendering
                                }
                                Button {
                                    text: "Stop Rendering"
                                    onClicked: {
                                        screen.stopRendering();
                                    }
                                    enabled: screen.hdmiRendering || screen.embeddedRendering
                                }
                            }
                            verticalAlignment: VerticalAlignment.Fill
                            horizontalAlignment: HorizontalAlignment.Fill
                        }
                    }
                }
            }
        },
        Tab {
            description: "USB Details"
            title: "USB"
            content: Page {
                titleBar: TitleBar {
                    title: "USB"
                }
                content: Container {
                    verticalAlignment: VerticalAlignment.Fill
                    horizontalAlignment: HorizontalAlignment.Fill
                    layout: DockLayout {
                    }
                    Container {
                        horizontalAlignment: HorizontalAlignment.Center
                        verticalAlignment: VerticalAlignment.Center
                        ImageView {
                            horizontalAlignment: HorizontalAlignment.Center
                            imageSource: "asset:usb-logo.png"
                            opacity: aapio.connected ? 1 : 0.25
                        }
                        Label {
                            horizontalAlignment: HorizontalAlignment.Center
                            text: aapio.connected ? "Connected" : "Not connected"
                        }
                    }
                }
            }
        },
        Tab {
            description: "BlueTooth Details"
            title: "BT"
            content: Page {
                content: Container {
                    Button {
                        text: "Start SPP Server"
                        onClicked: {
                            bluetoothManager.startSPPServer();
                        }
                    }
                    ListView {
                        id: bluetoothDevices
                        dataModel: bluetoothManager.getPairedDevices()
                        multiSelectHandler.active: true
                        onSelectionChanged: {
                            if (selected && indexPath.length >= 1) {
                                sppClientButton.enabled = true;
                            } else {
                                sppClientButton.enabled = false;
                            }
                        }
                        preferredHeight: 400
                        enabled: true
                        onActivationChanged: {
                            if (active) {
                                bluetoothDevices.clearSelection();
                                bluetoothDevices.select(indexPath, active);
                            }
                        }
                    }
                    Button {
                        id: sppClientButton
                        text: "Start SPP Client"
                        enabled: false
                        onClicked: {
                            bluetoothManager.startSPPClient(bluetoothDevices.selected()[0]);
                        }
                    }
                }
            }
        }
    ]
    paneProperties: NavigationPaneProperties {
    }
}
