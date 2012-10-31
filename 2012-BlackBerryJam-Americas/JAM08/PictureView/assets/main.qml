/* Copyright (c) 2012 Research In Motion Limited.
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

// We use the Camera control from cascades multimedia, it needs to be initiated from C++
// code before we can use it though. 
import bb.cascades.multimedia 1.0

import bb.cascades 1.0
import com.sample.invoke 1.0


//-- create one page with a label and text
TabbedPane {
    showTabsOnActionBar: true
    onActiveTabChanged: {
        
        cardContent.resetSelectedIndex();
        //cardLabel.text = qsTr ("Select card from dropdown");
        //cardContentLabel.text = "";
        if (activeTab == camtab) {
            _camhdlr.camerastart();
        }
        else {
            _camhdlr.camerastop();
        }
    }

    Tab {
        id: camtab
        title: qsTr ("Camera")                

        content : Page {
            // A Container is used to gather visual items together.
            Container {
                layout: DockLayout {}
                horizontalAlignment: HorizontalAlignment.Fill
                verticalAlignment: VerticalAlignment.Fill
                
                // This is the camera control that is defined in the cascades multimedia library.
                Camera {
                    id: camera
                    objectName: "myCamera"
                    
                    onTouch: {
                        if (event.isDown()) {
                            // Take photo
                            capturePhoto();
                        }
                    }
                    
                    // There are loads of messages we could listen to here.
                    // onPhotoSaved and onShutterFired are taken care of in the C++ code.
                    onCameraOpenFailed: {
                        console.log("onCameraOpenFailed signal received with error " + error);
                    }
                    onViewfinderStartFailed: {
                        console.log("viewfinderStartFailed signal received with error " + error);
                    }
                    onViewfinderStopFailed: {
                        console.log("viewfinderStopFailed signal received with error " + error);
                    }
                    onPhotoCaptureFailed: {
                        console.log("photoCaptureFailed signal received with error " + error);
                    }
                    onPhotoSaveFailed: {
                        console.log("photoSaveFailed signal received with error " + error);
                    }
                    onCameraCloseFailed: {
                        console.log("photoSaveFailed signal received with error " + error);
                    }
                    onPhotoSaved: {
                        console.log("photoSave signal received " + fileName)
                        _cardinvoker.updateTarget(fileName)
                    }
                
                
                }
                Button {
                    id: toggleVf
                    objectName: "toggleVf"
                    //text: "Toggle Viewfinder"
                    imageSource: "asset:///images/start_stop.png"
                    layoutProperties: AbsoluteLayoutProperties {}
                    verticalAlignment: VerticalAlignment.Bottom
                    horizontalAlignment: HorizontalAlignment.Center
                
                }
            
            }//content container        
        }// Page
    }
  
    Tab {
        id: viewtab
        title: qsTr ("View")
        content : Page {
            Container {
                background: Color.Black
                layout: StackLayout {
                }
                Target {
                    id: cardTarget
                    visible: false
                    target: targetMap.target
                    action: targetMap.action
                    mime: targetMap.mime
                    uri: targetMap.uri
                    data: targetMap.data
                    description: targetMap.targetDescription
                    contentDescription: targetMap.contentDescription
                    onReplyReceived: {
                        console.debug("target reply: " + reply)
                    }
                }
                Container {
                    Label {
                        text: qsTr ("Choose a Picture")
                        layoutProperties: StackLayoutProperties {
                        }
                        horizontalAlignment: HorizontalAlignment.Center
                        textStyle {
                            color: Color.White
                            base: SystemDefaults.TextStyles.TitleText
                        }
                    }
                    Container {
                        background: Color.White
                        Divider {
                        }
                    }
                 
                   
                }
                Container {
                    background: Color.create("#929292")
                    opacity: 0.7
                    verticalAlignment: VerticalAlignment.Top
                    horizontalAlignment: HorizontalAlignment.Fill
                    DropDown {
                        id: cardContent
                        objectName: "cardContent"
                        visible: true
                        title: qsTr ("Content")
                        horizontalAlignment: HorizontalAlignment.Center
                        onSelectedIndexChanged: {
                            if (selectedIndex != -1) {     
                                _cardinvoker.setContent("Pictures", selectedIndex)                           
                                cardLaunch.enabled = true
                                setWallpaper.enabled = true
                            
                            } else {                                
                                cardLaunch.enabled = false
                                setWallpaper.enabled = false
                            }
                        }
                    }
                    Button {
                        id: cardLaunch
                        enabled: false
                        horizontalAlignment: HorizontalAlignment.Fill
                        text: qsTr ("View")
                        onClicked: cardTarget.invoke()
                        verticalAlignment: VerticalAlignment.Bottom
                    }
                    Button {
                        id: setWallpaper
                        enabled: false
                        horizontalAlignment: HorizontalAlignment.Fill
                        text: qsTr ("Set as Wallpaper")
                        onClicked: _cardinvoker.setaswallpaper(cardContent.selectedIndex)
                        verticalAlignment: VerticalAlignment.Bottom
                    }
                    
                    
                }
            }
        }
        onTriggered: {
          _cardinvoker.populateContentDropDown("cardContent")
        }
    }
    
    

    function resetTargetMapValues() {
        targetMap.target = ""
        targetMap.action = ""
        targetMap.mime = ""
        targetMap.uri = ""
        targetMap.data = ""
        targetMap.targetDescription = ""
        targetMap.contentDescription = ""
    }
}
