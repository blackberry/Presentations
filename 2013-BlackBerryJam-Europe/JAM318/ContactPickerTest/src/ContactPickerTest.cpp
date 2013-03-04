// Default empty project template
#include "ContactPickerTest.hpp"

#include <bb/cascades/Application>
#include <bb/cascades/QmlDocument>
#include <bb/cascades/AbstractPane>
#include <bb/cascades/Pickers/ContactPicker.hpp>
#include <QtDeclarative/qdeclarative.h>
#include <qdebug.h>

using namespace bb::cascades;
using namespace bb::cascades::pickers;

ContactPickerTest::ContactPickerTest(bb::cascades::Application *app)
: QObject(app)
{
	qDebug() << "+++++++ Contact picker begin" << endl;

	//All we really need to do here is register the ContactPicker control
	qmlRegisterType<ContactPicker>("bb.cascades.pickers", 1, 0, "ContactPicker");


	// create scene document from main.qml asset
    // set parent to created document to ensure it exists for the whole application lifetime
    QmlDocument *qml = QmlDocument::create("asset:///main.qml").parent(this);

    // create root object for the UI
    AbstractPane *root = qml->createRootObject<AbstractPane>();
    // set created root object as a scene
    app->setScene(root);
}

