/*
* Copyright (c) 2012 Research In Motion Limited.
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

#include <assert.h>
#include <bps/bps.h>
#include <bps/event.h>
#include <bps/navigator.h>
#include <bps/screen.h>
#include <sys/iofunc.h>
#include <fcntl.h>
#include <screen/screen.h>
#include <camera/camera_api.h>
#include <bps/soundplayer.h>
#include <pthread.h>
//#include <sys/neutrino.h>

/* The following is required to get XMP loaded */
#define UNIX_ENV
#include <cpp/string>
#define TXMP_STRING_TYPE std::string
#include <xmp/XMP.incl_cpp>
#include <xmp/XMP.hpp>
#include <cpp/iostream.h>
#include <cpp/fstream.h>
using namespace std;

//these are the namespaces necessary to read the face detect data
#define ADOBE_DIM_NS "http://ns.adobe.com/xap/1.0/sType/Dimensions#"
#define ADOBE_AREA_NS "http://ns.adobe.com/xmp/sType/Area#"
#define MWG_REGIONS_NS "http://www.metadataworkinggroup.com/schemas/regions/"
#define RIM_NS "http://www.rim.com" //TODO: For internal stuff, what should be the namespace?


// an arbitrary ZORDER for our application window.
#define APP_ZORDER (100)

typedef enum {
    STATE_STARTUP = 0,
    STATE_VIEWFINDER,
    STATE_RECORDING
} state_t;

// the pulse code we'll be using for the filter sigevent
#define FILTER_PULSE_CODE  (123)

static bool shutdown = false;
static screen_context_t screen_ctx;
static screen_window_t vf_win = NULL;
static const char vf_group[] = "viewfinder_window_group";
static state_t state = STATE_STARTUP;
static camera_handle_t handle = CAMERA_HANDLE_INVALID;
static bool shouldmirror = false;
static bool touch = false;
static int video_fd = -1;
static pthread_t filter_tid;
static int filter_chid;
static int filter_coid;
static struct sigevent filter_sigev;
static bool filter_stop = false;


