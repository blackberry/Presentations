// Default empty project template
#include "BarcodeInvoker.hpp"

#include <bb/cascades/Application>
#include <bb/cascades/QmlDocument>
#include <bb/cascades/AbstractPane>
#include <bb/system/CardDoneMessage.hpp>

#include <bb/cascades/controls/button.h>

#include <qdebug.h>;

using namespace bb::cascades;
using namespace bb::system;

BarcodeInvoker::BarcodeInvoker(bb::cascades::Application *app) :
		QObject(app) {
	qDebug() << "+++++++ Barcode Invoker Started" << endl;
	_invokeManager = new InvokeManager(this);
	QObject::connect(_invokeManager,
			SIGNAL(childCardDone(const bb::system::CardDoneMessage&)), this,
			SLOT(onChildCardDone(const bb::system::CardDoneMessage&)));

	// create scene document from main.qml asset
	// set parent to created document to ensure it exists for the whole application lifetime
	QmlDocument *qml = QmlDocument::create("asset:///main.qml").parent(this);

	// create root object for the UI
	AbstractPane *root = qml->createRootObject<AbstractPane>();
	// set created root object as a scene
	app->setScene(root);

	_resultsLabel = root->findChild<Label*>("resultsLabel");

	Button* scanButton = root->findChild<Button*>("scanButton");
	QObject::connect(scanButton,
			SIGNAL(clicked()), this,
			SLOT(onInvokeButtonClicked()));

}

void BarcodeInvoker::onInvokeButtonClicked() {
	qDebug() << "+++++++ Invoke button clicked" << endl;
	InvokeRequest invokeRequest;
	invokeRequest.setTarget("com.example.BarcodeScanner");
	invokeRequest.setAction("community.action.SCANBARCODE");
	InvokeTargetReply *invokeReply = _invokeManager->invoke(invokeRequest);
}

void BarcodeInvoker::onChildCardDone(const bb::system::CardDoneMessage
		&message){
	qDebug() << "+++++++ Child card done: " << message.data() << endl;
	_resultsLabel->setText("Results: " + message.data());
}
