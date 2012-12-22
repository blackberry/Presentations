import bb.cascades 1.0
import bb.cascades.multimedia 1.0
import bb.community.barcode 1.0

//-- create one page with a label and text

NavigationPane {
    id: mainPane
    objectName: "mainPane"
    Page {
        Container {
            layout: DockLayout {
            }
            Container {
                horizontalAlignment: HorizontalAlignment.Left
                verticalAlignment: VerticalAlignment.Top
                BarcodeDecoder {
                    id: barcodeDecoder
                    objectName: "barcodeDecoder"
                    onNewBarcodeDetected: {
                        barcodeLabel.text = barcode
                        //barcodeText.text = barcode
                        //mainPane.push(barcodeInfo)
                    }
                }
            }
            Container {
                horizontalAlignment: HorizontalAlignment.Center
                verticalAlignment: VerticalAlignment.Bottom
                Label {
                    id: barcodeLabel 
                    objectName: "barcodeLabel"
                    text: "Scan a barcode"
                    textStyle.fontSize: FontSize.XLarge
                    textStyle.fontWeight: FontWeight.Bold
                    textStyle.color: Color.White
                }
            }
        }
    }
    attachedObjects: [
        Page {
            id: barcodeInfo
            Container {
                Label {
                    id: barcodeText
                    text: "Barcode scanned!"
                }
            }
            /*                // Override the default back button to just pop the current
             * // screen off of the stack, instead of pop and delete
             * paneProperties: NavigationPaneProperties {
             * backButton: ActionItem {
             * onTriggered: {
             * mainPane.pop();
             * }
             * }
             * }*/
        }
    ]
}
