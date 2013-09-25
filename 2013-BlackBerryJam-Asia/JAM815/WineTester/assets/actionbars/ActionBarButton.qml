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
    id: mainContainer
    property alias backgroundImageSource: btnBackground.imageSource
    property alias iconImageSource: iconImageView.imageSource
    
    layout: DockLayout {
    }
    Container {
        background: btnBackground.imagePaint
        horizontalAlignment: HorizontalAlignment.Fill
        verticalAlignment: VerticalAlignment.Fill
    }
    Container {
        horizontalAlignment: HorizontalAlignment.Fill
        verticalAlignment: VerticalAlignment.Fill
        layout: DockLayout {
        }

        ImageView {
            id: iconImageView
            visible: iconImageSource != undefined
            horizontalAlignment: HorizontalAlignment.Fill
            verticalAlignment: VerticalAlignment.Fill
            minHeight: _features.getTopActionBarSquareButtonHeight()
            maxHeight: _features.getTopActionBarSquareButtonHeight()
            minWidth: _features.getTopActionBarSquareButtonWidth()
            maxWidth: _features.getTopActionBarSquareButtonWidth()
        }
    }

    attachedObjects: [
        ImagePaintDefinition {
            id: btnBackground
            repeatPattern: RepeatPattern.XY
        }
    ]
}