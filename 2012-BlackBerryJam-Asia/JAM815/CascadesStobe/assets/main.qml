import bb.cascades 1.0
import bb.cascades.multimedia 1.0
import qt 1.0

Page {
    id: page
    Container {
        onCreationCompleted: {
            console.debug("+++++++ All cameras acessible: " + camera.allCamerasAccessible);
            if (camera.allCamerasAccessible) {
                console.debug("Opening camera");
                camera.open(CameraUnit.Rear);
            }
        }
        layout: DockLayout {
        }
        Camera {
            id: camera
            property bool strobeOn
            property bool videoBeingTaken
            property bool cameraRunning

            onCameraOpened: {
                console.debug("+++++++ Camera opened");
                
                camera.getSettings(cameraSettings);
                cameraSettings.cameraMode = CameraMode.Video;
                cameraSettings.flashMode = CameraFlashMode.Light
                camera.applySettings(cameraSettings);
                console.debug("+++++++ Settings applied");
                camera.startViewfinder();
                strobeOn = false;

            }
            onCameraOpenFailed: {
                console.debug("+++++++ Camera open FAILED: " + error);
            }
            onViewfinderStarted: {
                console.debug("+++++++ Viewfinder started");
                videoBeingTaken = false;
            }
            onViewfinderStartFailed: {
                console.debug("+++++++ Viewfinder start FAILED: " + error);
            }
            onVideoCaptureFailed: {
                console.debug("+++++++ Video capture FAILED: " + error);
                videoBeingTaken = false;
                recordButton.text = "Start record"
            }
            onVideoCaptureStarted: {
                console.debug("+++++++ Video capture started");
            }
            onVideoCaptureStopped: {
                console.debug("+++++++ Video capture stopped: " + fileName);
            }
            onCameraResourceReleased: {
                videoBeingTaken = false;
                cameraRunning = false;
            }
        }
        Container {
            verticalAlignment: VerticalAlignment.Bottom
            horizontalAlignment: HorizontalAlignment.Center
            Button {
                id: recordButton
                text: "Start record"
                horizontalAlignment: HorizontalAlignment.Center
                onClicked: {
                    if (! camera.videoBeingTaken) {
                        camera.videoBeingTaken = true;
                        camera.startVideoCapture();
                        console.debug("+++++++ ROLLING!");
                        text = "Stop record"
                    } else {
                        camrea.videoBeingTaken = false;
                        camera.stopVideoCapture();
                        console.debug("+++++++ CUT!");
                        text = "Start record";
                    }
                }
            }
            Button {
                id: strobeButton
                text: "Start Strobe"
                horizontalAlignment: HorizontalAlignment.Center
                onClicked: {
                    if (! camera.strobeOn) {
                        camera.strobeOn = true;
                        strobeIntervalTimer.start();
                        console.debug("+++++++ Strobe on!");
                        text = "Stop strobe"
                    } else {
                        camera.strobeOn = false;
                        console.debug("+++++++ Strobe off!");
                        text = "Start strobe";
                    }
                }
            }
            Label {
                id: sliderValue
                text: "Strobe Interval: " + slider.value
            }
            Slider {
                id: slider
                fromValue: 125
                toValue: 600
                value: 125
            }
        }
        attachedObjects: [
            CameraSettings {
                id: cameraSettings
            },
            //These timers will switch the light on and off, creating a strobe effect 
            QTimer {
                id: strobeIntervalTimer
                interval: slider.value
                singleShot: true
                onTimeout: {
                    if (camera.strobeOn) {
                        cameraSettings.flashMode = CameraFlashMode.Light;
                        camera.applySettings(cameraSettings);
                        strobeLengthTimer.start();
                    }
                }
            },
            QTimer {
                id: strobeLengthTimer
                interval: 200
                singleShot: true
                onTimeout: {
                    if (camera.strobeOn) {
                        cameraSettings.flashMode = CameraFlashMode.Off;
                        camera.applySettings(cameraSettings);
                        strobeIntervalTimer.start();
                    }
                }
            }
        ]
    }
}
