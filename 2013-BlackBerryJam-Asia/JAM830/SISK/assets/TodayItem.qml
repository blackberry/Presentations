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

MyItem {
    verticalAlignment: VerticalAlignment.Center
    horizontalAlignment: HorizontalAlignment.Center
    function daysTo(dob) {
        var measureDays = function(dateObj) {
            var toreturn = 31 * dateObj.getMonth() + dateObj.getDate();
            if (toreturn <= 0) {
                toreturn = 999; //toreturn-365;
            }
            return toreturn
        };
        
        var d = new Date(dob), now = new Date();
        
        var toReturn = measureDays(d) - measureDays(now)
        
        if (toReturn < 0)
            toReturn = 365 - Math.abs(toReturn);
        return toReturn
    }
    Container {
        layout: StackLayout {
            orientation: LayoutOrientation.LeftToRight
        }
        verticalAlignment: VerticalAlignment.Center
        
        Label {
            minWidth: 36 + 60 - 18
            text: daysTo(ListItemData.date)
            textStyle.fontSize: FontSize.PointValue
            textStyle.fontSizeValue: 9.0
            textStyle.textAlign: TextAlign.Center
            textStyle.fontWeight: FontWeight.Bold
            textStyle.lineHeight: 0.0
        }
        Label {
            textFormat: TextFormat.Html
            text: "<html>Days to <b>" + ListItemData.title + "</b></html>"
            textStyle.fontSize: FontSize.PointValue
            textStyle.lineHeight: 0.0
            textStyle.fontStyle: FontStyle.Default
            textStyle.fontSizeValue: 9.0
            textStyle.textAlign: TextAlign.Left
        }
    }
}