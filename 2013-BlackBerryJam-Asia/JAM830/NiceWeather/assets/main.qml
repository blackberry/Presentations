/* Copyright (c) 2013 BlackBerry Limited.
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
    ListView {
        id: citiesList
        // keeps track of what current items is shown on screen.
        property variant firstItem
        // snapping currently very slow. Will be fixed on trunk soon.
        snapMode: SnapMode.LeadingEdge
        flickMode: FlickMode.SingleItem
        scrollIndicatorMode: ScrollIndicatorMode.None
        
        dataModel: XmlDataModel {
            // Workaround due to https://jira.bbqnx.net/browse/CAS-3361 : Can't use flickmode.singleItem and ListHeaderMode.None together
            //source: "asset:///models/model.xml"
            source: "asset:///models/cities.xml"
            
        }
        layout: StackListLayout {
            orientation: LayoutOrientation.LeftToRight
            // Woararound due to CAS-3361
            //headerMode: ListHeaderMode.None
        }
        
        listItemComponents: ListItemComponent {
            type: "city"
            City {
                id: cityItem
                // Woararound due to CAS-3361
                //rootIndexPath : ListItem.indexPath
                modelUrl: ListItemData.modelUrl 
                
                bg: ListItemData.background
                // TODO: Could be replaced with dynamically bluring the image with QImage. No cascades-supprt for this though.
                bg2: ListItemData.altBackground

                // forwarding this property in order to get changed singnalse.
                property variant firstItem: ListItem.view.firstItem
                onFirstItemChanged: {
                    if ("" + firstItem == "" + cityItem.ListItem.indexPath) {
                        cityItem.showAnim();
                    } else {
                        cityItem.hideAnim();
                    }
                }
            }
        }

        attachedObjects: ListScrollStateHandler {
            onScrollingChanged: {
                if (! scrolling) {
                    // race condition check since scolling and firstVisible item is set simultaniously at startup
                    if (firstVisibleItem == "") {
                        citiesList.firstItem = 0
                    } else {
                        citiesList.firstItem = firstVisibleItem;
                    }
                }
            }
        }
    }
}
