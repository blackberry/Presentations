import bb.cascades 1.0

Container {
    id: highScoreItem
    leftPadding: 50
    rightPadding: 50
    
    Container {
        layout: StackLayout {
            orientation: LayoutOrientation.LeftToRight
        }
        Label {
            text: highScoreItem.ListItem.indexPath[0]
            textStyle.base: SystemDefaults.TextStyles.TitleText
            textStyle.color: Color.create("#fafafa")
            layoutProperties: StackLayoutProperties {
                spaceQuota: 2
            }
        }
        Label {
            text: ListItemData.name
            textStyle.base: SystemDefaults.TextStyles.TitleText
            textStyle.color: Color.create("#fafafa")
            layoutProperties: StackLayoutProperties {
                spaceQuota: 4
            }
        }
        Label {
            text: ListItemData.moves
            textStyle.color: Color.create("#fafafa")
            layoutProperties: StackLayoutProperties {
                spaceQuota: 2
            }
        }
        Label {
            text: ListItemData.time
            textStyle.color: Color.create("#fafafa")
            layoutProperties: StackLayoutProperties {
                spaceQuota: 2
            }
        }
    }
    
    Divider {
    }
}