// the BLUR filter will just pixelate the picture where faces are detected.
// Since face-detect can be a bit jittery, the blur may appear and disappear from frame to frame
// if the camera reports that no faces are seen in some frames.
// As a workaround for this, I have implemented a 10-frame debouncer
static void apply_blur_filter(camera_buffer_t* inbuf,
                              camera_buffer_t* outbuf)
{
    // the retrieved face co-ordinates will persist until 10 frames without metadata elapse
    static int fx = -1;
    static int fy = -1;
    static int fw = -1;
    static int fh = -1;
    static int debounce = 0;

    // NOTE: there is apparently a bug in assembling viewfinder metadata packets.
    // if size of 65536 is reported, then we've hit the bug!
    // alternate workaround is not to loop, but just inspect the 1st packet
    if (inbuf->framemetasize == 65536) return;

    // we're going to scan the input metadata buffer for XMP face-detect data
    camera_metapacket_t* packet;
    for (packet = inbuf->framemeta;
            packet < inbuf->framemeta + inbuf->framemetasize;
            packet += packet->size + sizeof(camera_metapacket_t))
    {
        if ((packet->format == CAMERA_METAFORMAT_XMP) && (packet->size > 0)) {
            fprintf(stderr,"found XMP packet!\n");
            char * xml = (char *) &inbuf->framemeta->data;
            // parsing the XMP packet is fairly tedious.
            // you might want to investigate some other XML parsers.
            // Also note that there is a lot more face metadata available than I care to use.
            // Fields like pitch and roll and yaw may not be accurately set by the hardware yet either.
            fprintf(stderr, "Parsing XMP packet\n");
            try {
                SXMPMeta::Initialize();

                string actualPrefix;
                SXMPMeta::RegisterNamespace(MWG_REGIONS_NS, "mwg-rs", &actualPrefix );
                SXMPMeta::RegisterNamespace(ADOBE_AREA_NS, "stArea", &actualPrefix);
                SXMPMeta::RegisterNamespace(ADOBE_DIM_NS, "stDim", &actualPrefix);
                SXMPMeta::RegisterNamespace(RIM_NS, "rim-ns", &actualPrefix);

                //now load the XML and traverse it using libxmp
                SXMPMeta meta;
                meta.ParseFromBuffer(xml, strlen(xml));

                // get a path to the RegionList Array
                string regionListPath;
                SXMPUtils::ComposeStructFieldPath(MWG_REGIONS_NS, "Regions", MWG_REGIONS_NS, "RegionList", &regionListPath);

                // get a path to the AppliedToDimensions struct
                string appliedToDimensionsPath;
                SXMPUtils::ComposeStructFieldPath(MWG_REGIONS_NS, "Regions", MWG_REGIONS_NS, "AppliedToDimensions", &appliedToDimensionsPath);

                // get the AppliedToDimensions struct's field values
                string dimW, dimH, dimUnit;
                meta.GetStructField(MWG_REGIONS_NS, appliedToDimensionsPath.c_str(), ADOBE_DIM_NS, "w", &dimW, 0);
                meta.GetStructField(MWG_REGIONS_NS, appliedToDimensionsPath.c_str(), ADOBE_DIM_NS, "h", &dimH, 0);
                meta.GetStructField(MWG_REGIONS_NS, appliedToDimensionsPath.c_str(), ADOBE_DIM_NS, "unit", &dimUnit, 0);

                // determine how many array items there are
                // note, XMP array items are 1-based indices
                int count = meta.CountArrayItems(MWG_REGIONS_NS, regionListPath.c_str());
                // loop for each face
                for (int i = 1; i <= count; ++i) {
                    // get a path to the first item of the RegionList Array
                    string regionListFirstItemPath;
                    SXMPUtils::ComposeArrayItemPath(MWG_REGIONS_NS, regionListPath.c_str(), i, &regionListFirstItemPath);

                    // get a path to the Type field
                    string typePath;
                    SXMPUtils::ComposeStructFieldPath(MWG_REGIONS_NS, regionListFirstItemPath.c_str(), MWG_REGIONS_NS, "Type", &typePath);

                    // get the Type field's value
                    string typeValue;
                    meta.GetProperty(MWG_REGIONS_NS, typePath.c_str(), &typeValue, NULL);

                    if (strcmp(typeValue.c_str(), "Face") == 0) {
                        // get a path to the Area struct
                        string areaPath;
                        SXMPUtils::ComposeStructFieldPath(MWG_REGIONS_NS, regionListFirstItemPath.c_str(), MWG_REGIONS_NS, "Area", &areaPath);

                        // get the Area struct's field values
                        string faceX, faceY, faceW, faceH, faceUnit;
                        meta.GetStructField(MWG_REGIONS_NS, areaPath.c_str(), ADOBE_AREA_NS, "x", &faceX, 0);
                        meta.GetStructField(MWG_REGIONS_NS, areaPath.c_str(), ADOBE_AREA_NS, "y", &faceY, 0);
                        meta.GetStructField(MWG_REGIONS_NS, areaPath.c_str(), ADOBE_AREA_NS, "w", &faceW, 0);
                        meta.GetStructField(MWG_REGIONS_NS, areaPath.c_str(), ADOBE_AREA_NS, "h", &faceH, 0);
                        meta.GetStructField(MWG_REGIONS_NS, areaPath.c_str(), ADOBE_AREA_NS, "unit", &faceUnit, 0);

                        // get a path to the Extensions struct
                        string extensionsPath;
                        SXMPUtils::ComposeStructFieldPath(MWG_REGIONS_NS, regionListFirstItemPath.c_str(), MWG_REGIONS_NS, "Extensions", &extensionsPath);

                        // get the Extension struct's field values
                        string extPitch, extRoll, extYaw, extScore, extPriority;
                        meta.GetStructField(MWG_REGIONS_NS, extensionsPath.c_str(), RIM_NS, "Pitch", &extPitch, 0);
                        meta.GetStructField(MWG_REGIONS_NS, extensionsPath.c_str(), RIM_NS, "Roll", &extRoll, 0);
                        meta.GetStructField(MWG_REGIONS_NS, extensionsPath.c_str(), RIM_NS, "Yaw", &extYaw, 0);
                        meta.GetStructField(MWG_REGIONS_NS, extensionsPath.c_str(), RIM_NS, "Score", &extScore, 0);
                        meta.GetStructField(MWG_REGIONS_NS, extensionsPath.c_str(), RIM_NS, "Priority", &extPriority, 0);

#if 0
                        fprintf(stderr, "Face %d \n"
                                "x = %s, y = %s, w = %s, h = %s, unit = %s \n"
                                "pitch = %s, roll = %s, yaw = %s, score = %s, priority = %s\n"
                                "original width = %s, original height = %s, unit = %s\n",
                                i, faceX.c_str(), faceY.c_str(), faceW.c_str(), faceH.c_str(),
                                faceUnit.c_str(), extPitch.c_str(), extRoll.c_str(),
                                extYaw.c_str(), extScore.c_str(), extPriority.c_str(),
                                dimW.c_str(), dimH.c_str(), dimUnit.c_str());
#endif
                        // extract the face co-ordinates.
                        // note, we will only do one face.
                        if (strcmp(faceUnit.c_str(), "normalized") == 0) {
                            fx = (int) (atof(faceX.c_str()) * inbuf->framedesc.nv12.width);
                            fy = (int) (atof(faceY.c_str()) * inbuf->framedesc.nv12.height);
                            fw = (int) (atof(faceW.c_str()) * inbuf->framedesc.nv12.width);
                            fh = (int) (atof(faceH.c_str()) * inbuf->framedesc.nv12.height);
                        } else { // assume absolute
                            fx = atoi(faceX.c_str());
                            fy = atoi(faceY.c_str());
                            fw = atoi(faceW.c_str());
                            fh = atoi(faceH.c_str());
                        }

                        // let's expand the box a bit to be sure we have good coverage of the face
                        fw = fw * 1.3;
                        fh = fh * 1.5;
                        // translate the co-ordinates to the top-left corner instead of centre
                        fx -= (int)((float)fw / 2.0);
                        fy -= (int)((float)fh / 2.0);
                        // since a face was found on this frame, reset the debouncer and break out of the loop.
                        // we are capable of dealing with multiple faces, but a debouncer would be more
                        // complex than I care to implement right now.
                        debounce = 10;
                        break;
                    }
                }
                SXMPMeta::Terminate();
            } catch (XMP_Error & e) {
                printf("XMP ERROR: %s\n", e.GetErrMsg());
            }
        }
    }

    // debounce...
    if (debounce > 0) {
        debounce--;
        // draw the blur!
        // we're just going to pixelate the Y plane to achieve this effect
        for (int y = fy; y < fy + fh; y++) {
            if (y >= (int)inbuf->framedesc.nv12.height) {
                break;
            }
            for (int x = fx; x < fx + fw; x++) {
                if (x >= (int)inbuf->framedesc.nv12.width) {
                    break;
                }
                outbuf->framebuf[outbuf->framedesc.nv12.stride * y + x] =
                        inbuf->framebuf[inbuf->framedesc.nv12.stride * (y/50)*50 + (x/50)*50];
            }
        }
    }

}

