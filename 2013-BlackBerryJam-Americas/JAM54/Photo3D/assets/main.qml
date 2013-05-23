// Default empty project template
import bb.cascades 1.0

// creates one page with a label
Page {
    Container {
        topPadding: 20
        leftPadding: 20
        rightPadding: 20

        layout: StackLayout {}
        Label {
            text: qsTr("Photo 3D")
            textStyle.base: SystemDefaults.TextStyles.BigText
            verticalAlignment: VerticalAlignment.Center
            horizontalAlignment: HorizontalAlignment.Center
            multiline: true
        }

        Label {
            text: qsTr("Plug in the HDMI cable to see the stereoscopic photo")
            textStyle.base: SystemDefaults.TextStyles.BigText
            verticalAlignment: VerticalAlignment.Center
            horizontalAlignment: HorizontalAlignment.Center
            multiline: true
            leftMargin: 20
            rightMargin: 20
            topMargin: 100
            bottomMargin: 100
        }

        DropDown {
            title: "Stereoscopic Output"
            enabled: true
            topMargin: 10
            bottomMargin: 10
            horizontalAlignment: HorizontalAlignment.Center

            onSelectedIndexChanged: {
                console.log("SelectedIndex was changed to " + selectedIndex);
            }

            // image + text + description
            Option {
                //imageSource: "asset:///alarm.png"
                text: "Top / Bottom (Left - Right)"
                description: "Top shows Left photo and bottom shows Right photo"
                value: "tbLR"
                selected: true

                onSelectedChanged: {
                    if (selected == true) {
                        console.log("Setting stereoscopic mode to top / bottom (L - R)");
                        _photo3D_HDMI.setDisplayMode(0);
                    }
                }
            }

            // text + description
            Option {
                text: "Side by Side (Left - Right)"
                description: "Left and Right photos side by side"
                value: "tbRL"

                onSelectedChanged: {
                    if (selected == true) {
                        console.log("Setting stereoscopic mode to side by side (L - R)");
                        _photo3D_HDMI.setDisplayMode(1);
                    }
                }
            }

            // text + description
            Option {
                text: "Anaglyph (Red - Cyan)"
                description: "Left photo is shown in Red and Right photo is shown in Cyan"
                value: "tbRL"

                onSelectedChanged: {
                    if (selected == true) {
                        console.log("Setting stereoscopic mode to anaglyph (R - C)");
                        _photo3D_HDMI.setDisplayMode(2);
                    }
                }
            }

        }
    }
}

