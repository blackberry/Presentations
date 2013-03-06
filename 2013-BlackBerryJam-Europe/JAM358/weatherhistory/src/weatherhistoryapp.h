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
#ifndef WEATHERHISTORYAPP_H
#define WEATHERHISTORYAPP_H

#include <bb/cascades/Application>
#include <QUrl>

using namespace bb::cascades;

namespace bb
{
    namespace cascades
    {
        class QmlDocument;
    }
}

/**
 * WeatherHistoryApp Description:
 *
 * Welcome to the history of weather. This application gives you the possibility
 * to view the weather in the past. As in all dealings with weather the accuracy
 * should always be subject for scrutiny. Remember the best way to get an accurate
 * weather data point is to look out a window and short of a window, call a friend.
 * The purpose of the sample app is to show how to handle large data sets that
 * are presented in a list and provided over a network. You will need to have a
 * weather server set up and running in order for the sample to work.
 *
 * You will learn:
 * - How to present data retrieved over a network in a list
 * - How to implement functionality for asking for more data when reaching the end of a list
 * - How to handler errors that can occur when talking to a server
 */
class WeatherHistoryApp: public QObject
{
    Q_OBJECT

    /**
	 * The weather server url to grab data from.
	 */
    Q_PROPERTY(QUrl serverUrl READ serverUrl WRITE setServerUrl NOTIFY serverUrlChanged FINAL);

    /**
	 * Simulate server problems will tell the server to insert all sorts of errors while providing a response.
	 */
    Q_PROPERTY(bool simulateProblems READ simulateProblems WRITE setSimulateProblems NOTIFY simulateProblemsChanged FINAL);


public:
    // This is our constructor that sets up the recipe.
    WeatherHistoryApp();
    ~WeatherHistoryApp();

    /*
     * Accessor functions for properties.
     */
	QUrl serverUrl();
	void setServerUrl(QUrl url);

	bool simulateProblems();
	void setSimulateProblems(bool simulate);

signals:

	/*
	 * Signals for properties
	 */
	void serverUrlChanged(QUrl url);
	void simulateProblemsChanged(bool simulate);

private:

    /**
     * This function creates the model that is used to present a list of cities for different continents.
     */
    void createCitiesModel();

    /**
     * This function initializes the favorite model which is used for showing the cities marked as
     * favorites in the city database.
     */
    void createFavoritesModel();

    // The main qml document for the application.
    QmlDocument *mQmlDocument;

    /**
     * Default values for properties
     */
    static const QUrl mDefaultServerUrl;
	static const bool mDefaultSimulateProblems;

public:
	/**
	 * The keys where the properties are stored in the QSettings object.
	 */
    static const QString SERVER_URL_SETTINGS_KEY;
    static const QString SIMULATE_PROBLEMS_SETTINGS_KEY;

    /**
     * Utility function to prepare the server url, it sets up a complete
     * url ready to use for making a server request. Including adding
     * flags for things like simulating server issues and chunk sizes.
     *
     * @param resource A string with what resource that should be loaded
     * @param includeLoadSize Boolean controlling if a specific chunk of data entries should be used or not
     */
    static QUrl prepareServerUrl(const QString& resource);
};

#endif // ifndef WEATHERHISTORYAPP_H
