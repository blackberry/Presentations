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
#include "weathermodel.h"
#include "weatherhistoryapp.h"
#include "logging.h"

#include <bb/data/JsonDataAccess>
#include <QtNetwork/QNetworkAccessManager>

using namespace bb::cascades;

WeatherModel::WeatherModel(QObject *parent)
: mReply(0), mInitialLoad(false), mErrorCode(NoError)
{
	Q_UNUSED(parent);

    // Connect to the sslErrors signal to the onSslErrors() function. This will help us see what errors
    // we get when connecting to the address given by server url.
    connect(&mAccessManager, SIGNAL(sslErrors ( QNetworkReply *  , const QList<QSslError> & )),
            this, SLOT(onSslErrors ( QNetworkReply * , const QList<QSslError> & )));
}

QString WeatherModel::itemType(const QVariantList& indexPath)
{
    QString typeName("item");
    int currentSize = childCount(QVariantList());
    int itemIndexPath = indexPath[0].toInt();

    if (itemIndexPath == currentSize - 1) {
        // We only have to check if the item is a loadItem if it is the last entry
        if(value(itemIndexPath).toMap().contains("loadItem")) {
            // If the is contains the loadItem return loadItem type
            typeName = "loadItem";
        }
    }

    return typeName;
}

void WeatherModel::insertLoadItem()
{
    QVariantMap lastItem  = value(childCount(QVariantList()) - 1).toMap();

    // Insert a load item if there is no item already and not all data has been gathered
    if(!lastItem.contains("loadItem") && !mCursor.endOfData) {
        QVariantMap lastItem;
        lastItem["loadItem"] = QVariant(true);
        append(lastItem);
    }
}

void WeatherModel::removeLoadItem()
{
    QVariantMap lastItem  = value(childCount(QVariantList()) - 1).toMap();

    if(lastItem.contains("loadItem")) {
        removeAt(childCount(QVariantList()) - 1);
    }
}

void WeatherModel::httpFinished()
{
    JsonDataAccess jda;
    QVariantList weatherDataFromServer;
    int httpStatus = -1; // controls the final behavior of this function

    if (mReply->error() == QNetworkReply::NoError) {
        // Load the data using the reply QIODevice.
        weatherDataFromServer = jda.load(mReply).value<QVariantList>();

        if (jda.hasError()) {
			bb::data::DataAccessError error = jda.error();
	        WEATHERAPP_LOG(QString("JSON loading error: %1: %2").arg(error.errorType()).arg(error.errorMessage()));
	        httpStatus = -2;
		} else {
			httpStatus = 200;
		}
    } else {
    	// An error occurred, try to get the http status code and reason
    	QVariant statusCode = mReply->attribute( QNetworkRequest::HttpStatusCodeAttribute );
		QString reason = mReply->attribute( QNetworkRequest::HttpReasonPhraseAttribute ).toString();

		if (statusCode.isValid()) {
			httpStatus = statusCode.toInt();
		}

		WEATHERAPP_LOG(QString("Network request to %1 failed with http status %2 %3").arg(mReply->request().url().toString()).arg(httpStatus).arg(reason));
    }

	// Now behave
	switch (httpStatus) {
	case 200:
		loadNetworkReplyDataIntoModel(weatherDataFromServer);
		break;
	case 404:
		if (mCursor.index == 0) {
			// If we requested index 0 and didn't get an empty array it means the city does not exist and we should show an error
			mCursor.endOfData = true;
			setErrorCode(WeatherModel::InvalidCity);
		} else {
			// If we get a 404 in the middle of a data set it simply means there is no more data
			mCursor.endOfData = true;
		}
		break;
	case 503:
		// TODO: perhaps try again a few times and eventually just stop? if we end up stopping and the list is empty, show an alert message. if the list isn't empty just stop fetching
		setErrorCode(WeatherModel::ServerBusy);
		break;
	case -2:
		setErrorCode(WeatherModel::JsonError);
		mCursor.endOfData = true;
		break;
	case 500:
	default:
		// TODO: the server crapped out, if we don't have any entries let the user know an error occurred, otherwise just stop fetching
		mCursor.endOfData = true;
		setErrorCode(WeatherModel::ServerError);
		break;
	}

	// After this point the initial load will be false
	setInitialLoad(false);

    // The reply is not needed now so we call deleteLater() function since we are in a slot.
    mReply->deleteLater();
    mReply = 0;
}

