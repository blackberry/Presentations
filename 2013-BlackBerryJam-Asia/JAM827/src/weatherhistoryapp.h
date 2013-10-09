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
#ifndef WEATHERHISTORYAPP_H
#define WEATHERHISTORYAPP_H

#include <bb/cascades/Application>
#include <bb/cascades/datamanager/SqlHeaderDataQuery>

#include <QUrl>
#include "appsettings.h"

using namespace bb::cascades;
using namespace bb::cascades::datamanager;

namespace bb
{
    namespace cascades
    {
        class QmlDocument;
        class LocaleHandler;
    }
}

class AppSettings;
class QTranslator;

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
public:
    // This is our constructor that sets up the recipe.
    WeatherHistoryApp();
    ~WeatherHistoryApp();

    Q_INVOKABLE QString getHomeDirectory();
private:
    /**
     * System Language function, which will be attached to the signal emitted when
     * the system language change (for example if it is changed in the settings menu);
     */
    Q_SLOT void onSystemLanguageChanged();

    // Variables used for handling Language set-up
    QTranslator* mTranslator;
    bb::cascades::LocaleHandler* mLocaleHandler;

    // The main qml document for the application.
    QmlDocument *mQmlDocument;

    // Settings object that persistently stores all applicaiton settings.
    AppSettings *mAppSettings;
};

#endif // ifndef WEATHERHISTORYAPP_H
