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
        orientation: LayoutOrientation.TopToBottom
    }
    Container {
        layout: StackLayout {
        }
        leftPadding: 20
        rightPadding: 20
        topPadding: 20
        bottomPadding: 20
        Label {
            text: "Theme"
            textStyle.base: SystemDefaults.TextStyles.BigText
        }
        RadioGroup {
            id: theme
            Option {
                onCreationCompleted: {
                    selected = settings.theme == "Turtle"
                }
                text: "Turtle"
                onSelectedChanged: {
                    if (selected) {
                        settings.theme = text;
                    }
                }
            }
            Option {
                onCreationCompleted: {
                    selected = settings.theme == "Monkey"
                }
                text: "Monkey"
                
                onSelectedChanged: {
                    if (selected) {
                        settings.theme = text;
                    }
                }
            }
        }
        Divider {
        }
        Label {
            text: "Grid and Size"
            textStyle.base: SystemDefaults.TextStyles.BigText
        }
        Container {
            layout: StackLayout {
                orientation: LayoutOrientation.LeftToRight
            }
            Label {
                text: "Show grid"
                textStyle.base: SystemDefaults.TextStyles.BodyText
                verticalAlignment: VerticalAlignment.Center
            }
            ToggleButton {
                id: showGrid
                checked: settings.getValueFor("show_grid", "true")
                onCheckedChanged: {
                    settings.saveValueFor("show_grid", checked);
                }
            }
        }
        DropDown {
            title: "Game size"
            Option {
                text: "Large"
                value: "120"
                selected: true
            }
            Option {
                text: "Small"
                value: "60"
            }
        }
    }
   
}
