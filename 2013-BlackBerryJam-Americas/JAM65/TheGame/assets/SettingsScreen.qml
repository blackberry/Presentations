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

TabbedPane {
    showTabsOnActionBar: true
    signal closeSheet()
    Tab {
        title: "Audio"
        imageSource: "asset:///images/settingsAudio.png"
        SettingsPage {
            onCloseButtonPressed: {
                closeSheet();
            }
            SettingsScreen_Audio {
            }
        }
    }
    Tab {
        title: "Video"
        imageSource: "asset:///images/settingsVideo.png"
        SettingsPage {
            onCloseButtonPressed: {
                closeSheet();
            }
            SettingsScreen_Video {
            }
        }
    }
    Tab {
        title: "Game"
        imageSource: "asset:///images/settingsGame.png"
        SettingsPage {
            onCloseButtonPressed: {
                closeSheet();
            }
            SettingsScreen_Game {
            }
        }
    }
}
