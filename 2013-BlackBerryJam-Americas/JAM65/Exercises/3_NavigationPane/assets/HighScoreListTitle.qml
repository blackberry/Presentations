import bb.cascades 1.0

Container {
    id: highScoreItem
    leftPadding: 50
    rightPadding: 50
    
    layout: StackLayout {
        orientation: LayoutOrientation.LeftToRight
    }
    Container {
        layoutProperties: StackLayoutProperties {
            spaceQuota: 2
        }
    }
    Label {
        text: "Player"
        textStyle.color: Color.create("#fafafa")
        layoutProperties: StackLayoutProperties {
            spaceQuota: 4
        }
    }
    Label {
        text: "Moves"
        textStyle.color: Color.create("#fafafa")
        layoutProperties: StackLayoutProperties {
            spaceQuota: 2
        }
    }
    Label {
        text: "Time"
        textStyle.color: Color.create("#fafafa")
        layoutProperties: StackLayoutProperties {
            spaceQuota: 2
        }
    }
}
