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
#include <stdint.h>
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
	STATE_STARTUP = 0, STATE_VIEWFINDER, STATE_RECORDING
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
static uint8_t filterU = 110;
static uint8_t filterV = 140;
static bool doSepia = true;

// This filter will set the U and V values of frame to a constant, which will
// with these values have the effect of applying a sepia filter.
static void apply_filter(camera_buffer_t* inbuf, camera_buffer_t* outbuf) {

	// NOTE: there is apparently a bug in assembling viewfinder metadata packets.
	// if size of 65536 is reported, then we've hit the bug!
	// alternate workaround is not to loop, but just inspect the 1st packet
	if (inbuf->framemetasize == 65536)
		return;

	// This code steps through the UV plane of the NV12 preview frame and sets those values to be something specific
	// This specific example turns the image into sepia, but you could do something similar to get greyscale, or any other colour
	// just by changing filterU and filterV.
	if (doSepia) {
		for (int y = 0; y < (int) inbuf->framedesc.nv12.height / 2; y++) {
			for (int x = 0; x < (int) inbuf->framedesc.nv12.width; x += 2) {
				outbuf->framebuf[(int) inbuf->framedesc.nv12.uv_offset
						+ (int) outbuf->framedesc.nv12.uv_stride * y + x] =
						filterU;
				outbuf->framebuf[(int) inbuf->framedesc.nv12.uv_offset
						+ (int) outbuf->framedesc.nv12.uv_stride * y + x + 1] =
						filterV;
			}
		}
	}

}

static void* filter_thread(void* arg) {
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
	if (camera_enable_viewfinder_event(handle, CAMERA_EVENTMODE_READWRITE, &key,
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

	while (!filter_stop) {
		rcvid = MsgReceivePulse(filter_chid, &pulse, sizeof pulse, NULL);
		if (rcvid != 0)
			continue; // not a pulse
		if (pulse.code != FILTER_PULSE_CODE)
			continue; // not a pulse we can handle
		if (filter_stop)
			break; // stop if we're being told to stop

		// okay, at this point, we can be sure the pulse code is meant to indicate that
		// a new frame is available from the camera to process.
		// retrieve the input and output buffers for processing
		camera_get_viewfinder_buffers(handle, key, &inbuf, &outbuf);
		// do our image processing work.
		// NOTE: in read/write mode, inbuf and outbuf framebuf pointers point to the same
		// physical piece of memory, so keep this in mind when implementing a filter.  You will
		// have to work in-place with the data.  In the future we plan to provide either empty
		// or pre-copied outbufs since not all effects can efficiently be implemented in-place.
		apply_filter(&inbuf, &outbuf);
		// return the buffers to the camera
		camera_return_buffer(handle, &inbuf);
		camera_return_buffer(handle, &outbuf);
	}

	// de-register the viewfidner read/write event
	camera_disable_event(handle, key);
	// de-register our use of resources.  if the thread were being shut down due to the
	// CAMERA_STATUS_RESOURCENOTAVAIL status being received, this call will also signal
	// the camera service that it may proceed with revoking resources.
	camera_deregister_resource(handle);
	return NULL;
}

static void status_callback(camera_handle_t handle, camera_devstatus_t status,
		uint16_t extra, void* arg) {
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

static void handle_screen_event(bps_event_t *event) {
	int screen_val;

	screen_event_t screen_event = screen_event_get_event(event);
	screen_get_event_property_iv(screen_event, SCREEN_PROPERTY_TYPE,
			&screen_val);

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

static void handle_navigator_event(bps_event_t *event) {
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

static void handle_event() {
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

static void run_state_machine() {
	camera_error_t err;
	// this simple state machine just runs us through starting a viewfinder and taking pictures
	switch (state) {
	case STATE_STARTUP:
		// waiting for viewfinder...
		break;
	case STATE_VIEWFINDER:
		// viewfinder is visible.
		// if the user touches the screen anywhere, start sepia filter
		if (touch) {
			touch = false;
			doSepia = !doSepia;

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
static int init_camera() {
	camera_error_t err;
	unsigned int num;
	unsigned int i;
	camera_unit_t cams[CAMERA_UNIT_NUM_UNITS];
	camera_unit_t unit;
	camera_res_t *res;

	// I'm just explicitly going to pick a camera unit to open
	unit = CAMERA_UNIT_REAR;

	fprintf(stderr, "selecting camera unit %d\n", unit);
	err = camera_open(unit, CAMERA_MODE_RW | CAMERA_MODE_ROLL, &handle);
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
	for (i = 0; i < num; i++) {
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
	err = camera_set_videovf_property(handle, CAMERA_IMGPROP_WIDTH,
			res[best].width, CAMERA_IMGPROP_HEIGHT, res[best].height,
			CAMERA_IMGPROP_WIN_GROUPID, vf_group, CAMERA_IMGPROP_WIN_ID,
			"my_viewfinder");
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
		NULL,//&viewfinder_callback,
		&status_callback,
		(void*)123);// arbitrary user argument
		if (err != CAMERA_EOK) {
			fprintf(stderr, "camera_start_video_viewfinder() failed: %d\n", err);
		} else {
			// successfully started viewfinder
			// if it's a front-facing camera, we should mirror the viewfinder once
			// we receive it.
			if (unit == CAMERA_UNIT_FRONT) {
				shouldmirror = true;
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

int main(int argc, char **argv) {
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
	screen_get_window_property_pv(screen_win, SCREEN_PROPERTY_RENDER_BUFFERS,
			(void **) &screen_buf);
	screen_get_window_property_iv(screen_win, SCREEN_PROPERTY_BUFFER_SIZE,
			rect + 2);

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

