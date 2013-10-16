/* Copyright (c) 2013 BlackBerry Limited.
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
import bb.cascades 1.2

Container {
    id: pullInfoContainer
    horizontalAlignment: HorizontalAlignment.Fill
    property alias text: pullLabel.text
    property alias textScale: pullLabel.scaleX
    property bool allowScaling: true
    property real snapBackScale: 1.3
    property alias height: pullInfoContainer.preferredHeight

    signal done()

    layout: DockLayout {
    }

    Label {
        id: loadingLabel
        horizontalAlignment: HorizontalAlignment.Fill
        verticalAlignment: VerticalAlignment.Center
        text: qsTr("LOADING")
        textStyle.base: SystemDefaults.TextStyles.BigText
        textStyle.textAlign: TextAlign.Center
        translationY: - height

        // Turn of all implicit animations to get quick response while scaling in update().
        attachedObjects: [
            ImplicitAnimationController {
                enabled: false
            }
        ]
    }

    Label {
        id: pullLabel
        horizontalAlignment: HorizontalAlignment.Fill
        verticalAlignment: VerticalAlignment.Center
        text: qsTr("PULL TO REFRESH")
        textStyle.base: SystemDefaults.TextStyles.BigText
        textStyle.textAlign: TextAlign.Center
        pivotY: 10
        scaleX: 1.0
        scaleY: scaleX

        animations: [
            // Animation used to indicate that if releasing refresh will be triggered.
            ScaleTransition {
                id: readyForRefresh
                toX: 1
                toY: 1
                duration: 150
                easingCurve: StockCurve.QuadraticOut

                onEnded: {
                    allowScaling = false;
                }
            }
        ]

        // Turn of all implicit animations to get quick response while scaling in update().
        attachedObjects: [
            ImplicitAnimationController {
                enabled: false
            }
        ]
    }

    animations: [
        // Shows the loading label and hides the pull label.
        ParallelAnimation {
            id: loadingAnim
            TranslateTransition {
                target: loadingLabel
                toY: 0
            }
            TranslateTransition {
                target: pullLabel
                toY: height
            }
        },
        // Hides the loading label.
        TranslateTransition {
            id: doneAnim
            target: loadingLabel
            toY: height

            onEnded: {
                done();
                reset();
            }
        }
    ]

	// Turn of all implicit animations to get quick response while scaling in update().
    attachedObjects: [
        ImplicitAnimationController {
            enabled: false
        }
    ]

    function reset() {
        opacity = 0;
        loadingLabel.translationY = - height;
        pullLabel.translationY = 0;
        allowScaling = false;
    }

    function loading() {
        loadingAnim.play();
    }
    
    function refreshDone() {
        doneAnim.play();
    }
    
    function update(scale) {
        
        // Use an implicit animation to set the current scale of the status text.
        if(!allowScaling && scale < 1.0) {
            // Scaling is turned off while the animation is running and not turned on
            // again until on update has recieved (to avoid the label being shown prematurely)
            allowScaling = true;

        } else if(allowScaling && readyForRefresh.state != AnimationState.Playing) {
            if (scale >= snapBackScale) {
                readyForRefresh.play();
        
            } else {
                text = qsTr("PULL TO REFRESH");
                textScale = scale;
                opacity = 1;
            }
        }
    }
}
