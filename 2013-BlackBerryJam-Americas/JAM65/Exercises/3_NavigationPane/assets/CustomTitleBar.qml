import bb.cascades 1.0

Container {
    property alias title: titleLabel.text
    
    layout: DockLayout {
    }

    ImageView {
        horizontalAlignment: HorizontalAlignment.Fill
        imageSource: "asset:///images/header_bg.png"
    }

    Label {
        id: titleLabel
        horizontalAlignment: HorizontalAlignment.Center
        verticalAlignment: VerticalAlignment.Center
        text: "Title"
        textStyle.base: SystemDefaults.TextStyles.BigText
        textStyle.color: Color.create("#fafafa")
    }
}
