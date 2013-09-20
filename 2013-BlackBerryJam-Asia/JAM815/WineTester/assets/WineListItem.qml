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

//This is the version used for the Z10.
import bb.cascades 1.0
import "RenderRules.js" as RenderRules

ListItemComponent {

    Container {

		//MARTIN GITHUB:
		//USERNAME: MARTINGOULET

        property int standardPadding: RenderRules.StandardListItemPadding()
        property int standardIconSize: RenderRules.StandardListItemIconSize()
        property bool descriptionMultiline: RenderRules.WineItemDescriptionMultiline()

        horizontalAlignment: HorizontalAlignment.Fill
        verticalAlignment: VerticalAlignment.Fill

        layout: StackLayout {
            orientation: LayoutOrientation.TopToBottom
        }
        
        //Entire container
        Container {
            leftPadding: standardPadding
            topPadding: standardPadding
            bottomPadding: standardPadding
            layout: StackLayout {
                orientation: LayoutOrientation.TopToBottom
            }
            
            //Top section
            Container {
                layout: StackLayout {
                    orientation: LayoutOrientation.LeftToRight
                }
	            
	            ImageView {
                    layoutProperties: StackLayoutProperties {
                        spaceQuota: 1
                    }
                    horizontalAlignment: HorizontalAlignment.Center
                    verticalAlignment: VerticalAlignment.Center
                    imageSource: ListItemData.logo
                    maxWidth: standardIconSize
                    maxHeight: standardIconSize
                }
                //Name & Description section
                Container {
                    leftPadding: standardPadding
                    layoutProperties: StackLayoutProperties {
                        spaceQuota: 4
                    }
                    layout: StackLayout {
                        orientation: LayoutOrientation.TopToBottom
                    }

                    Label {
                        id: wineName
                        text: ListItemData.name
                        multiline: false
                        textStyle {
                            base: nameTextStyle.style
                        }
                    }
                    Label {
                        id: wineDate
                        text: ListItemData.date
                        multiline: false
                        textStyle {
                            base: dateTextStyle.style
                        }
                    }
                }
            }
	
			//Bottom section (description)
	        Container {
                topPadding: standardPadding
                Label {
	                id: wineDesc
	                text: ListItemData.description
                    multiline: descriptionMultiline
                    textStyle {
                        base: descriptionTextStyle.style
                    }
                }
	        }
	    }
        
        Divider {
	        horizontalAlignment: HorizontalAlignment.Fill
	        verticalAlignment: VerticalAlignment.Bottom
	    }

        attachedObjects: [
            TextStyleDefinition {
                id: descriptionTextStyle
                base: SystemDefaults.TextStyles.TitleText
                textAlign: TextAlign.Left
                fontWeight: FontWeight.W200
                fontSize: FontSize.Small
                fontFamily: "helvetica neue"
                color: Color.create(0.6, 0.6, 0.6, 1.0)
            },
            TextStyleDefinition {
                id: nameTextStyle
                base: SystemDefaults.TextStyles.TitleText
                textAlign: TextAlign.Center
                fontWeight: FontWeight.Bold
                fontSize: FontSize.Medium
                fontFamily: "helvetica neue"
                color: Color.create(1.0, 1.0, 1.0, 1.0)
            },
            TextStyleDefinition {
                id: dateTextStyle
                base: SystemDefaults.TextStyles.TitleText
                textAlign: TextAlign.Left
                fontWeight: FontWeight.W200
                fontSize: FontSize.Small
                fontFamily: "helvetica neue"
                color: Color.create(0.6, 0.6, 0.6, 1.0)
            }
        ]
    }
    
}