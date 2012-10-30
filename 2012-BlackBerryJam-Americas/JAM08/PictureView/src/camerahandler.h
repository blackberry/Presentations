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
#ifndef __CAMERAHANDLER_H__
#define __CAMERAHANDLER_H__

#include <QtCore/QObject>
#include <QtCore/QMetaType>

#include <bb/cascades/Application>
#include <bb/cascades/multimedia/Camera>
#include <bb/cascades/Button>


using namespace bb::cascades;
using namespace bb::cascades::multimedia;


class CameraHandler: public QObject
{
Q_OBJECT

public:
	/**
	 * This is our constructor that sets up the application. We register the multi-media library
	 * Camera in QML so it can accessed from the QML code.
	 */
    CameraHandler();
    ~CameraHandler();

    void setupControls(const QString &camName, const QString &vfButtonName);
    void setAbstractPane(const bb::cascades::AbstractPane *abspane);

    Q_INVOKABLE void camerastart();
    Q_INVOKABLE void camerastop();


private slots:


    void onOpenSuccess();

    // When we get a ShutterFired event from the system, we play a shutter sound and take a pic
    void onShutterFired();

    //When the button is clicked
    void onButtonClick();


    //When viewfinder has been successfully stopped
    void onStopVfSuccess();

    //when viewfinder has been successfully started
    void onStartVfSuccess();


private:
    Camera *m_camera;
    Button *m_camButton;
    bool m_ViewfinderOn;
    bool m_cameraopen;
    const bb::cascades::AbstractPane *m_abstractPane;


};

#endif // ifndef __CAMERAHANDLER_H__
