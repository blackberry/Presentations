
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
                text: qsTr("The screenreader will assume punctuation is part of the sentence.\n\nView the before page with screenreader turned on and tap the red text. The screenreader will not read out the names of punctuation marks. \n\nThe Solution is to set the accessibility.name string to the same string, but with your punctuation marks replaced with their names explicitly spelled out.") + Retranslate.onLocaleOrLanguageChanged
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
            source: "PunctuationReference_Before.qml"
        }
    ]

}
