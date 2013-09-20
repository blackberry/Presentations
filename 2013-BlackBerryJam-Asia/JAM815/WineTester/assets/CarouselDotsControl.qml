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
import QtQuick 1.0
import CustomTimer 1.0
import "RenderRules.js" as RenderRules

Container {
    property string page1LabelHeader: qsTr("Welcome")
    property string page1LabelText: qsTr("Discover the world of the Goulet Estates")
    
    property string page2LabelHeader: qsTr("Cellar tours")
    property string page2LabelText: qsTr("Explore our vast wine cellars with friends & family")

    property string page3LabelHeader: qsTr("Understand our process")
    property string page3LabelText: qsTr("Come learn our winemaking process")

    property variant carouselHeaders: [ page1LabelHeader, page2LabelHeader, page3LabelHeader ]
    property variant carouselTexts: [ page1LabelText, page2LabelText , page3LabelText ]

    property variant carouselImages: [ "asset:///images/background1.png",
    								   "asset:///images/background2.png",
								       "asset:///images/background3.png" ]

    property int currentSlideIndex: 1
    layout: DockLayout {}
    horizontalAlignment: HorizontalAlignment.Fill
    verticalAlignment: VerticalAlignment.Fill
    Container {
        layout: DockLayout {}
        horizontalAlignment: HorizontalAlignment.Fill
        verticalAlignment: VerticalAlignment.Fill

        ImageView {
            id: carouselBuffer1
            opacity: 1
            horizontalAlignment: HorizontalAlignment.Fill
            verticalAlignment: VerticalAlignment.Fill
            imageSource: carouselImages[0]
            animations: [
                FadeTransition {
                    id: carouselBuffer1FadeOut
                    duration: 2000
                    easingCurve: StockCurve.CubicIn
                    fromOpacity: 1.0
                    toOpacity: 0.0
                },
                FadeTransition {
                    id: carouselBuffer1FadeIn
                    duration: 2000
                    easingCurve: StockCurve.CubicIn
                    fromOpacity: 0.0
                    toOpacity: 1.0
                },
                ScaleTransition {
                    id: carouselBuffer1ScaleOut
                    duration: 5000
                    easingCurve: StockCurve.Linear
                    fromX: 1.0
                    fromY: 1.0
                    toX: 1.6
                    toY: 1.6
                }
            ]
        }

        ImageView {
            id: carouselBuffer2
            opacity: 0
            horizontalAlignment: HorizontalAlignment.Fill
            verticalAlignment: VerticalAlignment.Fill
            imageSource: carouselImages[1]
            animations: [
                FadeTransition {
                    id: carouselBuffer2FadeOut
                    duration: 2000
                    easingCurve: StockCurve.CubicIn
                    fromOpacity: 1.0
                    toOpacity: 0.0
                },
                FadeTransition {
                    id: carouselBuffer2FadeIn
                    duration: 2000
                    easingCurve: StockCurve.CubicIn
                    fromOpacity: 0.0
                    toOpacity: 1.0
                },
                ScaleTransition {
                    id: carouselBuffer2ScaleOut
                    duration: 5000
                    easingCurve: StockCurve.Linear
                    fromX: 1.0
                    fromY: 1.0
                    toX: 1.6
                    toY: 1.6
                }
            ]
        }

        ImageView {
            id: carouselBuffer3
            opacity: 0
            horizontalAlignment: HorizontalAlignment.Fill
            verticalAlignment: VerticalAlignment.Fill
            imageSource: carouselImages[2]
            animations: [
                FadeTransition {
                    id: carouselBuffer3FadeOut
                    duration: 2000
                    easingCurve: StockCurve.CubicIn
                    fromOpacity: 1.0
                    toOpacity: 0.0
                },
                FadeTransition {
                    id: carouselBuffer3FadeIn
                    duration: 2000
                    easingCurve: StockCurve.CubicIn
                    fromOpacity: 0.0
                    toOpacity: 1.0
                },
                ScaleTransition {
                    id: carouselBuffer3ScaleOut
                    duration: 5000
                    easingCurve: StockCurve.Linear
                    fromX: 1.0
                    fromY: 1.0
                    toX: 1.6
                    toY: 1.6
                }
            ]
        }
    }

    Container {
        layout: StackLayout {
        	orientation: LayoutOrientation.TopToBottom  
        }
     	topPadding: RenderRules.CarouselTopTextOffset()
        verticalAlignment: VerticalAlignment.Center
     	horizontalAlignment: HorizontalAlignment.Center
     	Container {
     	    id: labelsContainer
     	    maxWidth: 600
            Label {
		        id: headerLabel
		        horizontalAlignment: HorizontalAlignment.Center
		        verticalAlignment: VerticalAlignment.Center
		        textStyle {
		            base: SystemDefaults.TextStyles.Title
		            fontSize: FontSize.Large
		            fontWeight: FontWeight.Bold
		            color: Color.White
		        }
		    }
		
		    Label {
		        id: textsLabel
		        multiline: true
		        horizontalAlignment: HorizontalAlignment.Center
		        verticalAlignment: VerticalAlignment.Center
		        textStyle {
		            base: SystemDefaults.TextStyles.BodyText
                    fontSize: FontSize.Small
                    color: Color.create("#f4f4f4");
		            textAlign: TextAlign.Center
		        }
		    }
        }

	}

    function update() {
        if (currentSlideIndex > carouselImages.length - 1) 
        	currentSlideIndex = 0;
        cycleSlide();
        var currentPathHeader = carouselHeaders[currentSlideIndex];
        var currentPathText = carouselTexts[currentSlideIndex];

        headerLabel.text = currentPathHeader;
        textsLabel.text = currentPathText;
        currentSlideIndex ++;
    }

    //Function used to switch slides with a smooth effect
    function cycleSlide() {

        if (currentSlideIndex > carouselImages.length - 1) 
        	currentSlideIndex = 0;
        var currentSlidePath = carouselImages[currentSlideIndex]

        //do stuff
        var carouselBuffer1Opacity = carouselBuffer1.opacity
        var carouselBuffer2Opacity = carouselBuffer2.opacity
        
        if (carouselBuffer1Opacity == 1.0) {
            //carouselBuffer2.imageSource = currentSlidePath
            carouselBuffer2ScaleOut.play()
            carouselBuffer2FadeIn.play()
            carouselBuffer1FadeOut.play()
        } else if (carouselBuffer2Opacity == 1.0){
            //carouselBuffer1.imageSource = currentSlidePath
            carouselBuffer3ScaleOut.play()
            carouselBuffer3FadeIn.play()
            carouselBuffer2FadeOut.play()
        } else {
            carouselBuffer1ScaleOut.play()
            carouselBuffer1FadeIn.play()
            carouselBuffer3FadeOut.play()
        }
    }

    QmlTimer {
        id: carouselTimer
        interval: 3000
        onTimeout: {
            update();
        }
    }

    onCreationCompleted: {
        carouselBuffer1FadeIn.play()
        carouselBuffer2FadeOut.play()
        carouselBuffer1ScaleOut.play()
        carouselTimer.start();
        update()
    }
}
