/* Copyright (c) 2012 Research In Motion Limited.
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
/*


Attributions

Images:
Cricket by Pavan Kunder at Flickr
Drum by Vladimir Morozov at Flickr
Clap by Anodoin at Flickr

Sound:
Cricket http://www.freesound.org/people/kvgarlic/sounds/134772/

*/


import bb.cascades 1.0

TabbedPane {
    id: tabbedPane
    showTabsOnActionBar: true
    Tab {
        title: "Rimshot"
        imageSource: "images/in_app_icon_rimshot.png"
        Soundpage {
            theImagePath: "asset:///images/drums.png"
            theSound: "rimshot.wav"
        }
    }
    Tab {
        title: "Clap"
        imageSource: "images/in_app_icon_clap.png"
        Soundpage {
            theImagePath: "asset:///images/clap.png"
            theSound: "clap.wav"
        }
    }
    Tab {
        title: "Cricket"
        imageSource: "images/in_app_icon_cricket.png"
        Soundpage {
            theImagePath: "asset:///images/cricket.png"
            theSound: "cricket.wav"
        }
    }
    Tab {
        title: "Store"
        imageSource: "images/in_app_icon_appworld.png"
        Store {
        }
    }
    Tab {
        title: "Record"
        imageSource: "images/in_app_icon_record.png"
        Page {
            Label {
                text: "Nothing to see here"
            }
        }
    }
}
