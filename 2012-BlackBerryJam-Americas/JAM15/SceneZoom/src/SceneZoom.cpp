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
#include <bb/cascades/Application>
#include <bb/cascades/ForeignWindowControl>
#include <bb/cascades/Container>
#include <bb/cascades/StackLayout>
#include <bb/cascades/DockLayout>
#include <bb/cascades/LayoutProperties>
#include <bb/cascades/WindowProperty>
#include <bb/cascades/Button>
#include <bb/cascades/Page>
#include <bb/cascades/DropDown>
#include <bb/cascades/Option>

#include <camera/camera_api.h>
#include <screen/screen.h>
#include <bps/soundplayer.h>
#include <unistd.h>
#include "SceneZoom.hpp"

using namespace bb::cascades;

// qDebug() now logs to slogger2, which I find inconvenient since the NDK does not pick this up in the console,
// so I am installing a custom handler to log to stderr.
static void log_to_stderr(QtMsgType msgType, const char *msg)
{
    (void)msgType;  // go away, warning!
    fprintf(stderr, "%s\n", msg);
}

static const char* getSceneName(camera_scenemode_t sceneMode)
{
    switch(sceneMode) {
    case CAMERA_SCENE_DEFAULT:
        return "Default";
    case CAMERA_SCENE_AUTO:
        return "Auto";
    case CAMERA_SCENE_SPORTS:
        return "Sports";
    case CAMERA_SCENE_WHITEBOARD:
        return "Whiteboard";
    case CAMERA_SCENE_CLOSEUP:
        return "Closeup";
    case CAMERA_SCENE_BUSINESSCARD:
        return "Business Card";
    case CAMERA_SCENE_BARCODE:
        return "Barcode";
    case CAMERA_SCENE_ACTION:
        return "Action";
    case CAMERA_SCENE_TEST:
        return "Test";
    case CAMERA_SCENE_BEACHANDSNOW:
        return "Beach & Snow";
    case CAMERA_SCENE_NIGHT:
        return "Night";
    default:
        return NULL;
    }
}


