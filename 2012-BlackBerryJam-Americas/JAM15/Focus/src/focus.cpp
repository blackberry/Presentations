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
#include <bb/cascades/AbsoluteLayout>
#include <bb/cascades/LayoutProperties>
#include <bb/cascades/WindowProperty>
#include <bb/cascades/Button>
#include <bb/cascades/Page>
#include <bb/cascades/DropDown>
#include <bb/cascades/Option>
#include <bb/cascades/TouchEvent>
#include <bb/cascades/ImageView>

#include <camera/camera_api.h>
#include <screen/screen.h>
#include <bps/soundplayer.h>
#include <unistd.h>
#include "Focus.hpp"

using namespace bb::cascades;

// workaround a ForeignWindowControl race condition
#define WORKAROUND_FWC


// qDebug() now logs to slogger2, which I find inconvenient since the NDK does not pick this up in the console,
// so I am installing a custom handler to log to stderr.
static void log_to_stderr(QtMsgType msgType, const char *msg)
{
    (void)msgType;  // go away, warning!
    fprintf(stderr, "%s\n", msg);
}

static const char* getFocusName(camera_focusmode_t mode)
{
    switch(mode) {
    case CAMERA_FOCUSMODE_OFF:
        return "Off";
    case CAMERA_FOCUSMODE_EDOF:
        return "EDOF";
    case CAMERA_FOCUSMODE_MANUAL:
        return "Manual";
    case CAMERA_FOCUSMODE_AUTO:
        return "Auto S-AF";
    case CAMERA_FOCUSMODE_MACRO:
        return "Macro S-AF";
    case CAMERA_FOCUSMODE_CONTINUOUS_AUTO:
        return "Auto C-AF";
    case CAMERA_FOCUSMODE_CONTINUOUS_MACRO:
        return "Macro C-AF";
    default:
        return NULL;
    }
}


Focus::Focus() :
        mCameraHandle(CAMERA_HANDLE_INVALID)
{
    // install custom logging handler
    qInstallMsgHandler(log_to_stderr);
    qDebug() << "Focus";

    // create our foreign window
    mViewfinderWindow = ForeignWindowControl::create()
        .windowId(QString("cameraViewfinder"))
        .onTouch(this, SLOT(onFocusTouch(bb::cascades::TouchEvent*)));
    // Allow Cascades to update the native window's size, position, and visibility, but not the source-size.
    // Cascades may otherwise attempt to redefine the buffer source-size to match the window size, which would yield
    // undesirable results.  You can experiment with this if you want to see what I mean.
    mViewfinderWindow->setUpdatedProperties(WindowProperty::Position | WindowProperty::Size | WindowProperty::Visible);

    QObject::connect(mViewfinderWindow,
                     SIGNAL(windowAttached(screen_window_t, const QString &, const QString &)),
                     this, SLOT(onWindowAttached(screen_window_t, const QString &,const QString &)));
    QObject::connect(mViewfinderWindow,
                     SIGNAL(controlFrameChanged(const QRectF &)),
                     this, SLOT(onViewfinderFrameChanged(const QRectF &)));

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
    mRefocusButton = Button::create("Refocus")
        .preferredWidth(300)
        .onClicked(this, SLOT(onRefocus()));
    mRefocusButton->setVisible(false);
    // NOTE: we are using onImmediateValueChanged() so we can see focus updates
    // in realtime, rather than waiting for the user to let go of the slider.
    mManualFocusSlider = Slider::create()
        .from(0.0).to(1.0)
        .onImmediateValueChanged(this, SLOT(onManualFocusChanged(float)));
    mManualFocusSlider->setVisible(false);

    // pick a default such that the touch-to-focus code won't run
    mFocusMode = CAMERA_FOCUSMODE_OFF;

    // a drop-down box which we will populate with available scene modes
    mFocusList = DropDown::create()
        .preferredWidth(350);
    mFocusList->setVisible(false);
    QObject::connect(mFocusList, SIGNAL(selectedOptionChanged(bb::cascades::Option*)),
                     this, SLOT(onFocusModeChanged(bb::cascades::Option*)));

    // load up the focus indicator
    mFocusIndicator = ImageView::create("asset:///focusindicator.png");
    mFocusIndicator->setVisible(false);

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
             .vertical(VerticalAlignment::Top)
             .add(mFocusIndicator))
        .add(Container::create()
             .horizontal(HorizontalAlignment::Left)
             .vertical(VerticalAlignment::Center)
             .rz(270)
             .add(mManualFocusSlider)
                 .translate(-300,0))
        .add(Container::create()
             .horizontal(HorizontalAlignment::Right)
             .vertical(VerticalAlignment::Top)
             .add(mFocusList))
         .add(Container::create()
              .horizontal(HorizontalAlignment::Left)
              .vertical(VerticalAlignment::Top)
              .add(mRefocusButton))
        .add(Container::create()
            .horizontal(HorizontalAlignment::Center)
            .vertical(VerticalAlignment::Bottom)
            .layout(StackLayout::create()
                .orientation(LayoutOrientation::LeftToRight))
            .add(mStartFrontButton)
            .add(mStartRearButton)
            .add(mTakePictureButton)
            .add(mStopButton));

   Application::instance()->setScene(Page::create().content(container));
}


