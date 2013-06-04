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
#ifndef HIGHSCOREDATAMODEL_H_
#define HIGHSCOREDATAMODEL_H_

#include <QtDeclarative>

/**
 * Forward declarations
 */
namespace bb
{
    namespace cascades
    {
        class DataModel;
        class GroupDataModel;
    }

    namespace data
    {
        class SqlConnection;
        class DataAccessReply;
    }
}

class HighscoreDataModel: public QObject
{
    Q_OBJECT
    Q_PROPERTY(bb::cascades::DataModel* model READ model NOTIFY modelChanged)

public:
    HighscoreDataModel();
    virtual ~HighscoreDataModel();

    void onDataSaved(int requestId, int addedId);
    void onDataLoaded(const QVariantList& list);

    // Slot for handling responses to sql querys
    Q_SLOT
    void onSQLResponse(const bb::data::DataAccessReply& reply);

    Q_INVOKABLE
    void addItem(const QString& name, const QVariant& time, const QVariant& moves, int level);
    void updateLastSavedItem(const QString& name);

    bb::cascades::DataModel* model();Q_SIGNAL
    void modelChanged(bb::cascades::DataModel*);

private:
    void addItemToModel(const QString& name, const QVariant& time, const QVariant& moves, int level,
            int requestId);

    bb::data::SqlConnection* m_pSqlConnection;

    QVariantMap mLastSavedMap;

    bb::cascades::GroupDataModel* m_pModel;
};

#endif /* HIGHSCOREDATAMODEL_H_ */
