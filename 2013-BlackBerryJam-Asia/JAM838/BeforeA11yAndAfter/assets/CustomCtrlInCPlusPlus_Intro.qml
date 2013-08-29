
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
                text: qsTr("Controls you define yourself in C++ that inherrit from CustomControl need special attention.\n\nView the before page and the after page with screen reader on. Often, custom controls written in C++ are very complicated and A11y details need to be explicitly handled. \n\nThe Solution is to be aware that A11y APIs are available in C++ and to think about it early in the design phase.") + Retranslate.onLocaleOrLanguageChanged
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
            source: "CustomCtrlInCPlusPlus_Before.qml"
        }
    ]

}
