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

BarcodeScannerApp::BarcodeScannerApp(bb::cascades::Application *app) {

	// We need to register the QML types in the multimedia-library,
	// otherwise we will get an error from the QML.
	qmlRegisterType<BarcodeDecoderControl>("bb.community.barcode", 1, 0, "BarcodeDecoder");

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
	QObject::connect(barcodeDecoder, SIGNAL(newBarcodeDetected(QString)), this, SLOT(onNewBarcodeDetected(QString)));

	// Initialize app-specific UI stuff
	// This is important for this app, but not if you just want to know how to scan barcodes
	this->initalizeSounds();

	//Start the viewfinder, and the rest of the app
	//barcodeDecoder->startScanning();

}

void BarcodeScannerApp::onStartScan() {

}

void BarcodeScannerApp::onNewBarcodeDetected(QString barcode) {
	qDebug() << "+++++++++ Barcode detected: " << barcode << endl;
	player->setSourceUrl(QDir::currentPath() + "/app/native/assets/" + "sounds/boopdoop.mp3");
	player->play();
}

void BarcodeScannerApp::initalizeSounds() {
	player = new MediaPlayer(this);

}