void WeatherModel::loadNetworkReplyDataIntoModel(QVariantList weatherData)
{
	int numberOfItemsReceived = weatherData.size();
    removeLoadItem();

    WEATHERAPP_LOG(QString("Received %1 entries").arg(numberOfItemsReceived));

    // Iterate over all the items in the received data.
    QVariantList::Iterator item = weatherData.begin();
    while (item != weatherData.end()) {
        QVariantMap itemMap = (*item).toMap();
        QDate itemDate = QDate::fromString(itemMap["date"].toString(), "yyyy M d");

		itemMap["date"] = QVariant(itemDate);

		// Set the item to the current itemMap (with updated date property)
		(*item) = itemMap;

        ++item;
    }

    // Insert the data into this model.
    append(weatherData);

    // Increment our data index
    mCursor.index += numberOfItemsReceived;

    insertLoadItem();
}

void WeatherModel::onSslErrors(QNetworkReply * reply, const QList<QSslError> & errors)
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

void WeatherModel::requestMoreDataFromNetwork()
{
    // Only request data if there is currently no request being done.
    if(mReply == 0 && mCursor.endOfData == false) {
    	WEATHERAPP_LOG("mReply and mCursor.endOfData were false/0, requesting more items");

        QString encodedCity = QUrl(mCity).toEncoded();
        QString encodedRegion = QUrl(mRegion).toEncoded();
        QUrl path = WeatherHistoryApp::prepareServerUrl("resources/cities/" + encodedRegion + "/" +  encodedCity);
        path.addQueryItem("start", QString("%1").arg(mCursor.index));

        WEATHERAPP_LOG("GET " + path.toString());

        mReply = mAccessManager.get(QNetworkRequest(path));

        // Connect to the reply finished signal to httpFinsihed() Slot function.
        connect(mReply, SIGNAL(finished()), this, SLOT(httpFinished()));
    }
    else {
    	WEATHERAPP_LOG(QString("will not request more data because mReply was %1 or mCursor.endOfData was %2").arg((int)mReply).arg(mCursor.endOfData));
    }
}

void WeatherModel::setCity(QString city)
{
    if (mCity.compare(city) != 0 || mErrorCode != WeatherModel::NoError)
    {
    	// Reset the cursor
    	mCursor = DataCursor();

        // Remove all the old data.
        this->clear();
        mCity = city;
        emit cityChanged(city);

        // The data for the city is requested.
        requestMoreDataFromNetwork();
        setInitialLoad(true);
    }
}

QString WeatherModel::city()
{
    return mCity;
}


void WeatherModel::setRegion(QString region)
{
    if (mRegion.compare(region) != 0) {
        mRegion = region;
        emit regionChanged(region);
    }
}

QString WeatherModel::region()
{
    return mRegion;
}

void WeatherModel::reset()
{
    // Reset the cursor
    mCursor = DataCursor();

    // Remove all the old data.
    this->clear();

    if(!mCity.isEmpty() && !mRegion.isEmpty()) {
        // The data for the city is requested.
        requestMoreDataFromNetwork();
        setInitialLoad(true);
    }
}

bool WeatherModel::initialLoad()
{
    return mInitialLoad;
}

void WeatherModel::setInitialLoad(bool newStatus)
{
    if(newStatus != mInitialLoad) {
        if(newStatus){
            // Only set the error code to no error if when the initial is started.
            setErrorCode(WeatherModel::NoError);
        }
        mInitialLoad = newStatus;
        emit initialLoadChanged(mInitialLoad);
    }
}

WeatherModel::WeatherModelError WeatherModel::errorCode()
{
    return mErrorCode;
}

void WeatherModel::setErrorCode(WeatherModel::WeatherModelError error)
{
    mErrorCode = error;
    errorCodeChanged(mErrorCode);
}


