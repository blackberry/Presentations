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

class BarcodeScannerApp: public QObject {
	Q_OBJECT


public:
	BarcodeScannerApp(bb::cascades::Application *app);
	virtual ~BarcodeScannerApp() {
	}

public slots:

	void onStartScan();
	void onNewBarcodeDetected(QString barcode);


signals:


private:
	Label *barcodeLabel;
	Camera *camera;
	BarcodeDecoderControl *barcodeDecoder;
	NavigationPane *mainPane;

	//Audio
	MediaPlayer *player;


	void initalizeSounds();

};
}
}

#endif // ifndef BARCODE_SCANNER_APP_H
