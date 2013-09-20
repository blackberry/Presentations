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

#ifndef WINEHOMESTATE_H_
#define WINEHOMESTATE_H_

#include "WineApplicationState.h"
#include <bb/cascades/ArrayDataModel>

/*
 * This is the main application state. It will contain the majority of
 * interactions with the user and platform.
 */
class WineHomeState: public WineApplicationState
{
	Q_OBJECT

public:
	explicit WineHomeState(bb::cascades::Application* app);
	virtual ~WineHomeState();

	Q_INVOKABLE void logoutRequest();

	//Properties for listviews
	Q_PROPERTY(bb::cascades::DataModel* whiteWinesDataModel READ getWhiteWinesDataModel CONSTANT)
	bb::cascades::DataModel* getWhiteWinesDataModel() const;

	//Properties for listviews
	Q_PROPERTY(bb::cascades::DataModel* redWinesDataModel READ getRedWinesDataModel CONSTANT)
	bb::cascades::DataModel* getRedWinesDataModel() const;

Q_SIGNALS:
	void logoutSuccessful();

protected:

	virtual void onEntry(QEvent* e);
	virtual void onExit(QEvent* e);

private:

	/**
	 * DataModel used to display white wines
	 */
	bb::cascades::ArrayDataModel* m_whiteWinesDataModel;

	/**
	 * DataModel used to display red wines
	 */
	bb::cascades::ArrayDataModel* m_redWinesDataModel;

	/*
	 * Method used to load white wines from JSON file.
	 */
	void loadWhiteWines();

	/*
	 * Method used to load white wines from JSON file.
	 */
	void loadRedWines();
};


#endif /* WINEHOMESTATE_H_ */
