// Default empty project template
import bb.cascades 1.0

// creates one page with a label

Page {
    Container {
        layout: StackLayout {
        }
        Label {
            text: qsTr("Invoke the Calendar")
            textStyle.base: SystemDefaults.TextStyles.BigText
            verticalAlignment: VerticalAlignment.Center
            horizontalAlignment: HorizontalAlignment.Center
        }
        Button {
            objectName: "pickerButton"
            id: pickerButton
            text: "Pick Event"
        }
        Button {
            objectName: "composerButton"
            id: composerButton
            text: "Compose Event"
        }
        Button {
            objectName: "viewerButton"
            id: viewerButton
            text: "View Event"
        }
        
        Label {
             objectName: "result"
             id: result
             text: "Result"
        }
    }
}
