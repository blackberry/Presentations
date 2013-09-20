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

#ifndef WINESTATEMACHINE_H
#define WINESTATEMACHINE_H

#include <map>
#include <QObject>
#include <QStateMachine>
#include <QAbstractState>
#include <QFinalState>

class WineApplicationState;
class WineLoginState;
class WineHomeState;

/*
 * This is the main state machine for the application.
 */
class WineStateMachine: public QStateMachine
{

public:

	WineStateMachine(bb::cascades::Application* app);
	~WineStateMachine();

protected:

	//This is the main handle for the application.
	bb::cascades::Application* m_applicationHandle;

	//These are the main sub states for our application.
	WineLoginState* m_loginState;
	WineHomeState* m_mainState;

};


#endif /* WINESTATEMACHINE_H */

