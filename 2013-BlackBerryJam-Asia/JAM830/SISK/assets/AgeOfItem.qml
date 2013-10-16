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
    leftPadding: 36 + 60 + 36
    
    verticalAlignment: VerticalAlignment.Center
    horizontalAlignment: HorizontalAlignment.Center
    
    function getAge(dob) {
        var measureDays = function(dateObj) {
            return 31 * dateObj.getMonth() + dateObj.getDate();
        }, d = new Date(dob), now = new Date();
        var toReturn = now.getFullYear() - d.getFullYear() - (measureDays(now) < measureDays(d));
        return toReturn;
    }
    Label {
        text: "<html> " + ListItemData.title + " is <b>" + getAge(ListItemData.date) + "</b> years.</html>"
        verticalAlignment: VerticalAlignment.Center
        textStyle.fontSize: FontSize.PointValue
        textStyle.lineHeight: 0.0
        textStyle.fontStyle: FontStyle.Default
        textStyle.fontSizeValue: 9.0
    }
}