static void* filter_thread(void* arg)
{
    int rcvid;
    struct _pulse pulse;
    camera_eventkey_t key;
    camera_buffer_t inbuf;
    camera_buffer_t outbuf;

    // hook in to the viewfinder buffer stream in read/write mode.
    // when a frame becomes available, the event will be delivered to us.
    // NOTE: we should really be checking the CAMERA_FEATURE_PREVIEWISVIDEO feature
    // to make sure we are connecting to the correct image stream, but since all
    // devices that we currently build use the same frames for the viewfinder and
    // for video recording, I am not bothering to do so for the sake of clarity.
    // If the feature is reported as not available, then we would call camera_enable_video_event()
    // instead.
    if (camera_enable_viewfinder_event(handle,
                                       CAMERA_EVENTMODE_READWRITE,
                                       &key,
                                       &filter_sigev) != CAMERA_EOK) {
        fprintf(stderr, "failed to attach viewfinder read/write event\n");
        return NULL;
    }
    // NOTE: this is NEW!
    // if we are likely to be handling any camera buffer data, we must be sure
    // to inform the camera service that we are using resources.
    camera_register_resource(handle);
    // now that we have registered our intent to use resources, we will be notified
    // if the camera service is going to be unmapping our buffers if they are needed
    // by a higher priority system process.  if we do not register in this way,
    // the buffers can be unmapped AT ANY TIME, causing us to crash if we try to
    // reference the buffer data.  We must be sure to de-register the resource when
    // we are finished.  If the CAMERA_STATUS_RESOURCENOTAVAIL notification is
    // received at any time, we must be sure to immediately release all buffers and
    // call camera_deregister_resource().  There is a time limit on doing this, so do it fast!

    while(!filter_stop) {
        rcvid = MsgReceivePulse(filter_chid, &pulse, sizeof pulse, NULL);
        if (rcvid != 0) continue;   // not a pulse
        if (pulse.code != FILTER_PULSE_CODE) continue;  // not a pulse we can handle
        if (filter_stop) break;     // stop if we're being told to stop

        // okay, at this point, we can be sure the pulse code is meant to indicate that
        // a new frame is available from the camera to process.
        // retrieve the input and output buffers for processing
        camera_get_viewfinder_buffers(handle,
                                      key,
                                      &inbuf,
                                      &outbuf);
        // do our image processing work.
        // NOTE: in read/write mode, inbuf and outbuf framebuf pointers point to the same
        // physical piece of memory, so keep this in mind when implementing a filter.  You will
        // have to work in-place with the data.  In the future we plan to provide either empty
        // or pre-copied outbufs since not all effects can efficiently be implemented in-place.
        apply_blur_filter(&inbuf, &outbuf);
        // return the buffers to the camera
        camera_return_buffer(handle,
                             &inbuf);
        camera_return_buffer(handle,
                             &outbuf);
    }

    // de-register the viewfidner read/write event
    camera_disable_event(handle, key);
    // de-register our use of resources.  if the thread were being shut down due to the
    // CAMERA_STATUS_RESOURCENOTAVAIL status being received, this call will also signal
    // the camera service that it may proceed with revoking resources.
    camera_deregister_resource(handle);
    return NULL;
}


