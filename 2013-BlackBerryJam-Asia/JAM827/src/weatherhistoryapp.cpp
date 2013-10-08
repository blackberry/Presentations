/* Copyright (c) 2013 BlackBerry Limited.
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
#include "weatherhistoryapp.h"
#include "appsettings.h"

#include "common/loadmodeldecorator.h"
#include "common/sqlheaderdataqueryex.h"

#include "data/citydatasource.h"
#include "data/weatherdatasource.h"

#include <bb/cascades/LocaleHandler>
#include <bb/cascades/TabbedPane>
#include <bb/cascades/QmlDocument>

WeatherHistoryApp::WeatherHistoryApp()
{
	qmlRegisterType<SqlHeaderDataQueryEx>("bb.cascades.datamanager", 1, 2, "SqlHeaderDataQueryEx");
	qmlRegisterType<WeatherDataSource>("utils", 1, 0, "WeatherDataSource");
	qmlRegisterType<CityDataSource>("utils", 1, 0, "CityDataSource");
	qmlRegisterType<LoadModelDecorator>("utils", 1, 0, "LoadModelDecorator");


    // Prepare localization.Connect to the LocaleHandlers systemLanguaged change signal, this will
    // tell the application when it is time to load a new set of language strings.
    mTranslator = new QTranslator(this);
    mLocaleHandler = new LocaleHandler(this);
    connect(mLocaleHandler, SIGNAL(systemLanguageChanged()), SLOT(onSystemLanguageChanged()));
    onSystemLanguageChanged();

    // Create a QMLDocument and load it, using build patterns.
    mQmlDocument = QmlDocument::create("asset:///main.qml").parent(this);

    // The application settings object that stores application wide persistent data.
    mAppSettings = new AppSettings(this);

    mQmlDocument->setContextProperty("_app", this);
    mQmlDocument->setContextProperty("_appSettings", mAppSettings);

    if (!mQmlDocument->hasErrors()) {

        // The application navigationPane is created from QML.
        TabbedPane *tabs = mQmlDocument->createRootObject<TabbedPane>();

        if (tabs) {
            // Set the main application scene to NavigationPane.
            Application::instance()->setScene(tabs);
        }
    }
}

WeatherHistoryApp::~WeatherHistoryApp()
{
}

void WeatherHistoryApp::onSystemLanguageChanged()
{
    // Remove the old translator to make room for the new translation.
     QCoreApplication::instance()->removeTranslator(mTranslator);

     // Initiate, load and install the application translation files.
     QString localeString = QLocale().name();
     QString fileName = QString("weatherhistory_%1").arg(localeString);
     if (mTranslator->load(fileName, "app/native/qm")) {
         QCoreApplication::instance()->installTranslator(mTranslator);
     }
}

QString WeatherHistoryApp::getHomeDirectory() {
    return QDir::homePath();
}
