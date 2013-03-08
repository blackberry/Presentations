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

#include "citymodel.h"
#include "weatherhistoryapp.h"
#include "logging.h"
#include <bb/data/JsonDataAccess>
#include <QtNetwork/QNetworkAccessManager>

using namespace bb::cascades;


CityModel::CityModel(const QStringList &keys, QObject *parent) :
        GroupDataModel(keys, parent), mReply(0), mInitialLoad(false)
{
    // Connect to the sslErrors signal to the onSslErrors() function. This will help us see what errors
    // we get when connecting to the address given by mServerUrl.
    connect(&mAccessManager, SIGNAL(sslErrors ( QNetworkReply *  , const QList<QSslError> & )),
            this, SLOT(onSslErrors ( QNetworkReply * , const QList<QSslError> & )));

}

CityModel::~CityModel()
{
}


void CityModel::httpFinished()
{
    JsonDataAccess jda;
    QVariantList citiesData;

    if (mReply->error() == QNetworkReply::NoError) {
        // Load the data using the reply QIODevice.

        citiesData = jda.load(mReply).value<QVariantList>();
    } else {
        // If there was an error loading the data via the network, we will log it and
        // display an error message.
        QVariant statusCode = mReply->attribute( QNetworkRequest::HttpStatusCodeAttribute );

        if (statusCode.isValid()) {
            int status = statusCode.toInt();
            QString reason = mReply->attribute( QNetworkRequest::HttpReasonPhraseAttribute ).toString();

    		WEATHERAPP_LOG(QString("Network request to %1 failed with http status %2 %3").arg(mReply->request().url().toString()).arg(status).arg(reason));
		}
		else {
			WEATHERAPP_LOG("Network request to " + mReply->request().url().toString() + " failed, make sure you are connected to the same network as the server you are trying to reach");
		}

		// TODO: emit a signal that alerts watchers of an error
    }

    if (jda.hasError()) {
        bb::data::DataAccessError error = jda.error();
        WEATHERAPP_LOG(QString("JSON loading error: %1: %2").arg(error.errorType()).arg(error.errorMessage()));
        return;
    }

    this->insertList(citiesData);

    // The initial load will be false once we are ready with the first batch
    setInitialLoad(false);

    // The reply is not needed now so we call deleteLater() function since we are in a slot.
    mReply->deleteLater();
    mReply = 0;
}

void CityModel::onSslErrors(QNetworkReply * reply, const QList<QSslError> & errors)
{
    // Ignore all SSL errors to be able to load from JSON file from the secure address.
    // It might be a good idea to display an error message indicating that security may be compromised.
    //
    // The errors we get are:
    // "SSL error: The issuer certificate of a locally looked up certificate could not be found"
    // "SSL error: The root CA certificate is not trusted for this purpose"
    // Seems to be a problem with how the server is set up and a known QT issue QTBUG-23625

    reply->ignoreSslErrors(errors);
}

void CityModel::changeContinent(QString continent)
{
    QString query;

    // Remove previous items from the model, all items will be replaced by
    // cities from the continent given as a parameter.
    this->clear();

    QUrl path;

    if (continent.contains("All")) {
        // We have one special case where we load "All Cities", if this
        // is selected we do not load all cities at once but instead load
        // an initial batch, then we continue to load the rest in another thread.
        path = WeatherHistoryApp::prepareServerUrl("resources/cities");
    } else {
        // When loading a specific continent, we load the entire list synchronously.
    	path = WeatherHistoryApp::prepareServerUrl("resources/regions/" + continent);
    }

    mReply = mAccessManager.get(QNetworkRequest(path));

    // Connect to the reply finished signal to httpFinsihed() Slot function.
    connect(mReply, SIGNAL(finished()), this, SLOT(httpFinished()));
    setInitialLoad(true);
}

void CityModel::setContinent(QString continent)
{
    if (mContinent.compare(continent) != 0) {
        mContinent = continent;
        emit continentChanged();

        // Update the data to match the given continent
        changeContinent(continent);
    }
}

QString CityModel::continent()
{
    return mContinent;
}

void CityModel::reset()
{
    if(!mContinent.isNull()) {
        // Update the data to match the given continent
        changeContinent(mContinent);
    }
}

void CityModel::setInitialLoad(bool newStatus)
{
    if(newStatus != mInitialLoad) {
        mInitialLoad = newStatus;
        emit initialLoadChanged(mInitialLoad);
    }
}

bool CityModel::initialLoad()
{
    return mInitialLoad;
}
