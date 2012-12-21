#ifndef BARCODE_SCANNER_APP_H
#define BARCODE_SCANNER_APP_H

#include <QObject>
#include <QSharedPointer>
#include <bb/cascades/multimedia/Camera>
#include <bb/cascades/Button>
#include <bb/cascades/controls/label.h>
#include <bb/cascades/NavigationPane>
#include <bb/cascades/NavigationPaneProperties>
#include <zxing/MultiFormatReader.h>
#include "BarcodeDecoder.hpp"

#include <bb/system/InvokeManager.hpp>
#include <bb/system/InvokeRequest.hpp>


//Audio system
#include <bb/multimedia/MediaPlayer.hpp>

/*!
 * @brief Application GUI object
 */
namespace bb {
namespace cascades {
class Application;

using namespace bb::community::barcode;
using namespace bb::cascades;
using namespace bb::cascades::multimedia;
using namespace zxing;
using namespace bb::multimedia;
using namespace bb::system;

class BarcodeScannerApp: public QObject {
	Q_OBJECT


public:
	BarcodeScannerApp(bb::cascades::Application *app);
	virtual ~BarcodeScannerApp() {
	}

public slots:
	void onNewBarcodeDetected(QString barcode);
	//void onNewBarcodeDetectedForCard(QString barcode);
	void onInvoked(const bb::system::InvokeRequest&);
	void onCardPooled(const bb::system::CardDoneMessage&);


signals:


private:
	Label *barcodeLabel;
	Camera *camera;
	BarcodeDecoderControl *barcodeDecoder;
	NavigationPane *mainPane;

	//Card
	InvokeManager *_invokeManager;
	bool _invoked;
	bool _pooled;

	//Audio
	MediaPlayer *player;


	void initalizeSounds();

};
}
}

#endif // ifndef BARCODE_SCANNER_APP_H