static void
status_callback(camera_handle_t handle,
                camera_devstatus_t status,
                uint16_t extra,
                void* arg)
{
    fprintf(stderr, "status notification: %d, %d\n", status, extra);
    if (status == CAMERA_STATUS_RESOURCENOTAVAIL) {
        // if we are being warned that resources are about to be revoked, then we should
        // shut down immediately!
        // the filter thread will call camera_deregister_resource() as it terminates
        fprintf(stderr, "Resource are being revoked!\n");
        filter_stop = true;
        MsgSendPulse(filter_coid, -1, FILTER_PULSE_CODE, 0);
        pthread_join(filter_tid, NULL);
    }
}


static void
handle_screen_event(bps_event_t *event)
{
    int screen_val;

    screen_event_t screen_event = screen_event_get_event(event);
    screen_get_event_property_iv(screen_event, SCREEN_PROPERTY_TYPE, &screen_val);

    switch (screen_val) {
    case SCREEN_EVENT_MTOUCH_TOUCH:
        fprintf(stderr,"Touch event\n");
        touch = true;
        break;
    case SCREEN_EVENT_MTOUCH_MOVE:
        fprintf(stderr,"Move event\n");
        break;
    case SCREEN_EVENT_MTOUCH_RELEASE:
        fprintf(stderr,"Release event\n");
        break;
    case SCREEN_EVENT_CREATE:
        // in a more involved application, it may be wise to actually check the window name to ensure
        // that we are processing the viewfinder window here, and not some other window
        if (screen_get_event_property_pv(screen_event, SCREEN_PROPERTY_WINDOW, (void **)&vf_win) == -1) {
            perror("screen_get_event_property_pv(SCREEN_PROPERTY_WINDOW)");
        } else {
            fprintf(stderr,"viewfinder window found!\n");
            // mirror viewfinder if this is the front-facing camera
            int i = (shouldmirror?1:0);
            screen_set_window_property_iv(vf_win, SCREEN_PROPERTY_MIRROR, &i);
            // place viewfinder in front of the black application background window.
            // note that a child window's ZORDER is relative to it's parent.
            // if we wanted to draw a UI on the application window, we could place the
            // viewfinder behind it and rely on transparency.  or alternately, another
            // child window could be overlaid on top of the viewfinder.
            i = +1;
            screen_set_window_property_iv(vf_win, SCREEN_PROPERTY_ZORDER, &i);
            // make viewfinder window visible
            i = 1;
            screen_set_window_property_iv(vf_win, SCREEN_PROPERTY_VISIBLE, &i);
            screen_flush_context(screen_ctx, 0);
            // we should now have a visible viewfinder
            // other things we could do here include rotating the viewfinder window (screen rotation),
            // or adjusting the size & position of the window.
            // some properties are immutable for security reasons since the window was actually created
            // in another process.  anything related to presentation should be modifiable.
            touch = false;
            state = STATE_VIEWFINDER;
        }
        break;
    default:
        break;
    }
}


