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
#ifndef __SCENEZOOM_HPP__
#define __SCENEZOOM_HPP__

#include <QtCore/QObject>
#include <QtCore/QMetaType>

#include <bb/cascades/ForeignWindowControl>
#include <bb/cascades/Button>
#include <bb/cascades/Slider>
#include <bb/cascades/DropDown>
#include <bb/cascades/Application>

#include <camera/camera_api.h>

using namespace bb::cascades;


class SceneZoom : public QObject
{
    Q_OBJECT
public slots:
    void onWindowAttached(screen_window_t win,
                          const QString &group,
                          const QString &id);
    void onStartFront();
    void onStartRear();
    void onStopCamera();
    void onTakePicture();
    void onZoomChanged(float value);
    void onSceneChanged(bb::cascades::Option* option);

public:
    SceneZoom(bb::cascades::Application *app);
    ~SceneZoom();

signals:
    void pictureSaved();

private:
    int createViewfinder(camera_unit_t cameraUnit,
                         const QString &group,
                         const QString &id);
    static void shutterCallback(camera_handle_t handle,
                                void *arg);
    static void stillCallback(camera_handle_t handle,
                              camera_buffer_t *buf,
                              void *arg);
    void checkMetadata(camera_buffer_t *buf);
    unsigned int cameraZoomLevel();
    void updateSceneList();

    ForeignWindowControl *mViewfinderWindow;
    Button *mStartFrontButton;
    Button *mStartRearButton;
    Button *mStopButton;
    Button *mTakePictureButton;
    Slider *mZoomSlider;
    DropDown *mSceneList;
    float mCurZoom;
    unsigned int mMaxZoom;
    unsigned int mMinZoom;
    camera_scenemode_t mSceneMode;
    camera_handle_t mCameraHandle;
    camera_unit_t mCameraUnit;
};

#endif // ifndef __SCENEZOOM_HPP__