SceneZoom::SceneZoom(bb::cascades::Application *app) :
        QObject(app),
        mCameraHandle(CAMERA_HANDLE_INVALID)
{
    // install custom logging handler
    qInstallMsgHandler(log_to_stderr);
    qDebug() << "SceneZoom";

    // create our foreign window
    mViewfinderWindow = ForeignWindowControl::create()
        .windowId(QString("cameraViewfinder"));
    // Allow Cascades to update the native window's size, position, and visibility, but not the source-size.
    // Cascades may otherwise attempt to redefine the buffer source-size to match the window size, which would yield
    // undesirable results.  You can experiment with this if you want to see what I mean.
    mViewfinderWindow->setUpdatedProperties(WindowProperty::Position | WindowProperty::Size | WindowProperty::Visible);

    QObject::connect(mViewfinderWindow,
                     SIGNAL(windowAttached(screen_window_t, const QString &, const QString &)),
                     this, SLOT(onWindowAttached(screen_window_t, const QString &,const QString &)));

    // create a bunch of camera control buttons
    // NOTE: some of these buttons are not initially visible
    mStartFrontButton = Button::create("Front Camera")
        .onClicked(this, SLOT(onStartFront()));
    mStartRearButton = Button::create("Rear Camera")
        .onClicked(this, SLOT(onStartRear()));
    mStopButton = Button::create("Stop Camera")
        .onClicked(this, SLOT(onStopCamera()));
    mStopButton->setVisible(false);
    mTakePictureButton = Button::create("Take Picture")
        .onClicked(this, SLOT(onTakePicture()));
    mTakePictureButton->setVisible(false);
    mCurZoom = 0.0;
    // NOTE: we are using onImmediateValueChanged() so we can see zoom updates
    // in realtime, rather than waiting for the user to let go of the slider.
    mZoomSlider = Slider::create()
        .from(0).to(1.0).value(mCurZoom).
        onImmediateValueChanged(this, SLOT(onZoomChanged(float)));

    // a drop-down box which we will populate with available scene modes
    mSceneList = DropDown::create()
        .preferredWidth(400);
    mSceneList->setVisible(false);

    // NOTE: need to be explicit with namespaces when using QObject::connect().
    // Can't just use selectedOptionChanged(Option*).
    QObject::connect(mSceneList, SIGNAL(selectedOptionChanged(bb::cascades::Option*)),
                     this, SLOT(onSceneChanged(bb::cascades::Option*)));
    // note that since saving pictures happens in a different thread,
    // we need to use a signal/slot in order to re-enable the 'take picture' button.
    QObject::connect(this,
                     SIGNAL(pictureSaved()), mTakePictureButton, SLOT(resetEnabled()));

    // using dock layout mainly.  the viewfinder foreign window sits in the center,
    // and the buttons live in their own container at the bottom.
    Container* container = Container::create()
        .layout(DockLayout::create())
        .add(Container::create()
            .horizontal(HorizontalAlignment::Center)
            .vertical(VerticalAlignment::Center)
            .add(mViewfinderWindow))
        .add(Container::create()
             .horizontal(HorizontalAlignment::Left)
             .vertical(VerticalAlignment::Center)
             .rz(270)
             .add(mZoomSlider)
                 .translate(-300,0))
        .add(Container::create()
             .horizontal(HorizontalAlignment::Right)
             .vertical(VerticalAlignment::Top)
             .add(mSceneList))
        .add(Container::create()
            .horizontal(HorizontalAlignment::Center)
            .vertical(VerticalAlignment::Bottom)
            .layout(StackLayout::create()
                .orientation(LayoutOrientation::LeftToRight))
            .add(mStartFrontButton)
            .add(mStartRearButton)
            .add(mTakePictureButton)
            .add(mStopButton));

   app->setScene(Page::create().content(container));
}


SceneZoom::~SceneZoom()
{
}


void SceneZoom::onWindowAttached(screen_window_t win,
                                 const QString &group,
                                 const QString &id)
{
    qDebug() << "onWindowAttached: " << win << ", " << group << ", " << id;
    // set screen properties to mirror if this is the front-facing camera
    int i = (mCameraUnit == CAMERA_UNIT_FRONT);
    screen_set_window_property_iv(win, SCREEN_PROPERTY_MIRROR, &i);
    // put the viewfinder window behind the cascades window
    i = -1;
    screen_set_window_property_iv(win, SCREEN_PROPERTY_ZORDER, &i);
    screen_context_t screen_ctx;
    screen_get_window_property_pv(win, SCREEN_PROPERTY_CONTEXT, (void **)&screen_ctx);
    screen_flush_context(screen_ctx, 0);
}


int SceneZoom::createViewfinder(camera_unit_t cameraUnit,
                                const QString &group,
                                const QString &id)
{
    qDebug() << "createViewfinder";
    if (mCameraHandle != CAMERA_HANDLE_INVALID) {
        qDebug() << "camera already running";
        return EBUSY;
    }
    mCameraUnit = cameraUnit;
    if (camera_open(mCameraUnit,
                    CAMERA_MODE_RW | CAMERA_MODE_ROLL,
                    &mCameraHandle) != CAMERA_EOK) {
        qDebug() << "could not open camera";
        return EIO;
    }
    qDebug() << "camera opened";
    mMinZoom = mMaxZoom = 0;
    if (camera_has_feature(mCameraHandle, CAMERA_FEATURE_PHOTOZOOM)) {
        camera_get_zoom_limits(mCameraHandle, &mMinZoom, &mMaxZoom, NULL);
    }

    if (camera_set_photovf_property(mCameraHandle,
                                    // NOTE: you can change zoom before viewfinder is even started!
                                    CAMERA_IMGPROP_ZOOMFACTOR, cameraZoomLevel(),
                                    CAMERA_IMGPROP_WIN_GROUPID, group.toStdString().c_str(),
                                    CAMERA_IMGPROP_WIN_ID, id.toStdString().c_str()) == CAMERA_EOK) {
        qDebug() << "viewfinder configured";
        if (camera_start_photo_viewfinder(mCameraHandle, NULL, NULL, NULL) == CAMERA_EOK) {
            qDebug() << "viewfinder started";
            // toggle button visibility...
            mStartFrontButton->setVisible(false);
            mStartRearButton->setVisible(false);
            mStopButton->setVisible(true);
            mTakePictureButton->setVisible(true);
            mTakePictureButton->setEnabled(true);
            updateSceneList();
            mSceneList->setVisible(true);
            return EOK;
        }
    }
    qDebug() << "couldn't start viewfinder";
    camera_close(mCameraHandle);
    mCameraHandle = CAMERA_HANDLE_INVALID;
    return EIO;
}


