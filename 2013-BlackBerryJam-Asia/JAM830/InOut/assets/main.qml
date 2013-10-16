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

Page {
    property int contentHeight: 0

    titleBar: TitleBar {
        title: qsTr("Easing")
    }

    Container {
        layout: DockLayout {
        }
        
        // The Radio Button list used to set which easing to use to run the animation.
        CurveSelection {
            onCurveChanged: {
                overlay.hideCurve = curve;
                overlay.showCurve = curve;
            }
        }

		// The overlay that will be animated once pressed.
        Overlay {
            id: overlay
            horizontalAlignment: HorizontalAlignment.Center
            verticalAlignment: VerticalAlignment.Bottom
        }
    }
}
