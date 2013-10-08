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
    
    // 1. Add the first Page in the Navigation Pane the one that will be seen
    // when the application launches. Use the StartScreen QML component. 
    StartScreen {

    }

    // 2. Add a the HighScoreScreen as a ComponentDefinition.
    // 3. Open StarScreen.qml and an onClicked: { } signal handlers to the Highscore Button
    // 4. Create the HighScoreScreen via its ComponentDefinition (use the id and the createObject() function) 
    // 5. Finally push the HighScoreScreen in the onClicked: { } signal handler you added. 
    attachedObjects: ComponentDefinition {
        id: highScoreScreen
        source: "HighScoreScreen.qml"
    }
    
    // Transition is done destory the Page to free up memory.
    onPopTransitionEnded: {
        page.destroy();
    }
}
