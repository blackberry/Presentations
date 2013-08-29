
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
                text: qsTr("To a blind person, figuring out the context of a control is hard because they do not associate it with nearby labels.\n\nView the before page with screenreader turned on.Tap on the Sliders and CheckBox.\nIn the after page, you can see that the screen reader announces the sliders' context as well as value and the entire line of the checkbox can be used as a checkbox.\n\nThe Solution is to use the labelledby property to give the sliders context and to use a CustomA11yObject to set the entire container as a proxy for the checkbox.") + Retranslate.onLocaleOrLanguageChanged
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
            source: "LabellingRel_Before.qml"
        }
    ]

}
