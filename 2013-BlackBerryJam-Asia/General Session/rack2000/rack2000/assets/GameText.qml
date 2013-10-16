/* Copyright (c) 2013 BlackBerry.
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

Container {
    property bool monoChrome: false
    layout: DockLayout {
    }
    ImageView {
        id: defaultLogoImage
        imageSource: "asset:///images/logo-color.png"
        horizontalAlignment: HorizontalAlignment.Center
    }

    ImageView {
        id: monoChromeImage
        imageSource: "asset:///images/logo-white2.png"
        opacity: 0
        horizontalAlignment: HorizontalAlignment.Center
    }
    
    onMonoChromeChanged: {
        if(monoChrome) {
            defaultLogoImage.opacity = 0;
            monoChromeImage.opacity = 1;
        } else {
            defaultLogoImage.opacity = 1;
            monoChromeImage.opacity = 0;
        }
    }
}
