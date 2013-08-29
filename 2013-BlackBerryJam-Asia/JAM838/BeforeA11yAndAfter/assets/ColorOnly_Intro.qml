
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

import bb.cascades 1.2

Page {

    titleBar: TitleBar {
        title: qsTr("Introduction") + Retranslate.onLocaleOrLanguageChanged
    }
    ScrollView {
        Container {
            topPadding: 15
            bottomPadding: 15
            leftPadding: 15
            rightPadding: 15
            Label {
                text: qsTr("To a blind person, there is no difference between a blue button and a red button. \n\nView the before page with screenreader turned on.  How would a blind person distinguish the difference between the two buttons? \n\n\nThe Solution is labelling the buttons.  This is just a matter of good user experence as even seeing people might have misinterpretted the meaning of the colored buttons.") + Retranslate.onLocaleOrLanguageChanged
                multiline: true
            }
        }
    }

    actions: ActionItem {
        title: qsTr("Before A11y") + Retranslate.onLocaleOrLanguageChanged
        ActionBar.placement: ActionBarPlacement.OnBar

        onTriggered: {
            navigationPane.push(beforePageDefinition.createObject());
        }
    }

    attachedObjects: [
        ComponentDefinition {
            id: beforePageDefinition
            source: "ColorOnly_Before.qml"
        }
    ]

}
