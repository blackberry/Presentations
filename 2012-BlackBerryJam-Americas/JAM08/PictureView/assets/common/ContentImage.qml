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
import com.sample.invoke 1.0

Container {
    id: root

    background: Color.Blue
    preferredWidth: 106
    preferredHeight: 106
    rightMargin: 20

    property string mime: ""
    property string uri: ""
    property string imgSource: ""
    property bool menu: false
ImageView {

    leftMargin: 30
    preferredWidth: 100
    preferredHeight: 100
    verticalAlignment: VerticalAlignment.Center
    horizontalAlignment: HorizontalAlignment.Center
    imageSource:root.imgSource
    contextActions: [
        ActionSet {
            title: qsTr ("Content")
            InvokeActionItem {
                query {
                    mimeType: root.mime
                    uri: root.uri
                }
            }
        }
    ]
    contextMenuHandler: ContextMenuHandler {
        onShowing: {
            console.debug(event.toDebugString())
            if (event.toDebugString().indexOf("ContextMenuHidden") == -1) {
                console.debug("context menu open")
                root.menu = true
            }
        }
    }
    onTouch: {
        if(event.isDown()) {
            console.debug("is down")
            root.menu = false
        }

        if (event.isUp() && !root.menu) {
            console.debug("is up")
           //_imanager.defaultHandlerInvoke(mime, uri, "")
            contentInvoker.invoke()
        }
    }
    attachedObjects: [
        Target {
            id: contentInvoker
            visible: false
            target: ""
            action: ""
            mime: root.mime
            uri: root.uri
            data: ""
        }
    ]
}
}
