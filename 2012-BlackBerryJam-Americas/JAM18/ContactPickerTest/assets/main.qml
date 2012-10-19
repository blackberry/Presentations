import bb.cascades 1.0
// Don't forget to import the picker, and register it in C
import bb.cascades.pickers 1.0

Page {
    Container {
        layout: StackLayout {
        }
        Label {
            text: qsTr("Contact Picker")
            textStyle.base: SystemDefaults.TextStyles.BigText
            verticalAlignment: VerticalAlignment.Center
            horizontalAlignment: HorizontalAlignment.Center
        }
        //This button controls whether the ContactPicker is visible or not
        Button {
            text: "Open picker"
            onClicked: {
                contactPicker.open();
            }
            attachedObjects: [
                ContactPicker {
                    id: contactPicker
                    onContactSelected: {
                        //You could now take this contactId and use it to retrieve the actual Contact object
                        console.log("You chose contact: " + contactId);
                        result.text = "You chose contact: " + contactId;
                    }
                }
            ]
        }
        Label {
            id: result
            text: "You chose contact: "
        }
    }
}
