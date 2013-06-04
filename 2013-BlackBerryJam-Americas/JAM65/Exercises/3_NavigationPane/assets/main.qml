import bb.cascades 1.0

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

