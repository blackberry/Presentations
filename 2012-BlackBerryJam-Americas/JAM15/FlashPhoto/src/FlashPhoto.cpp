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

#include <camera/camera_api.h>
#include <camera/camera_meta.h>
#include <screen/screen.h>
#include <bps/soundplayer.h>
#include <unistd.h>
#include <stdint.h>
#include <libexif/exif-data.h>  // NOTE: libexif is broken in the NDK I am using. _stdint.h is missing
#include "FlashPhoto.hpp"

using namespace bb::cascades;

//#define DUMP_EXIF


// qDebug() now logs to slogger2, which I find inconvenient since the NDK does not pick this up in the console,
// so I am installing a custom handler to log to stderr.
static void log_to_stderr(QtMsgType msgType, const char *msg)
{
    (void)msgType;  // go away, warning!
    fprintf(stderr, "%s\n", msg);
}

static const char* flashModeStr(camera_flashmode_t mode) {
    switch(mode) {
    case CAMERA_FLASH_AUTO:
        return "FLASH AUTO";
    case CAMERA_FLASH_ON:
        return "FLASH   ON";
    case CAMERA_FLASH_OFF:
        return "FLASH  OFF";
    default:
        return "FLASH ????";
    }
}


FlashPhoto::FlashPhoto(bb::cascades::Application *app) :
        QObject(app),
        mCameraHandle(CAMERA_HANDLE_INVALID)
{
    // install custom logging handler
    qInstallMsgHandler(log_to_stderr);
    qDebug() << "FlashPhoto";

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
    mFlashMode = CAMERA_FLASH_AUTO;
    mFlashButton = Button::create(flashModeStr(mFlashMode))
        .onClicked(this, SLOT(onToggleFlash()));
    mFlashButton->setVisible(false);

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
             .horizontal(HorizontalAlignment::Right)
             .vertical(VerticalAlignment::Top)
             .layout(StackLayout::create()
                     .orientation(LayoutOrientation::TopToBottom))
             .add(mFlashButton))
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


FlashPhoto::~FlashPhoto()
{
}


void FlashPhoto::onWindowAttached(screen_window_t win,
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


int FlashPhoto::createViewfinder(camera_unit_t cameraUnit,
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
        if (camera_start_photo_viewfinder(mCameraHandle, NULL, NULL, NULL) == CAMERA_EOK) {
            qDebug() << "viewfinder started";
            // toggle button visibility...
            mStartFrontButton->setVisible(false);
            mStartRearButton->setVisible(false);
            mStopButton->setVisible(true);
            mTakePictureButton->setVisible(true);
            mTakePictureButton->setEnabled(true);
            // AUTO is always the default flash mode if supported
            mFlashMode = CAMERA_FLASH_AUTO;
            mFlashButton->setText(flashModeStr(mFlashMode));
            // we'll make the flash button visible always, but only enable it
            // if the flash feature is supported
            mFlashButton->setVisible(true);
            mFlashButton->setEnabled(false);
            // check if flash feature is currently available
            if (camera_can_feature(mCameraHandle, CAMERA_FEATURE_FLASH)) {
                if (camera_config_flash(mCameraHandle, mFlashMode) == EOK) {
                    mFlashButton->setEnabled(true);
                } else {
                    mFlashButton->setEnabled(false);
                }
            }
            return EOK;
        }
    }
    qDebug() << "couldn't start viewfinder";
    camera_close(mCameraHandle);
    mCameraHandle = CAMERA_HANDLE_INVALID;
    return EIO;
}


