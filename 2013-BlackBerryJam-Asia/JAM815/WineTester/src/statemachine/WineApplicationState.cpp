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
 * WineApplicationState.cpp
 *
 *  Created on: Apr 10, 2013
 *      Author: mgoulet
 */

#include "WineApplicationState.h"

WineApplicationState::WineApplicationState(bb::cascades::Application* app)
: m_application(app), m_qmlDocument(NULL), m_applicationRoot(NULL)
{

}

WineApplicationState::~WineApplicationState()
{
    delete m_qmlDocument;
}

void WineApplicationState::onEntry(QEvent* e)
{
    if (m_application) {
        GENERAL_LOG(
                QString("SUCCESS: %1 state entered.").arg(metaObject()->className()));
    } else {
        GENERAL_LOG(
                QString("ERROR: %1 cannot start, missing application handle.").arg(metaObject()->className()));
    }
}

void WineApplicationState::onExit(QEvent* e)
{
    if (m_application) {
        GENERAL_LOG(
                QString("SUCCESS: %1 state exited.").arg(metaObject()->className()));
    }
}

bb::cascades::QmlDocument* WineApplicationState::loadQml(const QString& filePath)
{

    bb::cascades::QmlDocument* qmlDocument = QmlDocument::create(filePath);

    if (qmlDocument && qmlDocument->hasErrors()) {
        foreach (QDeclarativeError error, qmlDocument->errors()){
            GENERAL_LOG(QString("QML Error: %1").arg( error.toString()));
        }
    }

    return qmlDocument;
}
