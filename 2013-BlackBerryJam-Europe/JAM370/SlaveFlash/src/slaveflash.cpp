/*
* Copyright (c) 2013 Research In Motion Limited.
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
#include <fcntl.h>
#include <screen/screen.h>
#include <camera/camera_api.h>
#include <bps/soundplayer.h>
#include <string.h>
#include <stdlib.h>
#include <bps/navigator_invoke.h>

// an arbitrary ZORDER for our application window.
#define APP_ZORDER (100)

typedef enum {
    STATE_STARTUP = 0,
    STATE_VIEWFINDER,
    STATE_TAKINGPHOTO
} state_t;

// size of brightness transient (+ and -) before declaring that we've seen the flash fire
#define FLASH_THRESH 15

static bool shutdown = false;
static screen_context_t screen_ctx;
static screen_window_t vf_win = NULL;
static const char vf_group[] = "viewfinder_window_group";
static state_t state = STATE_STARTUP;
static camera_handle_t handle = CAMERA_HANDLE_INVALID;
static bool shouldmirror = false;
static int vfrotation = 0;
static bool touch = false;
static int photo_done_domain = -1;
static int main_bps_chid = -1;
#define MAX_BURST_FRAMES 10
static int burst_index = 0;
static int burst_brightness_index = 0;
static uint8_t* burst_bufs[MAX_BURST_FRAMES] = { 0 };
static uint64_t burst_sizes[MAX_BURST_FRAMES] = { 0 };
static uint64_t burst_timestamp[MAX_BURST_FRAMES] = { 0 };
static uint64_t flash_timestamp = 0;
static float lastbrightness = 0;
static float lastdeltabrightness = 0;
static float lasttimestamp = 0;

#define VF_WINDOW_NAME "my_viewfinder"

static void
invoke_picture_viewer(const char* path)
{
    char uri[1024];
    // create handler invocation
    navigator_invoke_invocation_t *invoke = NULL;
    navigator_invoke_invocation_create(&invoke);

    // set invocation action and type
    navigator_invoke_invocation_set_target(invoke, "sys.pictures.card.previewer");
    navigator_invoke_invocation_set_action(invoke, "bb.action.VIEW");
    navigator_invoke_invocation_set_type(invoke, "image/jpg");
    snprintf(uri, sizeof(uri), "file://%s", path);
    navigator_invoke_invocation_set_uri(invoke, uri);


    // invoke the target
    navigator_invoke_invocation_send(invoke);

    // clean up resources
    navigator_invoke_invocation_destroy(invoke);
}

static void
handle_screen_event(bps_event_t *event)
{
    int screen_val;
    screen_window_t temp_win;
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
        if (screen_get_event_property_pv(screen_event, SCREEN_PROPERTY_WINDOW, (void **)&temp_win) == -1) {
            perror("screen_get_event_property_pv(SCREEN_PROPERTY_WINDOW)");
        } else {
            char winname[1024];
            if (screen_get_window_property_cv(temp_win, SCREEN_PROPERTY_ID_STRING, sizeof(winname), winname) == -1) {
                perror("screen_get_window_property_cv(SCREEN_PROPERTY_ID_STRING)");
            } else {
                if (strcmp(winname, VF_WINDOW_NAME)) {
                    fprintf(stderr, "non-viewfinder window %s ignored\n", winname);
                } else {
                    vf_win = temp_win;
                    fprintf(stderr,"viewfinder window found!\n");
                    // place viewfinder in front of the black application background window.
                    // note that a child window's ZORDER is relative to it's parent.
                    // if we wanted to draw a UI on the application window, we could place the
                    // viewfinder behind it and rely on transparency.  or alternately, another
                    // child window could be overlaid on top of the viewfinder.
                    int i = +1;
                    screen_set_window_property_iv(vf_win, SCREEN_PROPERTY_ZORDER, &i);
                    // if the window needs to be rotated because the viewfinder does not support operating at 0-degrees,
                    // then do so here...
                    if (vfrotation) {
                        i = (360 - vfrotation) % 360;
                        screen_set_window_property_iv(vf_win, SCREEN_PROPERTY_ROTATION, &i);
                        if ((vfrotation % 180) != 0) {
                            // at 90 or 270 degrees, we need to swap the width & height of the window
                            int dims[2];
                            screen_get_window_property_iv(vf_win, SCREEN_PROPERTY_SIZE, dims);
                            int temp = dims[0];
                            dims[0] = dims[1];
                            dims[1] = temp;
                            screen_set_window_property_iv(vf_win, SCREEN_PROPERTY_SIZE, dims);
                        }
                    }
                    // mirror viewfinder if this is the front-facing camera
                    i = (shouldmirror?1:0);
                    if ((vfrotation % 180) == 0) {
                        screen_set_window_property_iv(vf_win, SCREEN_PROPERTY_MIRROR, &i);
                    } else {
                        // NOTE: if the viewfinder is going to be rotated 90 or 270 degrees, then
                        // a mirror is actually a flip, due to the order of compositing operations.
                        screen_set_window_property_iv(vf_win, SCREEN_PROPERTY_FLIP, &i);
                    }
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
            }
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
cleanup_burst()
{
    // free memory
    for (int i=0; i<MAX_BURST_FRAMES; i++) {
        if (burst_sizes[i] > 0) {
            free(burst_bufs[i]);
            burst_sizes[i] = 0;
            burst_timestamp[i] = 0;
        }
    }
    burst_index = 0;
    burst_brightness_index = 0;
}


static void
handle_photo_done_event(bps_event_t *event) {
    // re-arm the viewfinder state
    fprintf(stderr, "received photo-done event\n");
    touch = false;
    state = STATE_VIEWFINDER;
    camera_stop_burst(handle);
    camera_set_3a_lock(handle, CAMERA_3A_NONE);

    // find the frame that corresponds with the flash timestamp
    int i;
    for (i=0; i<MAX_BURST_FRAMES; i++) {
        if (burst_timestamp[i] != 0) {
            uint64_t delta;
            fprintf(stderr, "consider burst frame: %llu... ", burst_timestamp[i]);
            if (flash_timestamp > burst_timestamp[i]) {
                delta = flash_timestamp - burst_timestamp[i];
            } else {
                delta = burst_timestamp[i] - flash_timestamp;
            }
            fprintf(stderr, "delta: %llu\n", delta);
            // within expected framerate delta
            if (delta < 1000000L / 15 / 2) {
                break;
            }
        }
    }
    if (i != MAX_BURST_FRAMES) {
        int fd;
        char filename[CAMERA_ROLL_NAMELEN];
        if (camera_roll_open_photo(handle,
                                   &fd,
                                   filename,
                                   sizeof(filename),
                                   CAMERA_ROLL_PHOTO_FMT_JPG) == CAMERA_EOK) {
            fprintf(stderr, "saving burst frame %d: %s\n", i, filename);
            int index = 0;
            uint8_t* buf = burst_bufs[i];
            while(index < (int)burst_sizes[i]) {
                int rc = write(fd, &buf[index], burst_sizes[i]-index);
                if (rc > 0) {
                    index += rc;
                } else if (rc == -1) {
                    if ((errno == EAGAIN) || (errno == EINTR)) continue;
                    perror("write");
                    break;
                }
            }
            close(fd);
            // view the resulting picture
            invoke_picture_viewer(filename);
        }
    } else {
        fprintf(stderr, "failed to find jpeg corresponding to timestamp %llu :(\n", flash_timestamp);
    }
    // clean up buffers
    cleanup_burst();
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
        } else if (domain == photo_done_domain) {
            handle_photo_done_event(event);
        }
    }
}


static void
still_callback(camera_handle_t handle,
               camera_buffer_t* buf,
               void* arg)
{
    // The still callback is going to keep a small queue of full-resolution JPEGs in a circular buffer.
    // When the review/postview callback detects a flash transient, the timestamp of the flash frame will be passed
    // over to the photo-saving code, which will run through our circular buffer and find the JPEG frame with the
    // corrsponding timestamp.
    if (buf->frametype == CAMERA_FRAMETYPE_JPEG) {
        //fprintf(stderr, "still image size: %lld\n", buf->framedesc.jpeg.bufsize);
        if (burst_sizes[burst_index] > 0) {
            burst_sizes[burst_index] = 0;
            free(burst_bufs[burst_index]);
        }

        burst_bufs[burst_index] = (uint8_t*)malloc(buf->framedesc.jpeg.bufsize);
        if (!burst_bufs[burst_index]) {
            fprintf(stderr, "could not allocate jpeg buffer\n");
        } else {
            memcpy(burst_bufs[burst_index], buf->framebuf, buf->framedesc.jpeg.bufsize);
            burst_sizes[burst_index] = buf->framedesc.jpeg.bufsize;
            burst_timestamp[burst_index] = buf->frametimestamp;
            burst_index = (burst_index + 1) % MAX_BURST_FRAMES;
        }
    }
}


static void
postview_callback(camera_handle_t handle,
                  camera_buffer_t *buf,
                  void* arg)
{
    // NOTE: what we're doing here is looking at the reduced-resolution review frames which correspond with
    // the full-sized burst frames.  The timestamps between the two should match very closely, so we don't have to
    // bother decompressing a JPEG to determine the histogram or brightness of the frame.
    // The algorithm here is computing the brightness of each frame, and the difference in brightness of each frame.
    // If the difference in brightness across 3 successive frames rises more than a predetermined amount and then falls by
    // more than the same amount, then we declare that we have seen a flash and signal the photo-saving code using a
    // bps event.
    int histogram[256] = { 0 };
    if (buf->frametype == CAMERA_FRAMETYPE_NV12) {
        // let's do a 4x4 subsampled histogram to speed things up.
        // NOTE: in hindsight, we don't really need a histogram to compute the overall scene brightness, but
        // originally, the plan was to watch for the histogram to shift suddenly instead of monitor for brightness.
        for (unsigned int y=0; y<buf->framedesc.nv12.height; y+=4) {
            uint32_t offset = y*buf->framedesc.nv12.stride;
            for (unsigned int x=0; x<buf->framedesc.nv12.width; x+=4, offset+=4) {
                uint8_t val = buf->framebuf[offset];
                histogram[val]++;
            }
        }
        // sample count
        int count = buf->framedesc.nv12.height * buf->framedesc.nv12.width / (4*4);
        // determine overall brightness as an average.
        // NOTE: since we took a histogram, we could scope this only to a certain band (eg. bins > 128). feel free to
        // experiment with different trigger conditions for your own amusement.
        float brightness = 0;
        for (int i=0; i<256; i++) {
            brightness += histogram[i]*i;
        }
        brightness = brightness / count;
        // look for transient
        if (lastbrightness != 0) {
            if ((lastdeltabrightness > FLASH_THRESH) &&
                ((brightness - lastbrightness) < -FLASH_THRESH)) {
                // declare this as a transient
                fprintf(stderr, "FLASH!\n");
                flash_timestamp = lasttimestamp;
                // signal that capture is complete!
                bps_event_t* photo_done_event;
                bps_event_create(&photo_done_event, photo_done_domain, 0, NULL, NULL);
                bps_channel_push_event((int)arg, photo_done_event);
                soundplayer_play_sound("event_camera_shutter");
            }
            lastdeltabrightness = brightness - lastbrightness;
        }
        lastbrightness = brightness;
        lasttimestamp = buf->frametimestamp;
        if (lastdeltabrightness != 0) {
            fprintf(stderr, "delta: %f\n", lastdeltabrightness);
        }
    }
}


static void
status_callback(camera_handle_t handle,
                camera_devstatus_t status,
                uint16_t extra,
                void* arg)
{
    fprintf(stderr, "status notification: %d, %d\n", status, extra);
    // when all photo-taking callbacks have completed, the capture-complete event is emitted
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
        // if the user touches the screen anywhere, start the continuous burst capture running...
        if (touch) {
            touch = false;
            // zero our burst frame counter
            burst_index = 0;
            burst_brightness_index = 0;
            flash_timestamp = 0;
            lastbrightness = 0;
            lasttimestamp = 0;
            lastdeltabrightness = 0;
            err = camera_start_burst(handle,
                                     NULL,
                                     NULL,
                                     &postview_callback,
                                     &still_callback,
                                     (void*)main_bps_chid);
            // NOTE: since the auto-exposure algorithm is trying to bring the brightness of the scene up to a nominal
            // value, and since manual exposure controls were not available at the time this demonstration was given,
            // I have added a hack here to lock the exposure at it's current value.
            // What I am trying to do here is trick the AE algorithm into under-exposing on purpose.  Since the dynamic
            // range of the sensor is such that firing a full-brightness flash relatively close to my subject causes the
            // frame to saturate almost entirely to white, I am trying to compensate by lockign the AE algorithm in at a level
            // which will hopefully provide a more correctly exposed image once the flash fires.  In practice, I point the camera
            // at a bright light source (eg. a ceiling light fixture) before tapping the screen to start burst capture and
            // lock exposure.  Then when I pan the camera back to my subject, it is purposely underlit, in preparation for
            // a quick change in brightness.  In the future, doing something like applying a -3.0 EV exposure compensation offset
            // would achieve a similar result.. but that functionality is not available just yet.
            camera_set_3a_lock(handle, CAMERA_3A_AUTOEXPOSURE);
            if (err != CAMERA_EOK) {
                fprintf(stderr, "camera_start_burst() error %d\n", err);
            } else {
                state = STATE_TAKINGPHOTO;
            }
        }
        break;
    case STATE_TAKINGPHOTO:
        // if user touches anywhere, cancel
        if (touch) {
            touch = false;
            camera_stop_burst(handle);
            camera_set_3a_lock(handle, CAMERA_3A_NONE);
            fprintf(stderr, "camera_stop_burst()\n");
            cleanup_burst();
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
    // Pick the front-facing camera for this demonstration.
    // Why?
    // Because due to the nature of the rolling-shutter on the rear-sensor, if the flash is too quick (which was the case
    // with my Metz 45CT-4) it will only be illuminating the scene for a fraction of the full-frame-sampling-time of the sensor.
    // This results in a bright band across the frame rather than a fully-illuminated frame.  The solution would have been to use
    // a lower-powered flash which would have illuminated the scene for longer to achieve the same integrated brightness, but
    // I did not have time to source such a lamp.  The workaround I came up with was to use the front-facing camera which has
    // a lower resolution, and therefore a quicker full-frame-sampling-time.  Most of the time, the flash duration was longer than
    // this sampling time, which resulted in an evenly illuminated frame.
    unit = CAMERA_UNIT_FRONT;
    fprintf(stderr, "selecting camera unit %d\n", unit);
    err = camera_open(unit,
                      CAMERA_MODE_RW | CAMERA_MODE_ROLL,
                      &handle);
    if (err != CAMERA_EOK) {
        fprintf(stderr, "camera_open() failed: %d\n", err);
        return err;
    }

    // This is the minimal required configuration for a viewfinder.
    // NOTE: we need to enable burst mode when starting the viewfinder.
    // the maximum burst framerate is 15fps.  don't trust me?  you can query
    // camera_get_photo_vf_framerates() to determine the capabilities.
    err = camera_set_photovf_property(handle,
                                      CAMERA_IMGPROP_WIN_GROUPID, vf_group,
                                      CAMERA_IMGPROP_WIN_ID, VF_WINDOW_NAME,
                                      CAMERA_IMGPROP_BURSTMODE, 1,
                                      CAMERA_IMGPROP_FRAMERATE, (double)15.0);  // max for burst is 15fps.
    if (err != CAMERA_EOK) {
        // NOTE: if you need to narrow down which setting is causing an error,
        // consider breaking the above command down into multiple calls.
        // be aware that some values must be changed together though (eg. height & width)
        // in order to pass range-checking
        fprintf(stderr, "camera_set_photovf_property() failed: %d\n", err);
    } else {

        // a valid photovf and photo configuration are required before starting
        // the viewfinder.
        // the defaults for both will always be sane, however if certain properties
        // are changed in one, they may need to be changed in the other.
        // resolution is one such example (aspect ratios must match).
        // here is an example configuration for photo properties - setting up the burst divisor.
        // ensuring burst divisor is set to 1 to deliver all frames
        camera_set_photo_property(handle,
                                  CAMERA_IMGPROP_BURSTDIVISOR, (double)1.0);  // DOUBLE!

        // callbacks are optional, however status callback is useful for detecting asynchronous events
        // unless your application requires processing of viewfinder frame data, don't bother with
        // a viewfinder callback, as it incurs some additional ipc overhead.  remember, the viewfinder
        // window itself is already rendered by the camera service, not your app.
        // NOTE: we are passing main_bps_chid as the void* arg which will then
        // be delivered to all callbacks. main_bps_chid is already a global variable,
        // so this isn't necessary, but is just done here to illustrate the convention.
        err = camera_start_photo_viewfinder(handle,
                                            NULL, //&viewfinder_callback,
                                            &status_callback,
                                            (void*)main_bps_chid);   // user-defined arg.
        if (err != CAMERA_EOK) {
            fprintf(stderr, "camera_start_photo_viewfinder() failed: %d\n", err);
        } else {
            // successfully started viewfinder
            // if it's a front-facing camera, we should mirror the viewfinder once
            // we receive it.
            if (unit == CAMERA_UNIT_FRONT) {
                shouldmirror = true;
            }
            // if the viewfinder rotation is non-zero, then we need to rotate the window when it shows up.
            camera_get_photovf_property(handle, CAMERA_IMGPROP_ROTATION, &vfrotation);
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
    main_bps_chid = bps_channel_get_active();
    screen_request_events(screen_ctx);
    navigator_request_events(0);

    // create a custom BPS event that we can use in to let our main thread know
    // that photo-taking is finished
    photo_done_domain = bps_register_domain();

    // open camera and configure viewfinder
    if (init_camera() == EOK) {
        // our main loop just runs a state machine and handles input
        while (!shutdown) {
            run_state_machine();
            // Handle user input
            handle_event();
        }

        if (state == STATE_TAKINGPHOTO) {
            // wait for picture-taking to finish? TBD
            state = STATE_VIEWFINDER;
        }
        if (state == STATE_VIEWFINDER) {
            // clean up camera
            camera_stop_photo_viewfinder(handle);
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