void FlashPhoto::shutterCallback(camera_handle_t handle,
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


// this is the metadata consumer function.  it is invoked for each piece of
// metadata found attached to a given camera buffer.  return true to continue
// processing metadata, or false to stop.
bool FlashPhoto::metaConsumer(camera_metadata_t metadata,
                              camera_metaformat_t format,
                              void *arg)
{
    if (format == CAMERA_METAFORMAT_EXIF) {
        qDebug() << "EXIF metadata found.";
        unsigned char *ptr;
        uint64_t size;
        if (camera_meta_get_exif(metadata, &ptr, &size) != EOK) {
            qDebug() << "failed to get exif pointer";
        } else {
            ExifData *exifData = exif_data_new_from_data(ptr, size);
            if (!exifData) {
                qDebug() << "couldn't find ExifData";
            } else {
    #ifdef DUMP_EXIF
                exif_data_dump(exifData);
    #endif
                ExifByteOrder byteOrder = exif_data_get_byte_order(exifData);
                ExifEntry* entry = exif_data_get_entry(exifData, EXIF_TAG_FLASH);
                if (entry) {
                    // EXIF_TAG_FLASH is supposed to be a uint16_t
                    if (entry->format == EXIF_FORMAT_SHORT) {
                        uint16_t flashVal = exif_get_short(entry->data, byteOrder);
                        qDebug() << "EXIF FLASH TAG = " << flashVal;
                        // EXIF standard defines a bunch of values for flash tags, but upon
                        // a quick glance, it looks like the LSB indicates whether it fired or not.
                        if (flashVal & 0x0001) {
                            // play a sound if the flash fired
                            soundplayer_play_sound("event_recording_start");
                        }
                    } else {
                        qDebug() << "unexpected EXIF_TAG_FLASH format: " << exif_format_get_name(entry->format);
                    }
                } else {
                    qDebug() << "no flash info found in EXIF";
                }
                exif_data_free(exifData);
            }

        }
        // don't bother processing additional metadata, since we already found what we need.
        return false;
    } else {
        // there are some other formats defined, but we only care about EXIF in this sample
        qDebug() << "metadata format" << format << "ignored.";
    }
    // continue processing metadata
    return true;
}


void FlashPhoto::stillCallback(camera_handle_t handle,
                               camera_buffer_t *buf,
                               void *arg)
{
    qDebug() << "stillCallback";
    FlashPhoto* inst = (FlashPhoto*)arg;
    if (buf->frametype == CAMERA_FRAMETYPE_JPEG) {
        qDebug() << "still image size: " << buf->framedesc.jpeg.bufsize;
        int fd;
        char filename[CAMERA_ROLL_NAMELEN];
        // let's inspect the metadata
        camera_meta_iterate_metadata(buf,
                                     &metaConsumer,
                                     arg);
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


void FlashPhoto::onStartFront()
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


void FlashPhoto::onStartRear()
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


void FlashPhoto::onStopCamera()
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
        mFlashButton->setVisible(false);
    }
}


void FlashPhoto::onTakePicture()
{
    qDebug() << "onTakePicture";
    if (mCameraHandle != CAMERA_HANDLE_INVALID) {
        if (camera_has_feature(mCameraHandle, CAMERA_FEATURE_FLASH)) {
            // let's check the flash power and avoid taking a force-flash-on picture if the level is too low
            // this is just a demonstration, but shows how to use some of the APIs
            uint32_t percent = 0;
            camera_get_flash_level(mCameraHandle, &percent);
            if ((mFlashMode == CAMERA_FLASH_ON) && (percent < 10)) {
                qDebug() << "flash charge level is too low!";
                return;
            }
            qDebug() << "flash charge level is " << percent << "%";
        }

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


void FlashPhoto::onToggleFlash()
{
    qDebug() << "onToggleFlash";
    camera_flashmode_t newMode = mFlashMode;
    if (mFlashMode == CAMERA_FLASH_AUTO) {
        newMode = CAMERA_FLASH_ON;
    } else if (mFlashMode == CAMERA_FLASH_ON) {
        newMode = CAMERA_FLASH_OFF;
    } else if (mFlashMode == CAMERA_FLASH_OFF) {
        newMode = CAMERA_FLASH_AUTO;
    }
    if (camera_config_flash(mCameraHandle,
                            newMode) == CAMERA_EOK) {
        mFlashMode = newMode;
        mFlashButton->setText(flashModeStr(newMode));
    }
}