Focus::~Focus()
{
    delete mViewfinderWindow;
}


void Focus::onWindowAttached(screen_window_t win,
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
#ifdef WORKAROUND_FWC
    // seems we still need a workaround in R9 for a potential race due to
    // ForeignWindowControl updating/flushing the window's properties in
    // parallel with the execution of the onWindowAttached() handler.
    mViewfinderWindow->setVisible(false);
    mViewfinderWindow->setVisible(true);
#endif
}


int Focus::createViewfinder(camera_unit_t cameraUnit,
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
    if (camera_set_photovf_property(mCameraHandle,
                                    CAMERA_IMGPROP_WIN_GROUPID, group.toStdString().c_str(),
                                    CAMERA_IMGPROP_WIN_ID, id.toStdString().c_str()) == CAMERA_EOK) {
        qDebug() << "viewfinder configured";
        if (camera_start_photo_viewfinder(mCameraHandle,
                                          NULL,
                                          &cameraStatusCallback,  // status callback to catch focus events
                                          (void*)this) == CAMERA_EOK) {
            qDebug() << "viewfinder started";
            // toggle button visibility...
            mStartFrontButton->setVisible(false);
            mStartRearButton->setVisible(false);
            mStopButton->setVisible(true);
            mTakePictureButton->setVisible(true);
            mTakePictureButton->setEnabled(true);
            mRefocusButton->setVisible(false);
            updateFocusList();
            mFocusList->setVisible(true);
            return EOK;
        }
    }
    qDebug() << "couldn't start viewfinder";
    camera_close(mCameraHandle);
    mCameraHandle = CAMERA_HANDLE_INVALID;
    return EIO;
}


void Focus::shutterCallback(camera_handle_t handle,
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


void Focus::stillCallback(camera_handle_t handle,
                          camera_buffer_t *buf,
                          void *arg)
{
    qDebug() << "stillCallback";
    Focus* inst = (Focus*)arg;
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


void Focus::onStartFront()
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


void Focus::onStartRear()
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


void Focus::onStopCamera()
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
        mFocusList->setVisible(false);
        mManualFocusSlider->setVisible(false);
        mRefocusButton->setVisible(false);
        mFocusMode = CAMERA_FOCUSMODE_OFF;
        mFocusIndicator->setVisible(false);
    }
}


void Focus::onTakePicture()
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


void Focus::onManualFocusChanged(float value)
{
    qDebug() << "new manual focus value" << value;
    // scale from [0..1] range to [0..mMaxManualStep]
    mCurManualStep = (int)((float)mMaxManualStep * value);
    if (mCameraHandle != CAMERA_HANDLE_INVALID) {
        camera_set_manual_focus_step(mCameraHandle,
                                     mCurManualStep);
    }
}


void Focus::onFocusModeChanged(Option *option)
{
    if (!option) return;
    qDebug() << "new focus mode selected: " << option->text();
    mFocusMode = (camera_focusmode_t)option->value().toInt();
    // apply new focus mode, but first, clear any user-defined regions and hide the indicator
    mFocusIndicator->setVisible(false);
    camera_set_focus_regions(mCameraHandle, 0, NULL);
    setFacePriority();
    camera_set_focus_mode(mCameraHandle,
                          mFocusMode);
    // side-effects
    if (mFocusMode == CAMERA_FOCUSMODE_MANUAL) {
        // in manual mode, let's make a slider available
        camera_get_manual_focus_step(mCameraHandle,
                                     &mMaxManualStep,
                                     &mCurManualStep);
        mManualFocusSlider->setValue((float)mCurManualStep / (float)mMaxManualStep);
        mManualFocusSlider->setVisible(true);
    } else {
        // hide manual focus slider in non-manual modes
        mManualFocusSlider->setVisible(false);
    }
    // make the REFOCUS button visible if we're in a single-cycle focus mode
    if ((mFocusMode == CAMERA_FOCUSMODE_AUTO) ||
        (mFocusMode == CAMERA_FOCUSMODE_MACRO)) {
        mRefocusButton->setVisible(true);
    } else {
        mRefocusButton->setVisible(false);
    }
}


