/* Copyright (c) 2013 Research In Motion Limited.
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
#include "citymodel.h"
#include "favoritemodel.h"

#include <bb/cascades/TabbedPane>
#include <bb/cascades/QmlDocument>


const QUrl WeatherHistoryApp::mDefaultServerUrl("http://localhost:8080/WeatherService/");
const bool WeatherHistoryApp::mDefaultSimulateProblems(false);
const QString WeatherHistoryApp::SERVER_URL_SETTINGS_KEY("serverUrl");
const QString WeatherHistoryApp::SIMULATE_PROBLEMS_SETTINGS_KEY("simulateProblems");


WeatherHistoryApp::WeatherHistoryApp()
{
    // Set up the application organization and name (used by QSettings
    // when saving values to the persistent store)
    // In this application, the home page is kept in the settings.
    QCoreApplication::setOrganizationName("Example");
    QCoreApplication::setApplicationName("Weather History");

    // Now that the QSettings are set up, store the default values
    QSettings settings;

    if (settings.value(SERVER_URL_SETTINGS_KEY).isNull()) {
    	setServerUrl(mDefaultServerUrl);
	}

    if (settings.value(SIMULATE_PROBLEMS_SETTINGS_KEY).isNull()) {
    	setSimulateProblems(mDefaultSimulateProblems);
    }

    // Create a QMLDocument and load it, using build patterns.
    mQmlDocument = QmlDocument::create("asset:///main.qml");
    mQmlDocument->setParent(this);
    mQmlDocument->setContextProperty("_app", this);

    // Create the cities and weather model (these are pages which is not part of the
    // NavigationPane, so the application will need handle navigating to them).
    createCitiesModel();

    // Setup the favorite page, setup the home page, load the models, and connect to the appropriate signals.
    createFavoritesModel();

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

void WeatherHistoryApp::createCitiesModel()
{
    // Create and bind the cityModel, so that it can be accessed in the ContinentCitiesPage.QML
    // via the _cityModel context property
    CityModel *cityModel = new CityModel(QStringList() << "name", this);
    mQmlDocument->setContextProperty("_cityModel", cityModel);
    connect(this, SIGNAL(serverUrlChanged(QUrl)), cityModel, SLOT(reset()));
}

void WeatherHistoryApp::createFavoritesModel()
{
    // Create a CityModel that will load the favorite cities which are presented
    // in a list from FavoritePage.qml.
    FavoriteModel *favoriteModel = new FavoriteModel(QStringList() << "name", "favorites_connection", this);
    mQmlDocument->setContextProperty("_favoriteModel", favoriteModel);
}

QUrl WeatherHistoryApp::serverUrl()
{
    return QSettings().value(SERVER_URL_SETTINGS_KEY, QUrl("serverUrl not set!")).toUrl();
}

void WeatherHistoryApp::setServerUrl(QUrl url)
{
    QUrl oldUrl = serverUrl();
    QSettings().setValue(SERVER_URL_SETTINGS_KEY, url);

    if (url != oldUrl) {
        // Emitting this signal will reset all models
        emit serverUrlChanged(url);
    }
}

bool WeatherHistoryApp::simulateProblems()
{
    return QSettings().value(SIMULATE_PROBLEMS_SETTINGS_KEY, false).toBool();
}

void WeatherHistoryApp::setSimulateProblems(bool simulate)
{
    bool oldSimulate = simulateProblems();
    QSettings().setValue(SIMULATE_PROBLEMS_SETTINGS_KEY, simulate);

    if (oldSimulate != simulate) {
        emit simulateProblemsChanged(simulate);
    }
}

QUrl WeatherHistoryApp::prepareServerUrl(const QString& resource)
{
    // Constructs the url e.g. http://localhost:8080/WeatherService/resources/cities/Europe/London/
	QSettings settings;
	QString url = settings.value(SERVER_URL_SETTINGS_KEY, "serverUrl not set!").toUrl().toString(QUrl::StripTrailingSlash);

	if (resource.at(0) != '/') {
		url.append('/');
	}
	url.append(resource);

	QUrl retval(url);
	bool simulateProblems = settings.value(SIMULATE_PROBLEMS_SETTINGS_KEY, mDefaultSimulateProblems).toBool();

	if (simulateProblems) {
		retval.addQueryItem("problems", "yes%20please");
	}

	// Set the number of items to load
	retval.addQueryItem("size", QString("%1").arg(20));

	return retval;
}
