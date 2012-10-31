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

import bb.cascades 1.0
import "common"

Container {
    topPadding: 20
    leftPadding: 10
    Label {
        text: qsTr ("Tap for Default Action")
        horizontalAlignment: HorizontalAlignment.Center
        textStyle {
            base: SystemDefaults.TextStyles.TitleText
            fontWeight: FontWeight.Bold
        }
    }
    Label {
        text: qsTr ("Long Press for More Options")
        horizontalAlignment: HorizontalAlignment.Center
        textStyle {
            base: SystemDefaults.TextStyles.TitleText
            fontWeight: FontWeight.Bold
        }
    }
    Container {
        background: Color.White
        Divider {}
    }
    Container {
        horizontalAlignment: HorizontalAlignment.Center
        topPadding: 40
        Container {
            layout: StackLayout {
                orientation: LayoutOrientation.LeftToRight
            }
            verticalAlignment: VerticalAlignment.Center
            ContentImage {
                imgSource: "asset:///images/pdf.png"
                mime: "application/pdf"
                uri: "file:////accounts/1000/shared/documents/Getting Started with Adobe Reader.pdf"
            }
            ContentImage {
                imgSource: "asset:///images/mp3.png"
                mime: "audio/mp3"
                uri: "music://songs?play=1"
            }
            ContentImage {
                imgSource: "asset:///images/jpg.png"
                mime: "image/jpeg"
                uri: "file:///accounts/1000/shared/photos/jpg/sample1.jpg"
            }
            ContentImage {
                imgSource: "asset:///images/png.png"
                mime: "image/png"
                uri: "file:///accounts/1000/shared/photos/png/sample2.png"
            }
            ContentImage {
                imgSource: "asset:///images/bmp.png"
                mime: "image/bmp"
                uri: "file:///accounts/1000/shared/photos/bmp/sample3.bmp"
            }
        }
        Container {
            layout: StackLayout {
                orientation: LayoutOrientation.LeftToRight
            }
            verticalAlignment: VerticalAlignment.Center
            topPadding: 50
            ContentImage {
                imgSource: "asset:///images/gif.png"
                mime: "image/gif"
                uri: "file:///accounts/1000/shared/photos/gif/sample4.gif"
            }
            ContentImage {
                imgSource: "asset:///images/tif.png"
                mime: "image/tif"
                uri: "file:///accounts/1000/shared/photos/tif/sample1.tif"
            }
            ContentImage {
                imgSource: "asset:///images/ppt.png"
                mime: "application/vnd.ms-powerpoint"
                uri: "file:////accounts/1000/shared/documents/sample1.ppt"
            }
            ContentImage {
                imgSource: "asset:///images/xls.png"
                mime: "application/vnd.ms-excel"
                uri: "file:////accounts/1000/shared/documents/sample1.xls"
            }
            ContentImage {
                imgSource: "asset:///images/vid_mp4.png"
                mime: "video/mp4"
                uri: "file:///accounts/1000/shared/videos/PlayBook HiDef Demo - clipped.mp4"
            }
        }
        Container {
            layout: StackLayout {
                orientation: LayoutOrientation.LeftToRight
            }
            verticalAlignment: VerticalAlignment.Center
            topPadding: 50
            ContentImage {
                imgSource: "asset:///images/http.png"
                mime: "application/xhtml+xml"
                uri: "http://www.rim.com"
            }
        }
    }
}
