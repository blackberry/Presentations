/* Copyright (c) 2012 Research In Motion Limited.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "camerahandler.h"

#include <QImage>
#include <QImageReader>


#include <bb/cascades/Page>
#include <bb/cascades/QmlDocument>

#include <bb/cascades/multimedia/Camera>
#include <bps/soundplayer.h>

#include <iostream.h>

using namespace bb::cascades;
using namespace bb::cascades::multimedia;



CameraHandler::CameraHandler()
:m_camera(NULL), m_camButton(NULL), m_ViewfinderOn(false), m_cameraopen(false), m_abstractPane(NULL)
{

	// We need to register the QML types in the multimedia-library,
	// otherwise we will get an error from the QML.
	qmlRegisterType<Camera>("bb.cascades.multimedia",1,0,"Camera");

}


void CameraHandler::setupControls(const QString &camName, const QString &vfButtonName)
{
	m_camera = m_abstractPane->findChild<Camera *>(camName);
	m_camButton = m_abstractPane->findChild<Button *>(vfButtonName);

	if (m_camera) {

		//connect the signals with slots
		QObject::connect(m_camera, SIGNAL(cameraOpened()), this, SLOT(onOpenSuccess()));
		QObject::connect(m_camera, SIGNAL(shutterFired()), this, SLOT(onShutterFired()));
		QObject::connect(m_camera, SIGNAL(viewfinderStarted()), this, SLOT(onStartVfSuccess()));
		QObject::connect(m_camera, SIGNAL(viewfinderStopped()), this, SLOT(onStopVfSuccess()));

	}


	if (m_camButton) {
		QObject::connect(m_camButton, SIGNAL(clicked()), this, SLOT(onButtonClick()));

	}

	camerastart();
}

void CameraHandler::camerastart()
{
	std::cout << "camera open" << std::endl;
	m_camera->open(CameraUnit::Rear);
}

void CameraHandler::camerastop()
{
	std::cout << "camera closed" << std::endl;
	if ( m_cameraopen ) {
		m_camera->close();
		m_cameraopen = false;
	}
}

void CameraHandler::onButtonClick()
{


	if (!m_camera) return;

	if (m_ViewfinderOn) {
		m_camera->stopViewfinder();
	}
	else {
		m_camera->startViewfinder();
	}


}

void CameraHandler::onStopVfSuccess()
{
	std::cout << "stop viewfinder success" << std::endl;
	m_ViewfinderOn = false;
}

void CameraHandler::onStartVfSuccess()
{
	std::cout << "start viewfinder success" << std::endl;
	m_ViewfinderOn = true;
}




CameraHandler::~CameraHandler()
{
	if ((m_camera) && (m_cameraopen)) {
		m_camera->close();
	}

}


void CameraHandler::onOpenSuccess()
{
	 m_cameraopen = true;

	 if (m_camera) {
		 m_camera->startViewfinder();
	 }
}

void CameraHandler::onShutterFired()
{
	if ( m_camera ){
		// A cool trick here to play a sound. There are legal requirements in many countries to have a shutter-sound when
		// taking pictures. So we need this shutter sound if you are planning to submit you're app to app world.
		// So we play the shutter-fire sound when the onShutterFired event occurs.
		soundplayer_play_sound("event_camera_shutter");

	}
}

void CameraHandler::setAbstractPane(const bb::cascades::AbstractPane *abspane)
{
        m_abstractPane = abspane;
}


