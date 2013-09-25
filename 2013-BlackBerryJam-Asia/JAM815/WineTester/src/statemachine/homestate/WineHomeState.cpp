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
 * WineHomeState.cpp
 *
 *  Created on: Apr 10, 2013
 *      Author: mgoulet
 */

#include <bb/cascades/Application>
#include <bb/cascades/QmlDocument>
#include <bb/cascades/AbstractPane>
#include <bb/cascades/ActionItem>

#include <bb/data/JsonDataAccess>
#include <bb/data/DataSource>

#include "WineHomeState.h"
#include "utilities.h"
#include "../../features.hpp"
#include "GeneralLogger.h"

using namespace bb::data;
using namespace bb::cascades;

WineHomeState::WineHomeState(bb::cascades::Application* app)
:WineApplicationState(app)
{
	//DO NOTHING
}

WineHomeState::~WineHomeState()
{
	//DO NOTHING
}

void WineHomeState::onEntry(QEvent* e)
{
	bb::data::DataSource::registerQmlTypes();

	m_whiteWinesDataModel = new bb::cascades::ArrayDataModel();
	m_redWinesDataModel = new bb::cascades::ArrayDataModel();

    WineApplicationState::onEntry(e);

    // create scene document from WineTabbedPane.qml asset
    // set parent to created document to ensure it exists for the whole application lifetime
    m_qmlDocument = loadQml("asset:///WineTabbedPane.qml");

    // set context property for this state
    m_qmlDocument->setContextProperty("_homeState", this);

    // set context property for the QML features
    m_qmlDocument->setContextProperty("_features", &Features::getInstance());


    // create root object for the UI
    m_applicationRoot = m_qmlDocument->createRootObject<AbstractPane>();

    // set created root object as a scene
    m_application->setScene(m_applicationRoot);

    // Load our data for the white wine lists
    loadWhiteWines();

    // Load our data for the red wine lists
    loadRedWines();

}

void WineHomeState::onExit(QEvent* e)
{
	delete m_qmlDocument;
}

void WineHomeState::logoutRequest()
{
	emit logoutSuccessful();
}

bb::cascades::DataModel* WineHomeState::getWhiteWinesDataModel() const
{
    return m_whiteWinesDataModel;
}

bb::cascades::DataModel* WineHomeState::getRedWinesDataModel() const
{
    return m_redWinesDataModel;
}

void WineHomeState::loadWhiteWines()
{

	JsonDataAccess jda;
    QString filePath = QDir::currentPath()+ QString("/app/native/assets/testdata/whitewines.json");
    QVariant data = jda.load(filePath);

	if (jda.hasError()) {
		// Get the error
		DataAccessError error = jda.error();

		// Determine type of error
		if (error.errorType() == DataAccessErrorType::SourceNotFound) {
			GENERAL_LOG("Error loading: " + error.errorMessage());
		} else if (error.errorType() == DataAccessErrorType::ConnectionFailure) {
			GENERAL_LOG("Connection failure: " + error.errorMessage());
		} else if (error.errorType() == DataAccessErrorType::OperationFailure) {
			GENERAL_LOG("Operation failure: " + error.errorMessage());
		}

	} else {
		//m_whiteWinesDataModel = data.toMap();
		Utilities::updateDataModel(data.toList(), m_whiteWinesDataModel);
	}

}

void WineHomeState::loadRedWines()
{

	JsonDataAccess jda;
    QString filePath = QDir::currentPath()+ QString("/app/native/assets/testdata/redwines.json");
    QVariant data = jda.load(filePath);

	if (jda.hasError()) {
		// Get the error
		DataAccessError error = jda.error();

		// Determine type of error
		if (error.errorType() == DataAccessErrorType::SourceNotFound) {
			GENERAL_LOG("Error loading: " + error.errorMessage());
		} else if (error.errorType() == DataAccessErrorType::ConnectionFailure) {
			GENERAL_LOG("Connection failure: " + error.errorMessage());
		} else if (error.errorType() == DataAccessErrorType::OperationFailure) {
			GENERAL_LOG("Operation failure: " + error.errorMessage());
		}

	} else {
		//m_whiteWinesDataModel = data.toMap();
		Utilities::updateDataModel(data.toList(), m_redWinesDataModel);
	}

}

