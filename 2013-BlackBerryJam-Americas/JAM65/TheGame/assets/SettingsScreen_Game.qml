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
        leftPadding: 40
        rightPadding: 40
        topPadding: 40
        bottomPadding: 40
        Label {
            id: enterName
            text: "Enter Name"
            textStyle.base: SystemDefaults.TextStyles.BigText
            verticalAlignment: VerticalAlignment.Center
        }
        TextField {
            id: playerName
            hintText: "Enter Name"
            text: settings.getValueFor("player_name", "playerName")
            layoutProperties: StackLayoutProperties {
            }
            onTextChanging: {
                settings.saveValueFor("player_name", text);
            }
        }
        Divider {
        }
        Label {
            id: timeLimit
            text: "Time Limit"
            textStyle.base: SystemDefaults.TextStyles.BigText
        }
        ToggleButton {
            id: primaryToggleButton
            checked: settings.getValueFor("time_limit", "true")
            onCheckedChanged: {
                settings.saveValueFor("time_limit", checked);
            }
        }
        Divider {
        }
        Label {
            id: resetHighscore
            text: "Reset HighScore"
            textStyle.base: SystemDefaults.TextStyles.BigText
        }
        Button {
            id: resetHighscoreButton
            text: "Reset"
        }
       
}
