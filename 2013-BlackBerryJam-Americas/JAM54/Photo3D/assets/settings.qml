/* Copyright (c) 2013 SurF-X Interactive
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
        id: settingsPage

	    titleBar: TitleBar {
            id: settingsBar
            title: "Settings"
            visibility: ChromeVisibility.Visible
        }

		content:
	    ScrollView {
        Container {
            id: settingsPane
            property real margins: 40
            topPadding: viewPane.margins
            leftPadding: viewPane.margins
            rightPadding: viewPane.margins

            layout: StackLayout {
            }

            DropDown {
                title: "Stereoscopic Output"
                enabled: true
                topMargin: viewPane.margins
                bottomMargin: topMargin

                onSelectedIndexChanged: {
                    console.log("SelectedIndex was changed to " + selectedIndex);
                }

                // image + text + description
                Option {
                    //imageSource: "asset:///alarm.png"
                    text: "Top / Bottom (Left - Right)"
                    description: "Top shows Left photo and bottom shows Right photo"
                    value: "tbLR"
                    selected: true

                    onSelectedChanged: {
                        if (selected == true) {
                            console.log("Setting stereoscopic mode to top / bottom (L - R)");
                            _photo3D_HDMI.setDisplayMode(0);
                        }
                    }
                }

                // text + description
                Option {
                    text: "Side by Side (Left - Right)"
                    description: "Left and Right photos side by side"
                    value: "tbRL"

                    onSelectedChanged: {
                        if (selected == true) {
                            console.log("Setting stereoscopic mode to side by side (L - R)");
                            _photo3D_HDMI.setDisplayMode(1);
                        }
                    }
                }

                // text + description
                Option {
                    text: "Anaglyph (Red - Cyan)"
                    description: "Left photo is shown in Red and Right photo is shown in Cyan"
                    value: "tbRL"

                    onSelectedChanged: {
                        if (selected == true) {
                            console.log("Setting stereoscopic mode to anaglyph (R - C)");
                            _photo3D_HDMI.setDisplayMode(2);
                        }
                    }
                }
//VIEW_3D_TYPE_ANAGLYPH_CYAN_RED, VIEW_3D_TYPE_ANAGLYPH_RED_GREEN, VIEW_3D_TYPE_ANAGLYPH_GREEN_RED, VIEW_3D_TYPE_ANAGLYPH_BLUE_YELLOW, VIEW_3D_TYPE_ANAGLYPH_YELLOW_BLUE, VIEW_3D_TYPE_ANAGLYPH_CLEAR_VIOLET } VIEW_3D_TYPE;

            }
        } // Container
    } // ScrollView


} // end of Page
