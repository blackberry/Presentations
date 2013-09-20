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
 * WineStateMachine.cpp
 *
 *  Created on: Apr 10, 2013
 *      Author: mgoulet
 */

#include <bb/cascades/Application>
#include <bb/cascades/QmlDocument>

#include "WineStateMachine.h"
#include "WineApplicationState.h"
#include "WineLoginState.h"
#include "WineHomeState.h"

//Include components that may be used by many qml trees
#include "../../customcomponents/qmltimer/QmlTimer.h"

WineStateMachine::WineStateMachine(bb::cascades::Application* app)
:m_applicationHandle(app)
,m_loginState(NULL)
,m_mainState(NULL)
{

	//Register properties that can be global to all QML trees.
	qmlRegisterType<QmlTimer>("CustomTimer", 1, 0, "QmlTimer");

	//Build our login state
	m_loginState = new WineLoginState(m_applicationHandle);
	this->addState(m_loginState);

	//Build our home state
	m_mainState = new WineHomeState(m_applicationHandle);
	this->addState(m_mainState);

	this->setInitialState(m_loginState);	//initial state

	//connect transitions
	m_loginState->addTransition(m_loginState, SIGNAL(loginSuccessful()), m_mainState);
	m_mainState->addTransition(m_mainState, SIGNAL(logoutSuccessful()), m_loginState);

}

WineStateMachine::~WineStateMachine()
{
	//cleanup
	delete m_loginState;
	delete m_mainState;
}