static void
handle_navigator_event(bps_event_t *event) {
    switch (bps_event_get_code(event)) {
    case NAVIGATOR_SWIPE_DOWN:
        fprintf(stderr,"Swipe down event\n");
        break;
    case NAVIGATOR_EXIT:
        fprintf(stderr,"Exit event\n");
        shutdown = true;
        break;
    default:
        break;
    }
}


static void
handle_event()
{
    int rc, domain;

    bps_event_t *event = NULL;
    rc = bps_get_event(&event, -1);
    assert(rc == BPS_SUCCESS);
    if (event) {
        domain = bps_event_get_domain(event);
        if (domain == navigator_get_domain()) {
            handle_navigator_event(event);
        } else if (domain == screen_get_domain()) {
            handle_screen_event(event);
        }
    }
}


static void
run_state_machine()
{
    camera_error_t err;
    // this simple state machine just runs us through starting a viewfinder and taking pictures
    switch(state) {
    case STATE_STARTUP:
        // waiting for viewfinder...
        break;
    case STATE_VIEWFINDER:
        // viewfinder is visible.
        // if the user touches the screen anywhere, start recording a video.
        if (touch) {
            touch = false;
            char filename[CAMERA_ROLL_NAMELEN];
            // acquire a filename on the camera roll (you could also save to your sandbox or elsewhere)
            err = camera_roll_open_video(handle,
                                         &video_fd,
                                         filename,
                                         sizeof(filename),
                                         CAMERA_ROLL_VIDEO_FMT_DEFAULT);
            if (err != CAMERA_EOK) {
                fprintf(stderr, "camera_roll_open_video() failed: %d", err);
                break;
            }
            // NOTE: after a successful call to camera_roll_open_video() we now have an open file descriptor
            // that we need to remember about.  When recording is finished, we must close the file.
            // we must also delete this file if we end up failing to start a recording, otherwise we will end up
            // with an empty 0-length file hanging around.

            // AN INTERESTING NOTE ABOUT VIDEO-START TONES:
            // if you play the sound after starting a recording, it may actually be recorded
            // as part of the video file.  There are a couple of workarounds I can think of:
            // 1. Play the sound before calling camera_start_video().  If the start call fails,
            //    you will probably want to immediately play the "system_recording_stop" sound.
            //    This is how the we do it in this sample.
            // 2. Mute the microphone before starting a video recording and then unmute after
            //    the sound has played.
            soundplayer_play_sound("event_recording_start");

            err = camera_start_video(handle,
                                     filename,
                                     NULL,
                                     NULL,
                                     (void*)filename);   // user-defined arg passed to any callbacks
            if (err != CAMERA_EOK) {
                fprintf(stderr, "camera_start_video() failed: %d", err);
                // delete file and give up.
                close(video_fd);
                unlink(filename);
                soundplayer_play_sound("event_recording_stop");
                break;
            }
            // video is now recording.
            state = STATE_RECORDING;
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
        }
        break;
    case STATE_RECORDING:
        if (touch) {
            touch = false;
            // stop recording
            camera_stop_video(handle);
            close(video_fd);
            soundplayer_play_sound("event_recording_stop");
            // recording has stopped, go back to viewfinder state
            state = STATE_VIEWFINDER;
        }
        break;
    default:
        break;
    }
}


