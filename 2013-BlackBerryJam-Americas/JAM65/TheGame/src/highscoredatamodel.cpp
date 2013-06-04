/*
 * Copyright (c) 2011, 2012 Research In Motion Limited.
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
#include "highscoredatamodel.h"

#include <bb/cascades/GroupDataModel>
#include <bb/data/DataAccessReply>
#include <bb/data/SqlConnection>

using namespace bb::cascades;
using namespace bb::data;

static const int SQL_LOAD = 1;

HighscoreDataModel::HighscoreDataModel()
{
    m_pModel = new GroupDataModel(QStringList() << "level" << "time" << "moves", this);
    m_pModel->setGrouping(ItemGrouping::ByFullValue);

    m_pSqlConnection = new SqlConnection("app/native/assets/highscoredb/highscore.db", this);

    connect(m_pSqlConnection, SIGNAL(reply(const bb::data::DataAccessReply&)), this,
            SLOT(onSQLResponse(const bb::data::DataAccessReply&)));

    m_pSqlConnection->execute("SELECT * from highscore order by time asc", SQL_LOAD);
}

HighscoreDataModel::~HighscoreDataModel()
{
}

void HighscoreDataModel::addItemToModel(const QString& name, const QVariant& time,
        const QVariant& moves, int level, int requestId)
{
    QVariantMap newItem;
    newItem["id"] = requestId;
    newItem["name"] = name;
    newItem["time"] = time.toLongLong();
    newItem["moves"] = moves.toLongLong();
    newItem["level"] = QVariant(level).toLongLong();
    m_pModel->insert(newItem);
    mLastSavedMap = newItem;
}

void HighscoreDataModel::addItem(const QString& name, const QVariant& time, const QVariant& moves,
        int level)
{
    // TODO:: using model size as request id could cause errors if a delete+insert is done quickly
    int requestId = -m_pModel->size();

    addItemToModel(name, time, moves, level, requestId);

    QString sqlQuery = "INSERT into highscore (name,time,moves,level) values ('" + name + "',"
            + time.toString() + "," + moves.toString() + "," + QString::number(level) + ")";
    m_pSqlConnection->beginTransaction();
    m_pSqlConnection->execute(sqlQuery);
    m_pSqlConnection->execute("SELECT Max(id) FROM highscore", requestId);
    m_pSqlConnection->endTransaction();
}

void HighscoreDataModel::updateLastSavedItem(const QString& name)
{
    if (mLastSavedMap.isEmpty())
        return;

    QVariantList indexPath = m_pModel->findExact(mLastSavedMap);
    mLastSavedMap["name"] = name;
    m_pModel->updateItem(indexPath, mLastSavedMap);

    QString sqlQuery = "UPDATE highscore set name = '" + name + "' where id = "
            + QString::number(mLastSavedMap["id"].toInt());
    m_pSqlConnection->execute(sqlQuery);
}

void HighscoreDataModel::onDataSaved(int requestId, int addedId)
{
    if (mLastSavedMap["id"] == requestId) {
        QVariantList indexPath = m_pModel->findExact(mLastSavedMap);
        mLastSavedMap["id"] = addedId;
        m_pModel->updateItem(indexPath, mLastSavedMap);
    } else {
        foreach(QVariantMap map,m_pModel->toListOfMaps()){
        if (map["id"] == requestId) {
            QVariantList indexPath = m_pModel->findExact(map);
            map["id"] = addedId;
            m_pModel->updateItem(indexPath, map);
            break;
        }
    }
}
}

void HighscoreDataModel::onDataLoaded(const QVariantList& list)
{
    m_pModel->insertList(list);
}

void HighscoreDataModel::onSQLResponse(const DataAccessReply& reply)
{
    if (reply.hasError()) {
        qDebug() << "SQL error: " << reply.id() << " " << reply.errorMessage();
    }
    if (reply.id() == SQL_LOAD) {
        onDataLoaded(reply.result().value<QVariantList>());
    } else if (reply.id() < 0) {
        QVariantList resultList = reply.result().value<QVariantList>();
        QVariantMap resultMap = resultList[0].value<QVariantMap>();
        int addedId = resultMap["Max(id)"].toInt();
        onDataSaved(reply.id(), addedId);
    }
}

DataModel* HighscoreDataModel::model()
{
    return m_pModel;
}

