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
#include <fcntl.h>
#include <screen/screen.h>
#include <camera/camera_api.h>
#include <bps/soundplayer.h>


// an arbitrary ZORDER for our application window.
#define APP_ZORDER (100)

typedef enum {
    STATE_STARTUP = 0,
    STATE_VIEWFINDER,
} state_t;


static bool shutdown = false;
static screen_context_t screen_ctx;
static screen_window_t vf_win = NULL;
static const char vf_group[] = "viewfinder_window_group";
static state_t state = STATE_STARTUP;
static camera_handle_t handle = CAMERA_HANDLE_INVALID;
static bool shouldmirror = false;
static bool touch = false;


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
viewfinder_callback(camera_handle_t handle,
                    camera_buffer_t* buf,
                    void* arg)
{
    if (buf->frametype == CAMERA_FRAMETYPE_NV12) {
        fprintf(stderr, "viewfinder frame %d x %d\n",
                buf->framedesc.nv12.width,
                buf->framedesc.nv12.height);
    } else {
        fprintf(stderr, "that's odd - not NV12\n");
    }
}


static void
status_callback(camera_handle_t handle,
                camera_devstatus_t status,
                uint16_t extra,
                void* arg)
{
    fprintf(stderr, "status notification: %d, %d\n", status, extra);
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
        // if the user touches the screen anywhere, stop the viewfinder.
        if (touch) {
            touch = false;
            shutdown = true;
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
    // here are 2 ways to determine which cameras are available on a given device...
#if 1
    // METHOD 1
    // inventory the available camera units
    // NOTE: to just find the number of available cameras: camera_get_cameras(0, &num, NULL);

    camera_get_supported_cameras(CAMERA_UNIT_NUM_UNITS,
                                 &num,
                                 cams);
    for (i=0; i<num; i++) {
        fprintf(stderr, "found camera unit %d\n", cams[i]);
    }
#else
    // METHOD 2
    // inventory cameras which support a given feature set - in this case PHOTO & VIDEO

    camera_feature_t features[] = { CAMERA_FEATURE_PHOTO, CAMERA_FEATURE_VIDEO };
    camera_unit_t next = CAMERA_UNIT_NONE;
    num = 0;
        // note that this is an iterating function call which returns only one "next" unit at a time
	while (camera_find_capable(features,
							   sizeof(features)/sizeof(*features),
							   next,
							   &next) == CAMERA_EOK) {
        cams[num++] = next;
        fprintf(stderr, "camera unit %d supports the required features\n", next);
    }
#endif

    // open the first camera found
    unit = cams[0];
    fprintf(stderr, "selecting camera unit %d\n", unit);
    err = camera_open(unit,
                      CAMERA_MODE_RW | CAMERA_MODE_ROLL,
                      &handle);
    if (err != CAMERA_EOK) {
        fprintf(stderr, "camera_open() failed: %d\n", err);
        return err;
    }

    // This is the minimal required configuration for a viewfinder.
    err = camera_set_photovf_property(handle,
                                      CAMERA_IMGPROP_WIN_GROUPID, vf_group,
                                      CAMERA_IMGPROP_WIN_ID, "my_viewfinder");
#if 0
    // here is a more complex example configuration:
    err = camera_set_photovf_property(handle,
                                      CAMERA_IMGPROP_WIN_GROUPID, vfWndGroupId,
                                      CAMERA_IMGPROP_WIN_ID, vfWndWindowId,
                                      CAMERA_IMGPROP_WIDTH, w,
                                      CAMERA_IMGPROP_HEIGHT, h,
                                      CAMERA_IMGPROP_HWOVERLAY, 1,
#ifdef DO_PHOTO_180
                                      CAMERA_IMGPROP_ROTATION, (rotation+180) % 360,
#else
                                      CAMERA_IMGPROP_ROTATION, rotation,
#endif
#ifdef DO_BURST
                                      CAMERA_IMGPROP_BURSTMODE, 1,
#endif
                                      CAMERA_IMGPROP_FRAMERATE, 15.0);
#endif
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
        // here is an example configuration for photo properties. (just updating rotation)
        camera_set_photo_property(handle,
                                  CAMERA_IMGPROP_ROTATION, 180);

        // callbacks are optional, however status callback is useful for detecting asynchronous events
        // unless your application requires processing of viewfinder frame data, don't bother with
        // a viewfinder callback, as it incurs some additional ipc overhead.  remember, the viewfinder
        // window itself is already rendered by the camera service, not your app.
        err = camera_start_photo_viewfinder(handle,
                                            &viewfinder_callback,
                                            &status_callback,
                                            (void*)123);   // arbitrary user argument
        if (err != CAMERA_EOK) {
            fprintf(stderr, "camera_start_photo_viewfinder() failed: %d\n", err);
        } else {
            // successfully started viewfinder
            // if it's a front-facing camera, we should mirror the viewfinder once
            // we receive it.
            if (unit == CAMERA_UNIT_FRONT) {
                shouldmirror = true;
            }
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

