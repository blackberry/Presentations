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
#include "weathermodel.h"

#include <bb/cascades/TabbedPane>
#include <bb/cascades/QmlDocument>


const QUrl WeatherHistoryApp::mDefaultServerUrl("http://localhost:8080/WeatherService/");
const bool WeatherHistoryApp::mDefaultSimulateProblems(false);
const uint WeatherHistoryApp::mDefaultLoadSize(10);
const uint WeatherHistoryApp::mDefaultMaxSize(1024);
const uint WeatherHistoryApp::mDefaultHackThreshold(10);
const QString WeatherHistoryApp::mDefaultHomeRegion("Europe");
const QString WeatherHistoryApp::mDefaultHomeCity("London");
const QString WeatherHistoryApp::SERVER_URL_SETTINGS_KEY("serverUrl");
const QString WeatherHistoryApp::SIMULATE_PROBLEMS_SETTINGS_KEY("simulateProblems");
const QString WeatherHistoryApp::LOAD_SIZE_SETTINGS_KEY("loadSize");
const QString WeatherHistoryApp::MAX_SIZE_SETTINGS_KEY("maxSize");
const QString WeatherHistoryApp::HACK_THRESHOLD_SETTINGS_KEY("hackThreshold");
const QString WeatherHistoryApp::HOME_REGION_KEY("homeRegion");
const QString WeatherHistoryApp::HOME_CITY_KEY("homeCity");


WeatherHistoryApp::WeatherHistoryApp()
{
    // Set up the application organization and name (used by QSettings
    // when saving values to the persistent store)
    // In this application, the home page is kept in the settings.
    QCoreApplication::setOrganizationName("Example");
    QCoreApplication::setApplicationName("Weather History");

    // Now that the QSettings are set up, store the default values
    QSettings settings;

    qmlRegisterUncreatableType <WeatherModel>("bb.cascades", 1, 0, "WeatherModel", "Uncreatable type");

    if (settings.value(SERVER_URL_SETTINGS_KEY).isNull()) {
    	setServerUrl(mDefaultServerUrl);
	}

    if (settings.value(SIMULATE_PROBLEMS_SETTINGS_KEY).isNull()) {
    	setSimulateProblems(mDefaultSimulateProblems);
    }

    if (settings.value(LOAD_SIZE_SETTINGS_KEY).isNull()) {
    	setLoadSize(mDefaultLoadSize);
    }

    if (settings.value(MAX_SIZE_SETTINGS_KEY).isNull()) {
    	setMaxSize(mDefaultMaxSize);
    }

    if (settings.value(HACK_THRESHOLD_SETTINGS_KEY).isNull()) {
    	setHackThreshold(mDefaultHackThreshold);
    }

    if (settings.value(HOME_REGION_KEY).isNull() && settings.value(HOME_CITY_KEY).isNull()) {
        onUpdateHomeRegion(mDefaultHomeRegion);
        onUpdateHomeCity(mDefaultHomeCity);
    }

    // Create a QMLDocument and load it, using build patterns.
    mQmlDocument = QmlDocument::create("asset:///main.qml");
    mQmlDocument->setParent(this);
    mQmlDocument->setContextProperty("_app", this);

    // Create the cities and weather model (these are pages which is not part of the
    // NavigationPane, so the application will need handle navigating to them).
    createCitiesModel();
    createWeatherModel();

    // Setup the favorite page, setup the home page, load the models, and connect to the appropriate signals.
    createFavoritesModel();
    createHomeModel();

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

void WeatherHistoryApp::createWeatherModel()
{
    // Create a WeatherModel that will load a weather forecast based on its
    // city property (see WeatherPage.qml and FavoritePage.qml).
    WeatherModel *weatherModel = new WeatherModel(this);
    mQmlDocument->setContextProperty("_weatherModel", weatherModel);
    connect(this, SIGNAL(serverUrlChanged(QUrl)), weatherModel, SLOT(reset()));
    connect(this, SIGNAL(maxSizeChanged(uint)), weatherModel, SLOT(reset()));
}

void WeatherHistoryApp::createFavoritesModel()
{
    // Create a CityModel that will load the favorite cities which are presented
    // in a list from FavoritePage.qml.
    FavoriteModel *favoriteModel = new FavoriteModel(QStringList() << "name", "favorites_connection", this);
    mQmlDocument->setContextProperty("_favoriteModel", favoriteModel);
}

void WeatherHistoryApp::createHomeModel()
{
    // The Home page is a special case for the WeatherModel and is set to be used
    // on the first tab in main.qml (see also WeatherItem.qml).
    WeatherModel *homeModel = new WeatherModel(this);
    mQmlDocument->setContextProperty("_homeModel", homeModel);

    // Connect to the homeModel cityChanged signal in order to update the application
    // settings for the home city (so it will be set on the next time the app launches)
    connect(homeModel, SIGNAL(regionChanged(QString)), this, SLOT(onUpdateHomeRegion(QString)));
    connect(homeModel, SIGNAL(cityChanged(QString)), this, SLOT(onUpdateHomeCity(QString)));
    connect(this, SIGNAL(serverUrlChanged(QUrl)), homeModel, SLOT(reset()));
    connect(this, SIGNAL(maxSizeChanged(uint)), homeModel, SLOT(reset()));

    // Begin loading weather data for the home page, if no hometown is stored in
    // the application settings
    QSettings settings;
    homeModel->setRegion(settings.value(HOME_REGION_KEY).toString());
    homeModel->setCity(settings.value(HOME_CITY_KEY).toString());
}

void WeatherHistoryApp::onUpdateHomeCity(QString city)
{
    QSettings().setValue(HOME_CITY_KEY, QVariant(city));
}

void WeatherHistoryApp::onUpdateHomeRegion(QString region)
{
    QSettings().setValue(HOME_REGION_KEY, QVariant(region));
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

uint WeatherHistoryApp::loadSize()
{
    return QSettings().value(LOAD_SIZE_SETTINGS_KEY, false).toUInt();
}

void WeatherHistoryApp::setLoadSize(uint size)
{
    uint oldLoadSize = loadSize();
    QSettings().setValue(LOAD_SIZE_SETTINGS_KEY, size);

    if (oldLoadSize != size) {
        emit loadSizeChanged(size);
    }
}

uint WeatherHistoryApp::maxSize()
{
    return QSettings().value(MAX_SIZE_SETTINGS_KEY, false).toUInt();
}

void WeatherHistoryApp::setMaxSize(uint size)
{
    uint oldMaxSize = maxSize();
    QSettings().setValue(MAX_SIZE_SETTINGS_KEY, size);

    if (oldMaxSize != size) {
        // Emitting this signal will reset all models
        emit maxSizeChanged(size);
    }
}

uint WeatherHistoryApp::hackThreshold()
{
	return QSettings().value(HACK_THRESHOLD_SETTINGS_KEY, false).toUInt();
}

void WeatherHistoryApp::setHackThreshold(uint threshold)
{
	QSettings().setValue(HACK_THRESHOLD_SETTINGS_KEY, threshold);

	emit hackThresholdChanged(threshold);
}

QUrl WeatherHistoryApp::prepareServerUrl(const QString& resource, bool includeLoadSize)
{
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

	if (includeLoadSize) {
		retval.addQueryItem("size", QString("%1").arg(settings.value(LOAD_SIZE_SETTINGS_KEY, mDefaultLoadSize).toUInt()));
	}

	return retval;
}