// NOTE, in this code I am purposely ignoring error return codes for the sake of clarity while
// walking through the code.  Normally, you should check the return codes for errors, as this
// will greatly help to isolate bugs.
static int
init_camera()
{
    camera_error_t err;
    unsigned int num;
    unsigned int i;
    camera_unit_t cams[CAMERA_UNIT_NUM_UNITS];
    camera_unit_t unit;
    camera_res_t *res;

    // I'm just explicitly going to pick a camera unit to open
    unit = CAMERA_UNIT_FRONT;

    fprintf(stderr, "selecting camera unit %d\n", unit);
    err = camera_open(unit,
                      CAMERA_MODE_RW | CAMERA_MODE_ROLL,
                      &handle);
    if (err != CAMERA_EOK) {
        fprintf(stderr, "camera_open() failed: %d\n", err);
        return err;
    }

    // going to run a query here to find the closest resolution to 720p
    // (error checking is omitted for clarity)
    camera_get_video_vf_resolutions(handle, 0, &num, NULL);
    res = new camera_res_t[num];
    camera_get_video_vf_resolutions(handle, num, &num, res);
    unsigned int best = 0;
    for (i=0; i<num; i++) {
        fprintf(stderr, "Supported resolution: %d x %d\n", res[i].width, res[i].height);
        if (res[i].width > res[i].height) {
            if ((abs(res[i].height - 720) < abs(res[best].height - 720)) &&
                (abs(res[i].width - 1280) < abs(res[best].width - 1280))) {
                best = i;
            }
        } else {
            if ((abs(res[i].width - 720) < abs(res[best].width - 720)) &&
                (abs(res[i].height - 1280) < abs(res[best].height - 1280))) {
                best = i;
            }
        }
    }

    fprintf(stderr, "Selecting resolution %d x %d\n", res[best].width, res[best].height);
    err = camera_set_videovf_property(handle,
                                      CAMERA_IMGPROP_WIDTH, res[best].width,
                                      CAMERA_IMGPROP_HEIGHT, res[best].height,
                                      CAMERA_IMGPROP_WIN_GROUPID, vf_group,
                                      CAMERA_IMGPROP_WIN_ID, "my_viewfinder");
    delete[] res;
    if (err != CAMERA_EOK) {
        // NOTE: if you need to narrow down which setting is causing an error,
        // consider breaking the above command down into multiple calls.
        // be aware that some values must be changed together though (eg. height & width)
        // in order to pass range-checking
        fprintf(stderr, "camera_set_videovf_property() failed: %d\n", err);
    } else {

        // callbacks are optional, however status callback is useful for detecting asynchronous events
        // unless your application requires processing of viewfinder frame data, don't bother with
        // a viewfinder callback, as it incurs some additional ipc overhead.  remember, the viewfinder
        // window itself is already rendered by the camera service, not your app.
        err = camera_start_video_viewfinder(handle,
                                            NULL, //&viewfinder_callback,
                                            &status_callback,
                                            (void*)123);   // arbitrary user argument
        if (err != CAMERA_EOK) {
            fprintf(stderr, "camera_start_video_viewfinder() failed: %d\n", err);
        } else {
            // successfully started viewfinder
            // if it's a front-facing camera, we should mirror the viewfinder once
            // we receive it.
            if (unit == CAMERA_UNIT_FRONT) {
                shouldmirror = true;
            }
            // enable face-detection metadata in the preview stream if the camera is capable
            if (camera_can_feature(handle, CAMERA_FEATURE_VFFACEDETECT)) {
                camera_config_vf_face_detect(handle, true);
            }

            // now we're going to do something new.  we're going to spawn a thread which will
            // implement our face-blurring filter.
            // first though, let's set up the channel/connection/sigevent stuff we're going to
            // be using to communicate with the camera and with the main thread (to tell it to stop)
            filter_chid = ChannelCreate(0);
            filter_coid = ConnectAttach(0, 0, filter_chid, _NTO_SIDE_CHANNEL, 0);
            SIGEV_PULSE_INIT(&filter_sigev,
                             filter_coid,
                             SIGEV_PULSE_PRIO_INHERIT,
                             FILTER_PULSE_CODE,
                             0);
            pthread_create(&filter_tid, NULL, filter_thread, NULL);

            return 0;
        }
    }
    // clean up on error
    camera_close(handle);
    handle = CAMERA_HANDLE_INVALID;
    return err;
}


