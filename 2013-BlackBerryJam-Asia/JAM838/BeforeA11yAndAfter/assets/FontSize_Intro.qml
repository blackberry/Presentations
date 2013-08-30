
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
                text: qsTr("Note that both in before and after pages, there is a one to one correspondence in the font sizes. \n\nGo to the before page, thumbnail this application, go to settings, go to Display and set the font size to something different. Note that the hardcoded font sizes specified in points on the before page will not change, but the ones specified by FontSize's on the after page do change. Point size specified fonts are not friendly for people who are hard of sight and need to see text in larger fonts. \n\nThe Solution is to use FontSize and not point size.") + Retranslate.onLocaleOrLanguageChanged
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
            source: "FontSize_Before.qml"
        }
    ]

}