// this code is executed whenever a viewfinder is started.
// we need to query the set of supported focus modes, since it may change between
// various shooting modes (video, still, burst, etc.)
void Focus::updateFocusList()
{
    // first, wipe out the list
    mFocusList->removeAll();
    // now query the camera
    int num = 0;

    // find out what current focus mode is
    camera_get_focus_mode(mCameraHandle,
                          &mFocusMode);

    camera_focusmode_t list[CAMERA_FOCUSMODE_NUMFOCUSMODES];
    // note, we could use pre-sizing mode here to determine how many modes are supported before
    // bothering to query, but since the worst case is that ALL scene modes are supported,
    // we may as well just make list[] maximally sized.
    qDebug() << "query modes";
    camera_get_focus_modes(mCameraHandle,
                           sizeof(list)/sizeof(*list),
                           &num,
                           list);
    qDebug() << "ok.. got" << num;
    for (int i=0; i<num; i++) {
        const char *name = getFocusName(list[i]);
        if (name) {
            Option *option = Option::create().value(list[i]).text(name);
            qDebug() << "adding option " << option->text() << " to list";
            mFocusList->add(option);
            // whatever mode the camera reported as active, indicate this in the drop-down list
            if (list[i] == mFocusMode) {
                qDebug() << "marking option " << option << " as active";
                option->setSelected(true);
            }
        }
    }

}

void Focus::onRefocus()
{
    qDebug() << "onRefocus";
    // force a re-focus cycle, but first, clear any user-defined regions
    camera_set_focus_regions(mCameraHandle, 0, NULL);
    setFacePriority();
    camera_set_focus_mode(mCameraHandle,
                          mFocusMode);
}


void Focus::onFocusTouch(bb::cascades::TouchEvent *event)
{
    // only accept focus events we are in a touchable focus mode
    switch(mFocusMode) {
    case CAMERA_FOCUSMODE_AUTO:
    case CAMERA_FOCUSMODE_MACRO:
        if (event->isDown()) {
            // now find out what the physical camera viewfinder size is
            int camW, camH;
            camera_get_photovf_property(mCameraHandle,
                                        CAMERA_IMGPROP_WIDTH, &camW,
                                        CAMERA_IMGPROP_HEIGHT, &camH);
            // scale co-ordinates to camera frame of reference.
            // touch event co-ordinates are relative to the ForeignWindowControl size,
            // which is cached in the onViewfinderFrameChanged SLOT.
            // NOTE: I didn't implement it in this sample, but for a front-facing camera where
            // the display is mirrored, you would want to reverse one of the co-ordinate axes.
            int x = (event->windowX() - mVfWindowGeometry.x()) * camW / mVfWindowGeometry.width();
            int y = (event->windowY() - mVfWindowGeometry.y()) * camH / mVfWindowGeometry.height();
            // define a box approx 10% of the viewfinder in size
            int w = camW * 0.1;
            int h = camW * 0.1;
            x = x - w/2;
            y = y - h/2;
            // clip box to viewfinder limits
            if (x<0) x = 0;
            if (y<0) y = 0;
            if (x+w > camW) x = camW - w;
            if (y+w > camH) y = camH - h;
            camera_region_t region;
            region.left = x;
            region.top = y;
            region.width = w;
            region.height = h;
            qDebug() << "applying focus region (" << region.left << "," << region.top << ","
                     << region.width << "," << region.height << ")";
            // drive the region-focus algorithm by applying a region and then
            // re-applying the focus mode to trigger a search
            camera_set_focus_regions(mCameraHandle,
                                     1,
                                     &region);
            // clear face-priority override.  faces take priority over regions, so if it is
            // enabled, the region will not be activated.
            camera_set_3a_face_priority(mCameraHandle,
                                        0);
            camera_set_focus_mode(mCameraHandle,
                                  mFocusMode);
            // now a focus search should be in progress!
        }
        break;

    case CAMERA_FOCUSMODE_CONTINUOUS_AUTO:
    case CAMERA_FOCUSMODE_CONTINUOUS_MACRO:
        // even though region-priority is supported by the C-AF modes,
        // it is not as useful in this demo as a "touch to focus" implementation.
        // where this is more useful is if you wish to restrict focus to one
        // part of the scene (eg. only adjust focus on objects in the top-half of
        // the frame).
        break;
    default:
        // not in an appropriate mode
        break;
    }
}


