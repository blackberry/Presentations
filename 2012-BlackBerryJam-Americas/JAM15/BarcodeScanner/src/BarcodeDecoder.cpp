/*
 * BarcodeDecoder.cpp
 *
 *  Created on: Jul 19, 2012
 *      Author: pbernhardt
 */

#include "BarcodeDecoder.hpp"

#include <zxing/MultiFormatReader.h>
#include <QObject>
#include <QSharedPointer>

#include <bb/cascades/multimedia/Camera>
#include <bb/cascades/multimedia/CameraSettings.hpp>
#include <bb/cascades/multimedia/CameraTypes.hpp>

#include <bb/cascades/Container>
#include <bb/cascades/StackLayout>

#include <zxing/common/GreyscaleLuminanceSource.h>
#include <zxing/common/HybridBinarizer.h>
#include <zxing/MultiFormatReader.h>

using namespace bb::community::barcode;
using namespace zxing;
using namespace bb::cascades::multimedia;
using namespace bb::cascades;

BarcodeDecoderControl::BarcodeDecoderControl(Container *parent) :
		CustomControl(parent) {

	qDebug() << "+++++++ Barcode control creation..." << endl;

	//Build the component
	_camera = new Camera();
	setRoot(_camera);

	//Prepare the camera
	_camera->open(CameraUnit::Rear);

	//Prepare the decoder
	reader = Ref<MultiFormatReader>(new MultiFormatReader());
	DecodeHints *hints = new DecodeHints();
	hints->addFormat(BarcodeFormat_QR_CODE);
	hints->addFormat(BarcodeFormat_EAN_8);
	hints->addFormat(BarcodeFormat_EAN_13);
	hints->addFormat(BarcodeFormat_UPC_A);
	hints->addFormat(BarcodeFormat_UPC_E);
	hints->addFormat(BarcodeFormat_DATA_MATRIX);
	hints->addFormat(BarcodeFormat_CODE_128);
	hints->addFormat(BarcodeFormat_CODE_39);
	hints->addFormat(BarcodeFormat_ITF);
	hints->addFormat(BarcodeFormat_AZTEC);
	reader.object_->setHints(*hints);

	QObject::connect(_camera, SIGNAL(cameraOpened()), this,
			SLOT(onCameraOpened()));

	QObject::connect(_camera, SIGNAL(viewfinderStarted()), this,
			SLOT(viewfinderStarted()));

	QObject::connect(_camera, SIGNAL(viewfinderStopped()), this,
			SLOT(viewfinderStopped()));


	qDebug() << "+++++++ Signals connected" << endl;

}

void BarcodeDecoderControl::onPreviewFrameAvailable(
		SharedUCharPointer previewBuffer, quint64 size, unsigned int width,
		unsigned int height, unsigned int stride) {

	try {
		qDebug() << "+++++++ Scanning frame" << endl;

		Ref<LuminanceSource> source(
				new GreyscaleLuminanceSource(previewBuffer.data(), stride,
						size / stride, 0, 0, width, height));
		Ref<Binarizer> binarizer(new HybridBinarizer(source));
		Ref<BinaryBitmap> bitmap(new BinaryBitmap(binarizer));
		Ref<Result> result = reader->decode(bitmap);
		result->getBarcodeFormat();

		QString newBarcodeData = QString::fromStdString(
				result->getText()->getText().data());
		qDebug() << "+++++++ Result: " << newBarcodeData << endl;

		if (newBarcodeData != barcodeData) {
			qDebug() << "+++++++ New barcode detected!";
			barcodeData = newBarcodeData;
			emit newBarcodeDetected(barcodeData);
		}

	} catch (std::exception &e) {
		qDebug() << "+++++++ ERROR: " << e.what() << endl;
	}
	_camera->addPreviewBuffer(previewBuffer, size);
}

void BarcodeDecoderControl::onCameraOpened() {
	qDebug() << "+++++++ Camera opened" << endl;
	_cameraSettings = new CameraSettings();
	_camera->getSettings(_cameraSettings);
	_cameraSettings->setCameraMode(CameraMode::Photo);
	_cameraSettings->setFocusMode(CameraFocusMode::ContinuousMacro);
	_camera->applySettings(_cameraSettings);

	qDebug() << "+++++++ Camera settings applied" << endl;

	quint64 bufferSize = _camera->previewBufferSize();
	//Use two buffers for double buffering goodness.
	_camera->addPreviewBuffer(QSharedPointer<unsigned char>(new unsigned char[bufferSize]), bufferSize);
	_camera->addPreviewBuffer(QSharedPointer<unsigned char>(new unsigned char[bufferSize]), bufferSize);

	QObject::connect(_camera,
			SIGNAL(previewFrameAvailable(bb::cascades::multimedia::SharedUCharPointer, quint64, unsigned int, unsigned int, unsigned int)),
			this,
			SLOT(onPreviewFrameAvailable(bb::cascades::multimedia::SharedUCharPointer, quint64, unsigned int, unsigned int, unsigned int)));

	_camera->startViewfinder();
	qDebug() << "+++++++ Viewfinder started" << endl;
}

void BarcodeDecoderControl::startScanning() {
	_camera->startViewfinder();
}

void BarcodeDecoderControl::stopScanning() {
	_camera->stopViewfinder();
}
