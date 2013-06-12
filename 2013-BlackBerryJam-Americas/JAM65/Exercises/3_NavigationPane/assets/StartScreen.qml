import bb.cascades 1.0

Page {
    Container {
        layout: DockLayout {
        }
 
        ImageView {
            imageSource: "asset:///images/bg.png"
        }

        ImageView {
            imageSource: "asset:///images/logo.png"
        }

        Container {
            verticalAlignment: VerticalAlignment.Center
            horizontalAlignment: HorizontalAlignment.Center
            topPadding: 230

            Button {
                text: "Start"
            }
            Button {
                text: "Settings"
            }
            Button {
                text: "Highscore"
                // Add a singal handler for navigation here
            }
            Button {
                text: "About"
            }
        }

        ImageView {
            verticalAlignment: VerticalAlignment.Bottom
            imageSource: "asset:///images/bg_turtle.png"
        }
    }
}