void Focus::onViewfinderFrameChanged(const QRectF & frame)
{
    mVfWindowGeometry = frame;
    qDebug() << "Viewfinder size is" << frame.width() << "x" << frame.height();
}


void Focus::setFacePriority()
{
    int num;
    // this code seems lengthy, but it is the safest way to query how to apply face-focus priority.
    // you could blindly just call camera_set_3a_face_priority(..., CAMERA_3A_AUTOFOCUS);, but that
    // may not be guaranteed to work on all platforms.
    if (camera_get_3a_face_priority_modes(mCameraHandle,
                                          0,
                                          &num,
                                          NULL) != CAMERA_EOK) return;
    uint32_t* masks = new uint32_t[num];
    if (camera_get_3a_face_priority_modes(mCameraHandle,
                                          num,
                                          &num,
                                          masks) == CAMERA_EOK) {
        // we now have a list of valid 3A face-priority mask combinations.
        // let's pick the mask that contains CAMERA_3A_AUTOFOCUS and the smallest number of other
        // modes.
        uint32_t bestmask = 0;
        int bestbits = INT_MAX;
        for (int i=0; i<num; i++) {
            if (masks[i] & CAMERA_3A_AUTOFOCUS) {
                int count = 1;
                if (masks[i] & CAMERA_3A_AUTOEXPOSURE) count++;
                if (masks[i] & CAMERA_3A_AUTOWHITEBALANCE) count++;
                if (count < bestbits) {
                    bestbits = count;
                    bestmask = masks[i];
                }
            }
        }
        if (bestmask) {
            camera_set_3a_face_priority(mCameraHandle,
                                        bestmask);
        }
    }
    delete[] masks;
}


void Focus::cameraStatusCallback(camera_handle_t handle,
                                 camera_devstatus_t status,
                                 uint16_t extra,
                                 void* arg)
{
    Focus *inst = (Focus*)arg;
    if (status == CAMERA_STATUS_FOCUS_CHANGE) {
        // a focus event was received, query the current state of the focus algorithm
        camera_region_t region;
        camera_focusstate_t state;
        camera_get_focus_state(handle,
                               &state,
                               &region);
        if (state == CAMERA_FOCUSSTATE_LOCKED) {
            // the focus has locked, update the visual indicator
            qDebug() << "Focus LOCK at (" << region.left << "," << region.top << ","
                    << region.width << "," << region.height << ")";
            // re-scale the co-ordinates to the window size
            int camW, camH;
            camera_get_photovf_property(handle,
                                        CAMERA_IMGPROP_WIDTH, &camW,
                                        CAMERA_IMGPROP_HEIGHT, &camH);
            // NOTE: I didn't implement it in this sample, but for a front-facing camera where
            // the display is mirrored, you would want to reverse one of the co-ordinate axes.
            int x = region.left * inst->mVfWindowGeometry.width() / camW + inst->mVfWindowGeometry.x();
            int y = region.top * inst->mVfWindowGeometry.height() / camH + inst->mVfWindowGeometry.y();
            int w = region.width * inst->mVfWindowGeometry.width() / camW;
            int h = region.height * inst->mVfWindowGeometry.height() / camH;

            // reposition the container which holds the focus indicator.
            // NOTE: I am scaling the focus indicator to match the region size reported by the
            // camera.  This may lead to some stretched boxes on-screen, but that is just due
            // to the ISP implementation.  If you don't like how this looks, you can
            // just keep a square image and make sure to reposition it accordingly.
            // the math for doing this is beyond the scope of this sample, which is attempting
            // to provide the most accurate visual representation of the focus co-ordinates.
            Container *container = (Container*)inst->mFocusIndicator->parent();
            container->setPreferredSize(w, h);
            container->setTranslation(x,y);
            inst->mFocusIndicator->setVisible(true);
        } else {
            // the focus is not locked, clear the visual indicator
            inst->mFocusIndicator->setVisible(false);
        }
    }
}
