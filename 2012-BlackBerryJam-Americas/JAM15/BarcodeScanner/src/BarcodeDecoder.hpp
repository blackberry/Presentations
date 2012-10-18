#ifndef BARCODE_DECODER_H
#define BARCODE_DECODER_H

#include <zxing/MultiFormatReader.h>
#include <QObject>
#include <QSharedPointer>
#include <bb/cascades/multimedia/Camera>
#include <bb/cascades/multimedia/CameraSettings.hpp>
#include <bb/cascades/CustomControl>
#include <bb/cascades/Container>
#include <bb/cascades/StackLayout>

using namespace zxing;
using namespace bb::cascades;
using namespace bb::cascades::multimedia;

namespace bb {
namespace community {
namespace barcode {
class BarcodeDecoderControl : public bb::cascades::CustomControl {
	Q_OBJECT


public:
	BarcodeDecoderControl(Container *parent = 0);
	Camera getCamera();

	Q_INVOKABLE void startScanning();
	Q_INVOKABLE void stopScanning();

public slots:

	void onPreviewFrameAvailable(bb::cascades::multimedia::SharedUCharPointer previewBuffer, quint64 size,
	                                unsigned int width, unsigned int height, unsigned int stride);
	void onCameraOpened();


signals:
	void newBarcodeDetected(QString barcode);
	void scanningStarted();
	void scanningStopped();

private:
	Ref<MultiFormatReader> reader;
	QString barcodeData;
	Camera *_camera;
	CameraSettings *_cameraSettings;

	QTime _lastFocusTime;
	static const int _focusInterval = 1200;

};
}
}
}
#endif //#ifndef BARCODE_DECODER_H