void SceneZoom::shutterCallback(camera_handle_t handle,
                                void *arg)
{
    qDebug() << "shutterCallback";

    // THE CAMERA SERVICE DOES NOT PLAY SOUNDS WHEN PICTURES ARE TAKEN OR
    // VIDEOS ARE RECORDED.  IT IS THE APP DEVELOPER'S RESPONSIBILITY TO
    // PLAY AN AUDIBLE SHUTTER SOUND WHEN A PICTURE IS TAKEN AND WHEN VIDEO
    // RECORDING STARTS AND STOPS.  NOTE THAT WHILE YOU MAY CHOOSE TO MUTE
    // SUCH SOUNDS, YOU MUST ENSURE THAT YOUR APP ADHERES TO ALL LOCAL LAWS
    // OF REGIONS WHERE IT IS DISTRIBUTED.  FOR EXAMPLE, IT IS ILLEGAL TO
    // MUTE OR MODIFY THE SHUTTER SOUND OF A CAMERA APPLICATION IN JAPAN OR
    // KOREA.
    // TBD:
    //   RIM will be providing clarification of this policy as part of the
    //   NDK developer agreement and App World guidelines.  A link will
    //   be provided when the policy is publicly available.
    soundplayer_play_sound("event_camera_shutter");

    // this is just here to silence a compiler warning
    if (handle && arg) {}
}


void SceneZoom::stillCallback(camera_handle_t handle,
                              camera_buffer_t *buf,
                              void *arg)
{
    qDebug() << "stillCallback";
    SceneZoom* inst = (SceneZoom*)arg;
    if (buf->frametype == CAMERA_FRAMETYPE_JPEG) {
        qDebug() << "still image size: " << buf->framedesc.jpeg.bufsize;
        int fd;
        char filename[CAMERA_ROLL_NAMELEN];
        if (camera_roll_open_photo(handle,
                                   &fd,
                                   filename,
                                   sizeof(filename),
                                   CAMERA_ROLL_PHOTO_FMT_JPG) == CAMERA_EOK) {
            qDebug() << "saving " << filename;
            int index = 0;
            while(index < (int)buf->framedesc.jpeg.bufsize) {
                int rc = write(fd, &buf->framebuf[index], buf->framedesc.jpeg.bufsize-index);
                if (rc > 0) {
                    index += rc;
                } else if (rc == -1) {
                    if ((errno == EAGAIN) || (errno == EINTR)) continue;
                    qDebug() << "error saving: " << strerror(errno);
                    break;
                }
            }
            close(fd);
        }
    }
    qDebug() << "re-enabling button";
    emit inst->pictureSaved();
}


void SceneZoom::onStartFront()
{
    qDebug() << "onStartFront";
    if (mViewfinderWindow) {
        // create a window and see if we can catch the join
        if (createViewfinder(CAMERA_UNIT_FRONT,
                             mViewfinderWindow->windowGroup().toStdString().c_str(),
                             mViewfinderWindow->windowId().toStdString().c_str()) == EOK) {
        }
    }
}


