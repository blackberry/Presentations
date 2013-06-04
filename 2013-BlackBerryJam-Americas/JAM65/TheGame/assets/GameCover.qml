/* Copyright (c) 2013 Research In Motion Limited.
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
    background: back.imagePaint
    layout: DockLayout {
    }
    attachedObjects: [
        ImagePaintDefinition {
            id: back
            repeatPattern: RepeatPattern.Fill
            image: tracker.image
        },
        ImageTracker {
            id: tracker
            imageSource: "asset:///images/bg.png"
        }
    ]
    Container {
        Label {
            objectName: "coverTitle"
            textStyle.color: Color.Black
            textStyle.base: SystemDefaults.TextStyles.BodyText
            textStyle.textAlign: TextAlign.Center
            horizontalAlignment: HorizontalAlignment.Center
            multiline: true
        }
        Label {
            objectName: "coverPlayerName"
            textStyle.color: Color.Black
            textStyle.base: SystemDefaults.TextStyles.BodyText
            textStyle.textAlign: TextAlign.Center
            horizontalAlignment: HorizontalAlignment.Center
        }
        Label {
            objectName: "coverScore"
            textStyle.color: Color.Black
            textStyle.base: SystemDefaults.TextStyles.BodyText
            textStyle.textAlign: TextAlign.Center
            multiline: true
        }
    }
}
