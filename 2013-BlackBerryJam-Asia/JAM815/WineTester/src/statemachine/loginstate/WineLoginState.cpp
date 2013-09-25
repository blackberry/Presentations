/*
 * Copyright 2013 BlackBerry Limited.
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

/*
 * WineLoginState.cpp
 *
 *  Created on: Apr 10, 2013
 *      Author: mgoulet
 */

#include <bb/cascades/Application>
#include <bb/cascades/QmlDocument>
#include <bb/cascades/AbstractPane>
#include <bb/cascades/Button>
#include <bb/cascades/Label>
#include "statemachine/loginstate/WineLoginState.h"

using namespace bb::cascades;

WineLoginState::WineLoginState(bb::cascades::Application* app)
:WineApplicationState(app)
{

}

WineLoginState::~WineLoginState()
{

}

void WineLoginState::onEntry(QEvent* e)
{
	GENERAL_LOG("WineLoginState::onEntry");

	// call parent state onEntry routine explicitly
    WineApplicationState::onEntry(e);

    // load our QML document
    m_qmlDocument = loadQml("asset:///LoginPage.qml");

    // set context property
    m_qmlDocument->setContextProperty("_loginState", this);

    // create root object for the UI
    m_applicationRoot = m_qmlDocument->createRootObject<AbstractPane>();

    // set created root object as a scene
    m_application->setScene(m_applicationRoot);

}

void WineLoginState::onExit(QEvent* e)
{
    GENERAL_LOG("WineLoginState::onExit");

    //Cleanup QML tree and states here
    delete m_qmlDocument;


}

void WineLoginState::loginRequest()
{
	emit loginSuccessful();
}
