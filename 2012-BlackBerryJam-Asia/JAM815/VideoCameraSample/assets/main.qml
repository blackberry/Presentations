import bb.cascades 1.0
import bb.cascades.multimedia 1.0

NavigationPane {
    id: navigationPane
    Page {
        onCreationCompleted: {
            console.debug("+++++++ All cameras acessible: " + camera.allCamerasAccessible);
            if (camera.allCamerasAccessible) {
                console.debug("Opening camera");
                camera.open(CameraUnit.Rear);
            }
        }
        Camera {
            id: camera
            property bool videoBeingTaken
            property bool cameraRunning
            
            onTouch: {
                if (! videoBeingTaken) {
                    videoBeingTaken = true;
                    camera.startVideoCapture();
                    console.debug("+++++++ ROLLING!");
                }
            }
            onCameraOpened: {
                console.debug("+++++++ Camera opened");
                camera.getSettings(cameraSettings);
                //cameraSettings = new CameraSettings();
                console.debug("+++++++ Settings got");
                cameraSettings.cameraMode = CameraMode.Video;
                console.debug("+++++++ Camera mode set");
                camera.applySettings(cameraSettings);
                console.debug("+++++++ Settings applied");
                camera.startViewfinder();
             
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
                console.debug("+++++++ Video caputre FAILED: " + error);
                videoBeingTaken = false;
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
            
            attachedObjects: [
                CameraSettings{
                    id: cameraSettings
                }
                ]
        }
    }
}
