// Default empty project template
#include "CascadesStobe.hpp"

#include <bb/cascades/Application>
#include <bb/cascades/QmlDocument>
#include <bb/cascades/AbstractPane>
#include <Qt/qtimer.h>

using namespace bb::cascades;

CascadesStobe::CascadesStobe(bb::cascades::Application *app)
: QObject(app)
{

	//This is the only line of C code we need, just to expose the QTimer to QML
	qmlRegisterType<QTimer>("qt", 1, 0, "QTimer");


    // create scene document from main.qml asset
    // set parent to created document to ensure it exists for the whole application lifetime
    QmlDocument *qml = QmlDocument::create("asset:///main.qml").parent(this);

    // create root object for the UI
    AbstractPane *root = qml->createRootObject<AbstractPane>();
    // set created root object as a scene
    app->setScene(root);
}

