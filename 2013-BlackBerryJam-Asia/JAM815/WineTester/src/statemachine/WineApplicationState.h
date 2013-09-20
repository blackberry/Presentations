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
 * WineApplicationState.h
 *
 *  Created on: Apr 10, 2013
 *      Author: mgoulet
 */

#ifndef WINEAPPLICATIONSTATE_H_
#define WINEAPPLICATIONSTATE_H_

#include <QState>
#include <bb/cascades/Application>
#include <bb/cascades/QmlDocument>
#include "GeneralLogger.h"

using namespace bb::cascades;

/*
 * this class serves as a generic State class for the application.
 * States can be nested and map to their own view controllers and view models.
 */
class WineApplicationState: public QState
{
    Q_OBJECT

public:
    explicit WineApplicationState(bb::cascades::Application* app);
    virtual ~WineApplicationState();

    virtual void onEntry(QEvent* e);
    virtual void onExit(QEvent* e);

protected:

	//Loads the qml document and displays errors if there was any
    bb::cascades::QmlDocument* loadQml(const QString& filePath);

    //Handle on the application object
    bb::cascades::Application *m_application;

    bb::cascades::QmlDocument *m_qmlDocument;

    //Handle on the applicaiton root.
    bb::cascades::AbstractPane *m_applicationRoot;

private:
};


#endif /* WINEAPPLICATIONSTATE_H_ */
