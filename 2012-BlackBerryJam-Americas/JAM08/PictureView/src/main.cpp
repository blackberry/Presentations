#include "CardInvoker.h"
//#include "Viewer.hpp"


#include <bb/cascades/AbstractPane>
#include <bb/cascades/Application>
#include <bb/cascades/DropDown>
#include <bb/cascades/Page>
#include <bb/cascades/QmlDocument>
#include <camerahandler.h>

#include <QLocale>
#include <QTranslator>

#include <iostream>

using namespace bb::cascades;

int main(int argc, char **argv)
{

    qmlRegisterType<Target>("com.sample.invoke", 1, 0, "Target");


    //-- this is where the server is started etc
    Application app(argc, argv);

    //initialize camera
    CameraHandler cameraHandler;

    //initialize card invocation object
    CardInvoker cardinvoker;



    QmlDocument *qml = QmlDocument::create("asset:///main.qml");
    if (!qml->hasErrors()) {
        // Make the Sender and Application object available to the UI as context properties
        QDeclarativePropertyMap* targetMap = new QDeclarativePropertyMap;

        qml->setContextProperty("targetMap", targetMap);
        qml->setContextProperty("_cardinvoker", &cardinvoker);
        qml->setContextProperty("_camhdlr", &cameraHandler);
        AbstractPane *appPage = qml->createRootObject<AbstractPane>();

        if (appPage) {
            //initialize camera handler
            cameraHandler.setAbstractPane(appPage);
            cameraHandler.setupControls("myCamera", "toggleVf");

            cardinvoker.setAbstractPane(appPage);
            cardinvoker.setTargetMap(targetMap);
            cardinvoker.populateContentDropDown("cardContent");
            cardinvoker.setTarget("Cards", "Pictures");

            Application::instance()->setScene(appPage);


        }
    }

#if 0
    QTranslator translator;
    const QString locale_string = QLocale().name();
    const QString filename = QString("invocationdemo_%1").arg(locale_string);
    if (translator.load(filename, "app/native/qm")) {
        app.installTranslator(&translator);
    }
#endif

    return Application::exec();
}

