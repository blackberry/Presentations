// Default empty project template
import bb.cascades 1.0

// creates one page with a label

Page {
    onCreationCompleted: {
            OrientationSupport.supportedDisplayOrientation = SupportedDisplayOrientation.All
        }
    Container {
        
        layout: DockLayout {
        }
        Label {
            text: qsTr("Camera Invoker")
            textStyle.base: SystemDefaults.TextStyles.BigText
            verticalAlignment: VerticalAlignment.Center
            horizontalAlignment: HorizontalAlignment.Center
        }
        Container {
            layout: StackLayout {
            }
            Button {
                id: photoButton
                objectName: "photoButton"
                text: "Take a picture"
            }
            Label {
                id: resultsLabel
                objectName: "resultsLabel"
                text: "Results: "
            }
        }
    }
}
