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
#ifndef APPSETTINGS_H_
#define APPSETTINGS_H_

#include <QObject>
#include <QVariantMap>
#include <QUrl>
#include <bb/data/SqlConnection>

/**
 * AppSettings Description
 *
 * This class handles application wide settings that persist between runs.
 */
class AppSettings: public QObject
{
Q_OBJECT

    /**
     * The currently selected home location. A QVariantMap with "city" and "region" as keywords.
     */
    Q_PROPERTY(QVariantMap home READ home WRITE setHome NOTIFY homeChanged FINAL)

    /**
     * The weather server url to grab data from.
     */
    Q_PROPERTY(QUrl serverUrl READ serverUrl WRITE setServerUrl NOTIFY serverUrlChanged FINAL)

    /**
     * Simulate server problems will tell the server to insert all sorts of errors while providing a response.
     */
    Q_PROPERTY(bool simulateProblems READ simulateProblems WRITE setSimulateProblems NOTIFY simulateProblemsChanged FINAL)

    /**
     * Number of items to request when loading items for lists.
     */
    Q_PROPERTY(uint loadSize READ loadSize WRITE setLoadSize NOTIFY loadSizeChanged FINAL)

    /**
     * Max number of items to for lists.
     */
    Q_PROPERTY(uint maxSize READ maxSize WRITE setMaxSize NOTIFY maxSizeChanged FINAL)

public:
    AppSettings(QObject *parent = 0);
    virtual ~AppSettings();

    /*
     * Accessor functions for properties.
     */
    QVariantMap home();
    Q_SLOT void setHome(QVariantMap home);

    QUrl serverUrl();
    void setServerUrl(QUrl url);

    bool simulateProblems();
    void setSimulateProblems(bool simulate);

    uint maxSize();
    void setMaxSize(uint size);

    /**
     * The load size function is static in order to get access to the currently stored
     * load size when setting up a request
     */
    static uint loadSize();
    void setLoadSize(uint size);

signals:

    /*
     * Signals for properties
     */
    void serverUrlChanged(QUrl url);
    void simulateProblemsChanged(bool simulate);
    void loadSizeChanged(uint size);
    void maxSizeChanged(uint size);
    void hackThresholdChanged(uint threshold);
    void homeCityChanged(QString city);
    void homeRegionChanged(QString region);
    void homeChanged(QVariantMap home);

private:
    /**
     * Default values for properties
     */
    static const QUrl mDefaultServerUrl;
    static const bool mDefaultSimulateProblems;
    static const uint mDefaultLoadSize;
    static const uint mDefaultMaxSize;
    static const uint mDefaultHackThreshold;

    /**
     * Default values for main page data
     */
    static const QString mDefaultHomeRegion;
    static const QString mDefaultHomeCity;

    /**
     * The memberVariable for the home location.
     */
    QVariantMap mHomeMap;

    /**
     * Data base connector
     */
    bb::data::SqlConnection *mSqlConnector;

public:
    /**
     * The keys where the properties are stored in the QSettings object.
     */
    static const QString SERVER_URL_SETTINGS_KEY;
    static const QString SIMULATE_PROBLEMS_SETTINGS_KEY;
    static const QString LOAD_SIZE_SETTINGS_KEY;
    static const QString MAX_SIZE_SETTINGS_KEY;
    static const QString HACK_THRESHOLD_SETTINGS_KEY;

    /**
     * The keys where the main page city values are stored
     */
    static const QString HOME_REGION_KEY;
    static const QString HOME_CITY_KEY;

    /**
     * Utility function to prepare the server url, it sets up a complete
     * url ready to use for making a server request. Including adding
     * flags for things like simulating server issues and chunk sizes.
     *
     * @param resource A string with what resource that should be loaded
     * @param includeLoadSize Boolean controlling if a specific chunk of data entries should be used or not
     * @param customLoadSize To gain full control of the number of items this can be used in combination with includeLoadSize
     */
    static QUrl prepareServerUrl(const QString& resource, bool includeLoadSize = false, int customLoadSize = 0);

};

#endif /* APPSETTINGS_H_ */
