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
 * WineLoginState.h
 *
 *  Created on: Apr 10, 2013
 *      Author: mgoulet
 */

#ifndef WINELOGINSTATE_H_
#define WINELOGINSTATE_H_

#include "statemachine/WineApplicationState.h"

/*
 * this state encapsulates the login process for hte entire application.
 * For more information, see substate classes.
 */
class WineLoginState: public WineApplicationState
{
	Q_OBJECT

public:

	explicit WineLoginState(bb::cascades::Application* app);
	virtual ~WineLoginState();

	Q_INVOKABLE void loginRequest();

Q_SIGNALS:
	void loginSuccessful();

protected:

	virtual void onEntry(QEvent* e);
	virtual void onExit(QEvent* e);

private:

};

#endif /* WINELOGINSTATE_H_ */
