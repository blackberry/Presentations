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
#ifndef _ANIMDATASOURCE_H_
#define _ANIMDATASOURCE_H_

#include <QObject>
#include <bb/data/SqlConnection>


namespace bb
{
    namespace data
    {
        class SqlConnection;
    }
}

/* AnimDataSource Description:
 *
 * A helper class for this demo, that will add new data to the data base
 * used for presenting the list.
 */
class AnimDataSource: public QObject
{
    Q_OBJECT

public:
    /**
     * The constructor of the data source object.
     *
     * @param parent The data model owner or @c 0. Optional parameter; defaults to @c 0, if not specified.
     */
    AnimDataSource(QObject *parent = 0);
    ~AnimDataSource();

    /**
     * Call this function to add an item to the data base.
     *
     * @param title - The title of the new album.
     * @param band - The band name of the new item.
     * @param rating - The rating 1-5 of the new item.
     */
    Q_INVOKABLE void addInfo(const QString title, const QString band, int rating);

    /**
     * Call this function to mark the item as old. A more consistent way of handling
     * this would be to create a data model decorator class that would handle the new state.
     */
    Q_INVOKABLE void markAsOld(int id);

    /**
     * Returns the current global revision of the database plus one.
     *
     * @return An integer with next revision.
     */
    Q_INVOKABLE int incrementRevision();

signals:

    /**
     * Signal emitted when new data have been added to the database.
     */
    void dataChanged(int revision);


private slots:

    /**
     * Function used to connect to the SqlConnectors reply signal, this is
     * called when data base queries have been executed.
     *
     * @param DataAccessReply reply The Database reply
     */
    void onSqlConnectorReply(const bb::data::DataAccessReply& reply);

private:
    Q_SLOT void fakeNetworkDelay();

    // Data base connector.
    bb::data::SqlConnection *mSqlConnector;

    // The last accessed data revision.
    int mRevision;

};

#endif // ifndef _ANIMDATASOURCE_H_
