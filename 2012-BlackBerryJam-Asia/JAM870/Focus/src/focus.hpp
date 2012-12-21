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
#ifndef __FOCUS_HPP__
#define __FOCUS_HPP__

#include <QtCore/QObject>
#include <QtCore/QMetaType>

#include <bb/cascades/ForeignWindowControl>
#include <bb/cascades/Button>
#include <bb/cascades/Slider>
#include <bb/cascades/DropDown>
#include <bb/cascades/TouchEvent>
#include <bb/cascades/ImageView>
#include <bb/cascades/Application>

#include <camera/camera_api.h>

using namespace bb::cascades;


class Focus : public QObject
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
    void onRefocus();
    void onManualFocusChanged(float value);
    void onFocusModeChanged(bb::cascades::Option* option);
    void onFocusTouch(bb::cascades::TouchEvent *event);
    void onViewfinderFrameChanged(const QRectF & frame);
    void onStatusChanged(camera_handle_t handle,
                         camera_devstatus_t status,
                         uint16_t extra,
                         void *arg);

public:
    Focus(bb::cascades::Application *app);
    ~Focus();

signals:
    void pictureSaved();
    void statusChanged(camera_handle_t handle,
                       camera_devstatus_t status,
                       uint16_t extra,
                       void *arg);

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
    void updateFocusList();
    void setFacePriority();
    static void cameraStatusCallback(camera_handle_t handle,
                                     camera_devstatus_t status,
                                     uint16_t extra,
                                     void *arg);

    ForeignWindowControl *mViewfinderWindow;
    Button *mStartFrontButton;
    Button *mStartRearButton;
    Button *mStopButton;
    Button *mTakePictureButton;
    Button *mRefocusButton;
    Slider *mManualFocusSlider;
    DropDown *mFocusList;
    int mCurManualStep;
    int mMaxManualStep;
    camera_focusmode_t mFocusMode;
    camera_handle_t mCameraHandle;
    camera_unit_t mCameraUnit;
    QRectF mVfWindowGeometry;
    ImageView *mFocusIndicator;
};

#endif // ifndef __FOCUS_HPP__
