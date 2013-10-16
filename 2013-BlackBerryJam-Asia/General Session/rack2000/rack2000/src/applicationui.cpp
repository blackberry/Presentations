/* Copyright (c) 2013 BlackBerry.
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
#include "accelerationsensor.hpp"
#include "messaging/BTController.hpp"
#include "messaging/ArduinoBTController.hpp"

#include "connection/MacAddressHandler.hpp"

#include <bb/cascades/Application>
#include <bb/cascades/QmlDocument>
#include <bb/cascades/AbstractPane>
#include <bb/cascades/LocaleHandler>

#include <bb/cascades/Window>

using namespace bb::cascades;

ApplicationUI::ApplicationUI(bb::cascades::Application *app) :
        QObject(app), mMacAddressHandler(0), mArduinoBTController(0)
{
    qmlRegisterType<BTController>("com.rack2000", 1, 0, "BTController");
    qmlRegisterType<ChatManager>();
    qmlRegisterType<QTimer>("com.rack2000", 1, 0, "QTimer");
    qmlRegisterType<AccelerationSensor>("com.rack2000", 1, 0, "AccelerationSensor");

    // Prepare the localization
    mTranslator = new QTranslator(this);
    mLocaleHandler = new LocaleHandler(this);
    connect(mLocaleHandler, SIGNAL(systemLanguageChanged()), this, SLOT(onSystemLanguageChanged()));
    onSystemLanguageChanged();

    // Create scene document from main.qml asset, the parent is set
    // to ensure the document gets destroyed properly at shut down.
    mQml = QmlDocument::create("asset:///main.qml").parent(this);

    mQml->setContextProperty("_app", this);

    // Create root object for the UI
    AbstractPane *root = mQml->createRootObject<AbstractPane>();

    // Set created root object as the application scene
    app->setScene(root);

    Application::instance()->mainWindow()->setScreenIdleMode(ScreenIdleMode::KeepAwake);
}

ApplicationUI::~ApplicationUI()
{
}

void ApplicationUI::onSystemLanguageChanged()
{
    QCoreApplication::instance()->removeTranslator(mTranslator);
    // Initiate, load and install the application translation files.
    QString locale_string = QLocale().name();
    QString file_name = QString("rack2000_%1").arg(locale_string);
    if (mTranslator->load(file_name, "app/native/qm")) {
        QCoreApplication::instance()->installTranslator(mTranslator);
    }
}

void ApplicationUI::createMacAddressHandler()
{
    if (!mMacAddressHandler) {
        mMacAddressHandler = new MacAddressHandler(this);
        mQml->setContextProperty("_btAddressHandler", mMacAddressHandler);
    }
}

void ApplicationUI::createArduinoController()
{
    if (!mArduinoBTController) {
        mArduinoBTController = new ArduinoBTController(this);
        mQml->setContextProperty("_btArduino", mArduinoBTController);
    }
}
