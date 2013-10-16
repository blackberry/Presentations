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

#include "animdatasource.h"
#include "globalconnection.h"

#include <bb/data/DataAccessReply>
#include <bb/data/JsonDataAccess>
#include <bb/data/SqlConnection>

#include <QTimer>

using namespace bb::data;

AnimDataSource::AnimDataSource(QObject *parent) :
		QObject(parent), mRevision(0) {
	// Get the global sql connection.
	mSqlConnector = GlobalConnection::instance()->sqlConnection();

	connect(mSqlConnector, SIGNAL(reply(const bb::data::DataAccessReply&)),
			this, SLOT(onSqlConnectorReply(const bb::data::DataAccessReply&)));
}

AnimDataSource::~AnimDataSource() {
}

int AnimDataSource::incrementRevision() {
	QVariantList dataList;
	dataList << "animinfo";
	mSqlConnector->executeAndWait(
			"UPDATE revision SET revision_id = revision_id + 1 WHERE table_name=:animinfo",
			dataList, 1);

	DataAccessReply reply = mSqlConnector->executeAndWait(
			"SELECT revision_id FROM revision WHERE table_name=:animinfo",
			dataList, 2);

	if (!reply.hasError()) {
		QVariantList resultList = reply.result().toList();
		QVariantMap resultMap = resultList.at(0).toMap();
		mRevision = resultMap["revision_id"].toInt();
	} else {
		qDebug() << "Revision could not be updated.";
	}

	return mRevision;
}

void AnimDataSource::onSqlConnectorReply(
		const bb::data::DataAccessReply& reply) {
	if (reply.hasError()) {
		qWarning() << "Database error, " << reply.errorType() << " "
				<< reply.errorMessage() << " id: " << reply.id();
	} else {
		if (reply.id() == 0) {
			QTimer::singleShot(800, this, SLOT(fakeNetworkDelay()));
		}
	}
}

void AnimDataSource::addInfo(const QString title, const QString band, int rating) {
	// Update the database.
	if (!title.isEmpty() && !title.isNull()) {
		int revision = incrementRevision();

		QVariantList dataList;
		dataList << title << band << rating << revision << true;

		QString query =
				"INSERT INTO animinfo (title, band, rating, revision_id, newItem) VALUES (:title, :band, :rating, :revision_id, :newItem)";
		mSqlConnector->execute(query, dataList);
	}
}

void AnimDataSource::fakeNetworkDelay() {
	emit dataChanged(mRevision);
}

void AnimDataSource::markAsOld(int id) {
	int revision = incrementRevision();

	QVariantList dataList;
	dataList << revision << false << id;
	qDebug() << "datalist " << dataList;
	QString query = "UPDATE animinfo SET revision_id=:revision,newItem=:newItem WHERE infoid=:infoid";
	mSqlConnector->executeAndWait(query, dataList);
}
