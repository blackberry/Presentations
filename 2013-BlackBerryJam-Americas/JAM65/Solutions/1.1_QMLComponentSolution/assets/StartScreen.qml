import bb.cascades 1.0

Page {
    Container {
        layout: DockLayout {
        }
        // 1. Add the background ImageView (images/bg.png)
        ImageView {
            imageSource: "asset:///images/bg.png"
        }

        // 2. Add the logo ImageView (images/logo.png)
        ImageView {
            imageSource: "asset:///images/logo.png"
        }

        // 3. Add three Buttons in a centered StackLayout.
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
            }
            Button {
                text: "About"
            }
        }

        // 4. Add the Turtle ImageView (images/bg_turtle.png)
        ImageView {
            verticalAlignment: VerticalAlignment.Bottom
            imageSource: "asset:///images/bg_turtle.png"
        }
    }
}