void SceneZoom::onStartRear()
{
    qDebug() << "onStartRear";
    if (mViewfinderWindow) {
        // create a window and see if we can catch the join
        if (createViewfinder(CAMERA_UNIT_REAR,
                             mViewfinderWindow->windowGroup().toStdString().c_str(),
                             mViewfinderWindow->windowId().toStdString().c_str()) == EOK) {
        }
    }
}


void SceneZoom::onStopCamera()
{
    qDebug() << "onStopCamera";
    if (mCameraHandle != CAMERA_HANDLE_INVALID) {
        // closing the camera handle causes the viewfinder to stop which will in turn
        // cause it to detach from the foreign window
        camera_close(mCameraHandle);
        mCameraHandle = CAMERA_HANDLE_INVALID;
        // reset button visibility
        mTakePictureButton->setVisible(false);
        mStopButton->setVisible(false);
        mStartFrontButton->setVisible(true);
        mStartRearButton->setVisible(true);
        mSceneList->setVisible(false);
    }
}


void SceneZoom::onTakePicture()
{
    qDebug() << "onTakePicture";
    if (mCameraHandle != CAMERA_HANDLE_INVALID) {
        camera_take_photo(mCameraHandle,
                          &shutterCallback,
                          NULL,
                          NULL,
                          &stillCallback,
                          (void*)this,
                          false);
        // disable take-picture button until we're ready to take a picture again
        mTakePictureButton->setEnabled(false);
    }
}


unsigned int SceneZoom::cameraZoomLevel()
{
    if (mMaxZoom == 0) return 0;
    // scale from [0..1] range to [mMinZoom..mMaxZoom] range
    return (mMaxZoom - mMinZoom) * mCurZoom + mMinZoom;
}


void SceneZoom::onZoomChanged(float value)
{
    qDebug() << "new zoom value" << value;
    // cache value in case the camera isn't running
    mCurZoom = value;
    if (mCameraHandle != CAMERA_HANDLE_INVALID) {
        // push new zoom setting to the camera
        camera_set_zoom(mCameraHandle, cameraZoomLevel(), false);
    }
}


void SceneZoom::onSceneChanged(Option *option)
{
    if (!option) return;
    qDebug() << "new scene selected: " << option->text();
    camera_set_scene_mode(mCameraHandle,
                          (camera_scenemode_t)option->value().toInt());
}


// this code is executed whenever a viewfinder is started.
// we need to query the set of supported scene lists, since it may change between
// various shooting modes (video, still, burst, etc.)
void SceneZoom::updateSceneList()
{
    // first, wipe out the list
    mSceneList->removeAll();
    // now query the camera
    int num = 0;
    camera_scenemode_t list[CAMERA_SCENE_NUMSCENEMODES];
    // note, we could use pre-sizing mode here to determine how many modes are supported before
    // bothering to query, but since the worst case is that ALL scene modes are supported,
    // we may as well just make list[] maximally sized.
    camera_get_scene_modes(mCameraHandle,
                           sizeof(list)/sizeof(*list),
                           &num,
                           list);
    for (int i=0; i<num; i++) {
        const char *name = getSceneName(list[i]);
        if (name) {
            Option *option = Option::create().value(list[i]).text(name);
            qDebug() << "adding option " << option->text() << " to list";
            mSceneList->add(option);
            if (list[i] == CAMERA_SCENE_DEFAULT) {
                // NOTE: there is no camera_get_scene_mode() function currently, so there is no good way
                // to determine what the initial mode is set to when starting a viewfinder.
                // By convention, we always select "DEFAULT" internally, which is usually an alias to
                // "AUTO".  But since that may not always be the case, let's display "DEFAULT" as the
                // currently selected scene mode in our drop-down list.
                qDebug() << "marking option " << option << " as active";
                option->setSelected(true);
            }
        }
    }
}
