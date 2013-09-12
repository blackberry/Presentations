/*
* Copyright 2013 BlackBerry Limited.
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

#include "applicationui.hpp"

#include <bb/cascades/Application>
#include <bb/cascades/QmlDocument>
#include <bb/cascades/AbstractPane>

#include <bb/utility/i18n/DateFormat>

#include <QTimer>
#include <QDateTime>



using namespace bb::cascades;

ApplicationUI::ApplicationUI(bb::cascades::Application *app)
: QObject(app)
{

    QObject::connect(&localeHandler, SIGNAL(systemLanguageChanged()),
                     this, SLOT(onSystemLanguageChanged()));
    QObject::connect(&localeHandler, SIGNAL(languageChanged()),
                     this, SLOT(onLanguageChanged()));

    qmlRegisterType<QTimer>("my.library", 1, 0, "QTimer");

    onSystemLanguageChanged();
    // create scene document from main.qml asset
    // set parent to created document to ensure it exists for the whole application lifetime
    QmlDocument *qml = QmlDocument::create("asset:///main.qml").parent(this);

    qml->setContextProperty("ApplicationUI", this);

    // create root object for the UI
    AbstractPane *root = qml->createRootObject<AbstractPane>();
    // set created root object as a scene
    app->setScene(root);
}

// Called when the system language is changed
void ApplicationUI::onSystemLanguageChanged() {
    QString locale_string = QLocale().name();
    QString filename = QString( "HelloTrans_%1" ).arg( locale_string );
    if (translator.load(filename, "app/native/qm")) {
    	QCoreApplication::instance()->installTranslator(&translator);
    }

}

// Called when new translators are installed
void ApplicationUI::onLanguageChanged() {
  qDebug() << "onLanguageChanged() called";
}

QString ApplicationUI::dateAndTime() {
    return QDateTime::currentDateTime().toString(Qt::DefaultLocaleLongDate);
}
