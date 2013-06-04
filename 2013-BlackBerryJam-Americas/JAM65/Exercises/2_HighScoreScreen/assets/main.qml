import bb.cascades 1.0

Page {
    Container {
        layout: DockLayout {
        }
        
        ImageView {
            imageSource: "asset:///images/bg.png"
        }
        
        Container {

            // An example of a QML component, a custom title bar
            CustomTitleBar {
                title: "Highscore"
            }

            // 1. Add a list ListView.
            // 2. Specify the datamodel property to a XmlDataModel with "highscoredb/highscore.xml" as source
            // 3. Create a QML component HighScoreItem.qml in a seperate file and use it as a ListItemComponent.
            // 4. Try to mimick the look of items in the presentation as closely as you can.
        }
    }
}
