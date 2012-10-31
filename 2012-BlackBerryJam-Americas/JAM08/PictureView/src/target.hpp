/* Copyright (c) 2012 Research In Motion Limited.
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

#ifndef TARGET_HPP
#define TARGET_HPP

#include <bb/cascades/CustomControl>
#include <bb/cascades/QListDataModel>

class Opt;
namespace bb {
namespace system {
class InvokeManager;
class InvokeReply;
class InvokeQueryTargetsReply;
}
}

class Target : public bb::cascades::CustomControl
{
    Q_OBJECT

    Q_PROPERTY(QString target READ target WRITE setTarget NOTIFY targetChanged)
    Q_PROPERTY(QString mime READ mime WRITE setMime NOTIFY mimeChanged)
    Q_PROPERTY(QString action READ action WRITE setAction NOTIFY actionChanged)
    Q_PROPERTY(QString uri READ uri WRITE setUri NOTIFY uriChanged)
    Q_PROPERTY(QString data READ data WRITE setData NOTIFY dataChanged)
    Q_PROPERTY(QString description READ description WRITE setDescription NOTIFY descriptionChanged)
    Q_PROPERTY(QString contentDescription READ contentDescription WRITE setContentDescription NOTIFY contentDescriptionChanged)

public:
    Target();
    Target(const QString &name, const QString &target, bb::system::InvokeManager *imanager, const QString &defaultAction = "bb.action.OPEN", const int &targetType = 0);
    virtual ~Target();

    QString name() const;
    QString target() const;
    QString action() const;
    QString mime() const;
    QString data() const;
    QString uri() const;
    QString invokeReply() const;
    QString description() const;
    QString contentDescription() const;

    QUrl icon() const;
    int targetType() const;


    bool operator==(const Target &other) const;

public Q_SLOTS:
    void setTarget(const QString &target);
    void setDescription(const QString &description);
    void setData(const QString &data);
    void setUri(const QString &uri);
    void setMime(const QString &mime);
    void setAction(const QString &action);
    void setContentDescription(const QString &contentDescription);

    void invoke();
    void processInvokeReply();


Q_SIGNALS:
    void descriptionChanged();
    void contentDescriptionChanged();
    void uriChanged();
    void dataChanged();
    void actionChanged();
    void replyReceived(const QString &reply);
    void targetChanged();
    void mimeChanged();


protected:
    QString m_name;
    QString m_target;
    QString m_mime;
    QString m_uri;
    QString m_ireply;
    QString m_data;
    QString m_action;
    QString m_description;
    QString m_contentDescription;

    int m_targetType;
    bb::system::InvokeReply *m_invokeReply;
    bb::system::InvokeManager *m_imanager;
    bb::system::InvokeQueryTargetsReply *m_queryReply;
};

#endif
