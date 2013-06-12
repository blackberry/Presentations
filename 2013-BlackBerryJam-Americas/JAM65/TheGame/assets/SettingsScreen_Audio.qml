/*
 * Copyright (c) 2011, 2012 Research In Motion Limited.
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
    layout: StackLayout {
    }
    topPadding: 40
    bottomPadding: 40
    leftPadding: 40
    rightPadding: 40
    Label {
        id: lableFX
        text: "Sound Effects"
        textStyle.base: SystemDefaults.TextStyles.BigText
    }
    Slider {
        value: soundManager.fxGain
        enabled: muteFXCheckBox.checked ? false : true
        onValueChanged: {
            soundManager.fxGain = value;
        }
    }
    Container {
        layout: StackLayout {
            orientation: LayoutOrientation.LeftToRight
        }
        CheckBox {
            id: muteFXCheckBox
            checked: soundManager.fxMute
            onCheckedChanged: {
                soundManager.fxMute = checked;
            }
        }
        Label {
            text: "Mute"
            textStyle.base: SystemDefaults.TextStyles.BodyText
            verticalAlignment: VerticalAlignment.Center
        }
    }
    Divider {
    }
    Label {
        id: labelSound
        text: "Music"
        textStyle.base: SystemDefaults.TextStyles.BigText
    }
    Slider {
        value: soundManager.musicGain
        onValueChanged: {
            soundManager.musicGain = value;
        }
        enabled: muteMusicCheckBox.checked ? false : true
    }
    Container {
        layout: StackLayout {
            orientation: LayoutOrientation.LeftToRight
        }
        CheckBox {
            id: muteMusicCheckBox
            checked: soundManager.musicMute
            onCheckedChanged: {
                soundManager.musicMute = checked;
            }
        }
        Label {
            text: "Mute"
            textStyle.base: SystemDefaults.TextStyles.BodyText
            verticalAlignment: VerticalAlignment.Center
        }
    }
}