int
main(int argc, char **argv)
{
    const int usage = SCREEN_USAGE_NATIVE;

    screen_window_t screen_win;
    screen_buffer_t screen_buf = NULL;
    int rect[4] = { 0, 0, 0, 0 };

    // create an application window which will just act as a background
    screen_create_context(&screen_ctx, 0);
    screen_create_window(&screen_win, screen_ctx);
    screen_create_window_group(screen_win, vf_group);
    screen_set_window_property_iv(screen_win, SCREEN_PROPERTY_USAGE, &usage);
    screen_create_window_buffers(screen_win, 1);
    screen_get_window_property_pv(screen_win, SCREEN_PROPERTY_RENDER_BUFFERS, (void **)&screen_buf);
    screen_get_window_property_iv(screen_win, SCREEN_PROPERTY_BUFFER_SIZE, rect+2);

    // fill the window with black
    int attribs[] = { SCREEN_BLIT_COLOR, 0x00000000, SCREEN_BLIT_END };
    screen_fill(screen_ctx, screen_buf, attribs);
    screen_post_window(screen_win, screen_buf, 1, rect, 0);
    // position the window at an arbitrary z-order
    int i = APP_ZORDER;
    screen_set_window_property_iv(screen_win, SCREEN_PROPERTY_ZORDER, &i);

    // Signal bps library that navigator and screen events will be requested
    bps_initialize();
    screen_request_events(screen_ctx);
    navigator_request_events(0);

    // open camera and configure viewfinder
    if (init_camera() == EOK) {
        // our main loop just runs a state machine and handles input
        while (!shutdown) {
            run_state_machine();
            // Handle user input
            handle_event();
        }

        if (state == STATE_RECORDING) {
            // stop recording
            camera_stop_video(handle);
            close(video_fd);
            state = STATE_VIEWFINDER;
        }
        if (state == STATE_VIEWFINDER) {
            // clean up camera
            // stop the filter thread - unless it was already stopped by a resource revocation
            if (filter_stop == false) {
                filter_stop = true;
                MsgSendPulse(filter_coid, -1, FILTER_PULSE_CODE, 0);
                pthread_join(filter_tid, NULL);
            }
            camera_stop_video_viewfinder(handle);
            camera_close(handle);
        }
    }

    // Clean up
    screen_stop_events(screen_ctx);
    bps_shutdown();
    screen_destroy_window(screen_win);
    screen_destroy_context(screen_ctx);
    return 0;
}

