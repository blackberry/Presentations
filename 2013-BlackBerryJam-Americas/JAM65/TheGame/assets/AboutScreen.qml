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

Page {
    content: Container {
        layout: DockLayout {
        }
        Background {
        }

        //Content
        Container {
            horizontalAlignment: HorizontalAlignment.Fill
            verticalAlignment: VerticalAlignment.Fill
            GameHeader {
                title: "About"
            }
            ScrollView {
                horizontalAlignment: HorizontalAlignment.Fill
                layoutProperties: StackLayoutProperties {
                    spaceQuota: 1
                }
                scrollViewProperties {
                    scrollMode: ScrollMode.Vertical
                }
                Container {
                    leftPadding: 20
                    rightPadding: 20
                    Label {
                        textStyle.color: Color.White
                        textStyle.base: SystemDefaults.TextStyles.BigText
                        text: "Why"
                    }
                    TextArea {
                        textStyle.color: Color.White
                        textStyle.base: SystemDefaults.TextStyles.BodyText
                        backgroundVisible: false
                        editable: false
                        text: "Because we, the CFA team, wanted to try to create a Cascades application ourselves and at the same time create a good reference for you guys."
                    }
                    Label {
                        textStyle.color: Color.White
                        textStyle.base: SystemDefaults.TextStyles.BigText
                        text: "Who"
                    }
                    ImageView {
                        imageSource: "asset:///images/success.png"
                        pivotY: -200
                        onTouch: {
                            if (event.isUp()) {
                                soundManager.play("bonus");
                                anim.play();
                            }
                        }
                        animations: [
                            SequentialAnimation {
                                id: anim
                                animations: [
                                    RotateTransition {
                                        toAngleZ: 15
                                        duration: 250
                                        easingCurve: StockCurve.QuadraticOut
                                    },
                                    RotateTransition {
                                        toAngleZ: -5
                                        duration: 400
                                        easingCurve: StockCurve.QuadraticInOut
                                    },
                                    RotateTransition {
                                        toAngleZ: 0
                                        duration: 300
                                        easingCurve: StockCurve.QuadraticInOut
                                    }
                                ]
                            }
                        ]
                    }
                    AboutItem {
                        title: "Anders Jalevik"
                    }
                    AboutItem {
                        title: "Johan Montan"
                    }
                    AboutItem {
                        title: "Jonas Knutsson"
                    }
                    AboutItem {
                        title: "Karl Berggren"
                    }
                    AboutItem {
                        title: "Magnus Johansson"
                    }
                    AboutItem {
                        title: "Mattias Ohman"
                    }
                    AboutItem {
                        title: "Olof Stenlund"
                    }
                    AboutItem {
                        title: "Oskar Apelvi"
                    }
                    AboutItem {
                        title: "Shailesh Kumar"
                    }
                    Label {
                        textStyle.color: Color.White
                        textStyle.base: SystemDefaults.TextStyles.BigText
                        text: "Exist of"
                    }
                    AboutItem {
                        title: "Custom control:"
                        text: "Game screen"
                    }
                    AboutItem {
                        title: "Dropdown:"
                        text: "Settings"
                    }
                    AboutItem {
                        title: "json-database:"
                        text: "Game screen"
                    }
                    AboutItem {
                        title: "Scrollable layout:"
                        text: "About"
                    }
                    AboutItem {
                        title: "SegmentedControl:"
                        text: "Settings"
                    }
                    AboutItem {
                        title: "SQL-list:"
                        text: "Highscores"
                    }
                    TextArea {
                        textStyle.color: Color.White
                        textStyle.base: SystemDefaults.TextStyles.BodyText
                        backgroundVisible: false
                        editable: false
                        text: "Assets stolen from the Scrapbook application. Thank you fellas! 8-)"
                    }
                }
            }
        }
    }
    paneProperties: NavigationPaneProperties {
        backButton: ActionItem {
            onTriggered: {
                soundManager.play("klick2");
                navigationPane.pop().destroy();
            }
        }
    }
}
