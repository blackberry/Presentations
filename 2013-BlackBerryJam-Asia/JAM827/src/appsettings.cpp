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
#include "appsettings.h"

#include <QCoreApplication>
#include <QSettings>
#include <QDebug>
#include <bb/data/DataAccessReply>

const QUrl AppSettings::mDefaultServerUrl("http://169.254.0.2:8080/WeatherService/");
const bool AppSettings::mDefaultSimulateProblems(false);
const uint AppSettings::mDefaultLoadSize(10);
const uint AppSettings::mDefaultMaxSize(1024);
const uint AppSettings::mDefaultHackThreshold(10);
const QString AppSettings::SERVER_URL_SETTINGS_KEY("serverUrl");
const QString AppSettings::SIMULATE_PROBLEMS_SETTINGS_KEY("simulateProblems");
const QString AppSettings::LOAD_SIZE_SETTINGS_KEY("loadSize");
const QString AppSettings::MAX_SIZE_SETTINGS_KEY("maxSize");
const QString AppSettings::HACK_THRESHOLD_SETTINGS_KEY("hackThreshold");

AppSettings::AppSettings(QObject* parent) : QObject(parent)

{
    // Set up the application organization and name (used by QSettings
    // when saving values to the persistent store).
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

    if (settings.value(LOAD_SIZE_SETTINGS_KEY).isNull()) {
        setLoadSize(mDefaultLoadSize);
    }

    if (settings.value(MAX_SIZE_SETTINGS_KEY).isNull()) {
        setMaxSize(mDefaultMaxSize);
    }
}

AppSettings::~AppSettings()
{
}

QUrl AppSettings::serverUrl()
{
    return QSettings().value(SERVER_URL_SETTINGS_KEY, QUrl("serverUrl not set!")).toUrl();
}

void AppSettings::setServerUrl(QUrl url)
{
    if (url != serverUrl()) {
        QSettings().setValue(SERVER_URL_SETTINGS_KEY, url);

        // Emitting this signal will reset all models
        emit serverUrlChanged(url);
    }
}

bool AppSettings::simulateProblems()
{
    return QSettings().value(SIMULATE_PROBLEMS_SETTINGS_KEY, false).toBool();
}

void AppSettings::setSimulateProblems(bool simulate)
{
    if (simulateProblems() != simulate) {
        QSettings().setValue(SIMULATE_PROBLEMS_SETTINGS_KEY, simulate);
        emit simulateProblemsChanged(simulate);
    }
}

uint AppSettings::loadSize()
{
    return QSettings().value(LOAD_SIZE_SETTINGS_KEY, false).toUInt();
}

void AppSettings::setLoadSize(uint size)
{
    if (loadSize() != size) {
        QSettings().setValue(LOAD_SIZE_SETTINGS_KEY, size);
        emit loadSizeChanged(size);
    }
}

uint AppSettings::maxSize()
{
    return QSettings().value(MAX_SIZE_SETTINGS_KEY, false).toUInt();
}

void AppSettings::setMaxSize(uint size)
{
    if (maxSize() != size) {
        QSettings().setValue(MAX_SIZE_SETTINGS_KEY, size);

        // Emitting this signal will reset all models
        emit maxSizeChanged(size);
    }
}

QUrl AppSettings::prepareServerUrl(const QString& resource, bool includeLoadSize, int customLoadSize)
{
    QSettings settings;
    QString url = QSettings().value(SERVER_URL_SETTINGS_KEY, QUrl("serverUrl not set!")).toUrl()
            .toString(QUrl::StripTrailingSlash);

    if (resource.at(0) != '/') {
        url.append('/');
    }
    url.append(resource);

    QUrl retval(url);
    bool simulateProblems = QSettings().value(SIMULATE_PROBLEMS_SETTINGS_KEY, false).toBool();

    if (simulateProblems) {
        retval.addQueryItem("problems", "yes%20please");
    }

    if (includeLoadSize) {
        int loadSize = QSettings().value(LOAD_SIZE_SETTINGS_KEY, mDefaultLoadSize).toUInt();

        if(customLoadSize > 0 && customLoadSize < loadSize) {
            // If a custom load size has been given and its smaller then the loadsize use that instead.
            loadSize = customLoadSize;
        }

        retval.addQueryItem("size", QString("%1").arg(loadSize));
    }

    return retval;
}
