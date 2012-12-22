// Default empty project template
import bb.cascades 1.0

// creates one page with a label

Page {
    Container {
        
        layout: DockLayout {
        }
        Label {
            text: qsTr("Barcode Invoker")
            textStyle.base: SystemDefaults.TextStyles.BigText
            verticalAlignment: VerticalAlignment.Center
            horizontalAlignment: HorizontalAlignment.Center
        }
        Container {
            layout: StackLayout {
            }
            Button {
                id: scanButton
                objectName: "scanButton"
                text: "Scan a barcode"
            }
            Label {
                id: resultsLabel
                objectName: "resultsLabel"
                text: "Results: "
            }
        }
    }
}
