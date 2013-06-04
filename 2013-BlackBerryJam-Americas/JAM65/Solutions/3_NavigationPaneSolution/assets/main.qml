import bb.cascades 1.0

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
