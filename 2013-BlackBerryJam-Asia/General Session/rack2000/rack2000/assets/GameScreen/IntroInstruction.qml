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
import "../"

Container {
    property alias message: messageLabel.text
    horizontalAlignment: HorizontalAlignment.Center
    verticalAlignment: VerticalAlignment.Center

    Label {
        id: messageLabel
        topMargin: 80
        text: qsTr("<html><span style='color:#ffffff;font-size:14;font-weight:100'>WAITING FOR</span>\n<span style='font-size:20;color:#fc9359;'>PLAYERS</span></html>")
        horizontalAlignment: HorizontalAlignment.Center
        multiline: true
        
        textStyle {
            color: Color.create("#ffffff")
            textAlign: TextAlign.Center
        }
    }
}