import bb.cascades 1.0

Page {
    Container {
        // A more flexible way of handling background images is to paint the background using and
        // image paint. The paint will paint the entire Container background and a tiled image can be used.
        // Image Paints definitions are done as attached objects.
        background: backgroundPaint.imagePaint
        
        // An Example of a QML component, a custom title bar
        CustomTitleBar {
            title: "Highscore"
        }

		HighScoreListTitle {
		}

        // Add ListView, create a QML component in a seperate file and use it as list item
        ListView {
            dataModel: XmlDataModel {
                source: "highscoredb/highscore.xml"
            }

            listItemComponents: [
                ListItemComponent {
                    type: "item"
                    HighScoreItem {
                    }
                }
            ]            
        }
    }
    
    attachedObjects: ImagePaintDefinition {
        id: backgroundPaint
        imageSource: "images/bg.png"
    }
}

