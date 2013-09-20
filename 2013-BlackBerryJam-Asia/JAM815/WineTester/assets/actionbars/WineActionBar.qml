/*
 * Copyright 2013 BlackBerry Limited.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

import bb.cascades 1.0

Container {

    /**
     * Text to dislay on the title bar
     */
    property alias titleText: titleLabel.text

    /**
     * Image to display next to the title bar
     */
    property alias titleImage: titleImageView.imageSource

    /**
     * Controls the visibility of the right button
     */
    property alias rightButtonVisible: rightButton.visible

    /**
     * Icon for the right ActionBarButton
     */
    property alias rightButtonIcon: rightButton.iconImageSource

    /**
     * Background image for the for the right ActionBarButton
     */
    property alias rightButtonImage: rightButton.backgroundImageSource

    signal rightButtonClicked()

    layout: DockLayout {
    }

    preferredHeight: _features.getTopActionBarHeight()
    maxHeight: _features.getTopActionBarHeight()

    horizontalAlignment: HorizontalAlignment.Fill

    background: back.imagePaint

    Container {
        id: labelContainer
        visible: titleText != undefined
        leftPadding: 15
        horizontalAlignment: imageContainer.visible ? HorizontalAlignment.Left : HorizontalAlignment.Center
        verticalAlignment: VerticalAlignment.Center
        Label {
            id: titleLabel
            textStyle.color: Color.White
            //textStyle.fontWeight: FontWeight.Bold
            textStyle.fontSize: FontSize.Large
        }
    }

    Container {
        verticalAlignment: VerticalAlignment.Center
        horizontalAlignment: HorizontalAlignment.Center
        layout: StackLayout {
            orientation: LayoutOrientation.LeftToRight
        }

        Container {
            id: imageContainer
            visible: titleImage != undefined
            rightPadding: titleLabel.visible ? 5 : undefined

            horizontalAlignment: labelContainer.visible ? HorizontalAlignment.Right : HorizontalAlignment.Center
            verticalAlignment: VerticalAlignment.Center
            ImageView {
                id: titleImageView
                horizontalAlignment: HorizontalAlignment.Center
                verticalAlignment: VerticalAlignment.Center
            }
        }

    }

    ActionBarButton {
        id: rightButton
        horizontalAlignment: HorizontalAlignment.Right
        verticalAlignment: VerticalAlignment.Center
        rightPadding: 15

        onTouch: {
            rightButtonClicked();
        }
    }
    attachedObjects: [
        ImagePaintDefinition {
            id: back
            repeatPattern: RepeatPattern.X
            imageSource: "asset:///images/ActionBarBanner.png"
        }
    ]
}
