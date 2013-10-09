
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

NavigationPane {
    id: nav

    // 1. main.qml - Add the first Page in the Navigation Pane the one that will be seen
    // when the application launches. Use the StartScreen QML component. 

    // 2. main.qml - Add a the HighScoreScreen as a ComponentDefinition.
    // 3. StarScreen.qml - Open StarScreen.qml and add an onClicked: { } signal handler to the Highscore Button
    // 4. StarScreen.qml - In the signal handler create the HighScoreScreen via its ComponentDefinition. 
    //    Use the ComponentDefinitions id and the createObject() function.
    // 5. StarScreen.qml - Finally push the HighScoreScreen in onClicked: { } 
    //    (use the navigaion id nav to access the NavigaitonPane control)
}

