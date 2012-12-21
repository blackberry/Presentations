#include "BarcodeScannerApp.hpp"

#include <bb/cascades/Application>
#include <bb/cascades/QmlDocument>
#include <bb/cascades/AbstractPane>
#include <bb/cascades/multimedia/Camera>
#include <bb/cascades/multimedia/CameraSettings.hpp>
#include <bb/cascades/multimedia/CameraTypes.hpp>
#include <bb/cascades/Page>
#include <bb/cascades/controls/label.h>
#include <bb/cascades/controls/page.h>
#include <bb/system/CardDoneMessage.hpp>
#include <bb/system/ApplicationStartupMode>

//Audio system
#include <bb/multimedia/MediaPlayer.hpp>

#include <zxing/common/GreyscaleLuminanceSource.h>
#include <zxing/common/HybridBinarizer.h>
#include <zxing/MultiFormatReader.h>

#include <exception>

using namespace bb::community::barcode;
using namespace bb::cascades;
using namespace bb::cascades::multimedia;
using namespace zxing;
using namespace bb::multimedia;
using namespace bb::system;

BarcodeScannerApp::BarcodeScannerApp(bb::cascades::Application *app) {

	_invokeManager = new InvokeManager(this);

	connect(_invokeManager, SIGNAL(invoked(const bb::system::InvokeRequest&)),
			this, SLOT(onInvoked(const bb::system::InvokeRequest&)));

	connect(_invokeManager,
			SIGNAL(cardPooled(const bb::system::CardDoneMessage&)), this,
			SLOT(onCardPooled(const bb::system::CardDoneMessage&)));

	switch (_invokeManager->startupMode()) {
	case ApplicationStartupMode::LaunchApplication:
		_invoked = false;
		break;
	case ApplicationStartupMode::InvokeApplication:
	case ApplicationStartupMode::InvokeViewer:
	case ApplicationStartupMode::InvokeCard:

		_invoked = true;
		break;
	}

	qDebug() << "+++++++++ Application invoked: " << _invoked << endl;

	// We need to register the QML types in the multimedia-library,
	// otherwise we will get an error from the QML.
	qmlRegisterType<BarcodeDecoderControl>("bb.community.barcode", 1, 0,
			"BarcodeDecoder");

	QmlDocument *qml = QmlDocument::create("asset:///main.qml");
	//QmlDocument
	//-- setContextProperty expose C++ object in QML as an variable
	//-- uncomment next line to introdu ce 'this' object to QML name space as an 'app' variable
	//qml->setContextProperty("app", this);

	AbstractPane *root = qml->createRootObject<AbstractPane>();
	app->setScene(root);

	//Create our Barcode Decoder object
	barcodeDecoder = root->findChild<BarcodeDecoderControl*>("barcodeDecoder");

	// Begin listening for a new barcode
	QObject::connect(barcodeDecoder, SIGNAL(newBarcodeDetected(QString)), this,
			SLOT(onNewBarcodeDetected(QString)));

	// Initialize app-specific UI stuff
	// This is important for this app, but not if you just want to know how to scan barcodes
	this->initalizeSounds();

	//Start the viewfinder, and the rest of the app
	//barcodeDecoder->startScanning();

}

void BarcodeScannerApp::onNewBarcodeDetected(QString barcode) {
	qDebug() << "+++++++++ Barcode detected: " << barcode << endl;
	player->setSourceUrl(
			QDir::currentPath() + "/app/native/assets/"
					+ "sounds/boopdoop.mp3");
	player->play();

	if (_invoked) {
		qDebug() << "+++++++++ Sending done message" << endl;
		CardDoneMessage message;
		message.setData(barcode);
		message.setDataType("text/plain");
		message.setReason("Barcode scanned!");
		barcodeDecoder->stopScanning();
		_invokeManager->sendCardDone(message);
	}
}

void BarcodeScannerApp::initalizeSounds() {
	player = new MediaPlayer(this);

}
void BarcodeScannerApp::onCardPooled(const bb::system::CardDoneMessage&) {
	qDebug() << "+++++++++ Barcode card pooled" << endl;
	_pooled = true;
	barcodeDecoder->stopScanning();
}

void BarcodeScannerApp::onInvoked(const bb::system::InvokeRequest&) {
	if (_pooled) {
		barcodeDecoder->startScanning();
	}
}
