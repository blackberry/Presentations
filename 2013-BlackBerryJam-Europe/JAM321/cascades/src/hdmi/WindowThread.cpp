/*
 * Copyright (c) 2013 Research In Motion.
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
#include "WindowThread.hpp"

#include <errno.h>

#include <QAbstractEventDispatcher>

#include <ScreenFunctions.hpp>

#include <unistd.h>

WindowThread::WindowThread(ScreenFunctions * sf, const WindowInfo & inf) :
		QThread(sf), GateFactory("WindowThread"), owner(sf), info(inf), isShutdown(
				false), scope(sf->scope()), screenContext(0), display(0), window(
				0), egl_disp(EGL_NO_DISPLAY ), egl_conf(0), egl_ctx(
				EGL_NO_CONTEXT ), egl_surf(EGL_NO_SURFACE ) {
	Gate g(this, "WindowThread");
	g.log() << info.group() << info.id();
}

WindowThread::~WindowThread() {
	Gate g(this, "~WindowThread()");
}

void WindowThread::run() {
	Gate g(this, "run()");
	exec();
	owner->recordThreadExit(this);
}

void WindowThread::exec() {
	Gate g(this, "exec()");
	if (initScreen()) {
		do {
			if (!isShutdown) {
				render();
				usleep(15000);
			}
		} while (!isShutdown);
	}

	tidyUp();
}

void WindowThread::tidyUp() {
	Gate g(this, "tidyUp()");

	if (egl_disp != EGL_NO_DISPLAY ) {
		g.log() << "Make current nothing"
				<< eglMakeCurrent(egl_disp, EGL_NO_SURFACE, EGL_NO_SURFACE,
						EGL_NO_CONTEXT );

		if (egl_ctx != EGL_NO_CONTEXT ) {
			g.log() << "Destroy context"
					<< eglDestroyContext(egl_disp, egl_ctx);
			egl_ctx = EGL_NO_CONTEXT;
		}

		if (egl_surf != EGL_NO_SURFACE ) {
			g.log() << "Destroy surface"
					<< eglDestroySurface(egl_disp, egl_surf);
			egl_surf = EGL_NO_SURFACE;
		}

		g.log() << "Terminate display" << eglTerminate(egl_disp);
		egl_disp = EGL_NO_DISPLAY;
	}

	if (window != NULL) {
		if (!info.group().isNull()) {
			g.log() << "Leave window group"
					<< screen_leave_window_group(window);
		}
		g.log() << "Destroy window" << screen_destroy_window(window);
		window = NULL;
	}

	if (screenContext != NULL) {
		g.log() << "Destroy screen context"
				<< screen_destroy_context(screenContext);
		screenContext = NULL;
	}
}

void WindowThread::render() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	if (scope != NULL) {
		scope->draw();
	}
	int rc = eglSwapBuffers(egl_disp, egl_surf);
	if (rc != EGL_TRUE) {
		Gate g("render()");
		g.log() << "swap failed";
	}

	pos++;
	if (pos % 1000 == 0) {
		Gate g("render");
	}
}

void WindowThread::stopWindow() {
	Gate g(this, "stopWindow()");
	isShutdown = true;
}

bool WindowThread::initScreen() {
	Gate g(this, "initScreen()");
	int rc = 0;

	try {

		rc = screen_create_context(&screenContext, SCREEN_APPLICATION_CONTEXT);
		if (rc) {
			throw "create";
		}

		if (!info.group().isNull()) {
			g.log() << "Embedded";

			QByteArray groupArr(info.group().toAscii());
			QByteArray idArr(info.id().toAscii());

			rc = screen_create_window_type(&window, screenContext,
					SCREEN_CHILD_WINDOW);
			if (rc) {
				throw "create window";
			}

			rc = screen_join_window_group(window, groupArr.constData());
			if (rc) {
				throw "join group";
			}

			rc = screen_set_window_property_cv(window,
					SCREEN_PROPERTY_ID_STRING, idArr.length(),
					idArr.constData());
			if (rc) {
				throw "set window id";
			}

		} else {
			g.log() << "hdmi";

			screen_display_t displays[info.count()];
			rc = screen_get_context_property_pv(screenContext,
					SCREEN_PROPERTY_DISPLAYS, (void**) displays);
			if (rc) {
				throw "get displays";
			}

			display = displays[info.which()];
			for (int i = 0; i < 4; ++i) {
				rect[i] = 0;
			}

			rc = screen_create_window(&window, screenContext);
			if (rc) {
				throw "create window";
			}

			rc = screen_set_window_property_pv(window, SCREEN_PROPERTY_DISPLAY,
					(void**) &display);
			if (rc) {
				throw "set window display";
			}

			rc = screen_get_display_property_iv(display, SCREEN_PROPERTY_SIZE,
					rect + 2);
			if (rc) {
				throw "get display size";
			}

			g.log() << "size" << rect[0] << rect[1] << rect[2] << rect[3];

			rc = screen_set_window_property_iv(window,
					SCREEN_PROPERTY_BUFFER_SIZE, rect + 2);
			if (rc) {
				throw "set window size";
			}

		}

		if (!initGL()) {
			throw "init GL";
		}
		return true;
	} catch (const char * where) {
		g.log() << "Failed initialization" << where;
		return false;
	}
}

bool WindowThread::initGL() {
	Gate g(this, "initGL()");

	try {

		int nBuffers = 2;

		int usage;
		int format = SCREEN_FORMAT_RGBX8888;
		EGLint interval = 0;
		int rc, num_configs;

		EGLint attrib_list[] = { EGL_RED_SIZE, 8, EGL_GREEN_SIZE, 8,
				EGL_BLUE_SIZE, 8, EGL_SURFACE_TYPE, EGL_WINDOW_BIT,
				EGL_RENDERABLE_TYPE, 0, EGL_NONE };

		usage = SCREEN_USAGE_OPENGL_ES1 | SCREEN_USAGE_ROTATION;
		attrib_list[9] = EGL_OPENGL_ES_BIT;

		egl_disp = eglGetDisplay(EGL_DEFAULT_DISPLAY );
		if (egl_disp == EGL_NO_DISPLAY ) {
			throw "Could not get display";
		}

		rc = eglInitialize(egl_disp, NULL, NULL);
		if (rc != EGL_TRUE) {
			throw "Could not initialize";
		}

		rc = eglBindAPI(EGL_OPENGL_ES_API);
		if (rc != EGL_TRUE) {
			throw "Could not bind API";
		}

		rc = eglChooseConfig(egl_disp, attrib_list, NULL, 5, &num_configs);
		if (rc != EGL_TRUE) {
			throw "Could not count configs";
		}

		if (num_configs == 0) {
			throw "No configs";
		}

		g.log() << "Number of configs" << num_configs;

		egl_conf = new EGLConfig[num_configs];

		rc = eglChooseConfig(egl_disp, attrib_list, egl_conf, num_configs,
				&num_configs);
		if (rc != EGL_TRUE) {
			throw "Could not choose config";
		}

		egl_ctx = eglCreateContext(egl_disp, egl_conf[0], EGL_NO_CONTEXT, NULL);

		if (egl_ctx == EGL_NO_CONTEXT ) {
			g.log() << "Could not create context";
			return false;
		}

		rc = screen_set_window_property_iv(window, SCREEN_PROPERTY_FORMAT,
				&format);
		if (rc) {
			throw "Could not set screen format property";
		}

		rc = screen_set_window_property_iv(window, SCREEN_PROPERTY_USAGE,
				&usage);
		if (rc) {
			throw "Could not set window usage property";
		}

		int screen_resolution[2];

		if (!info.group().isNull()) {
			g.log() << info.group() << info.id() << info.x() << info.y()
					<< info.width() << info.height();
			int rect[4] = { info.x(), info.y(), info.width(), info.height() };

			rc -= screen_set_window_property_iv(window,
					SCREEN_PROPERTY_BUFFER_SIZE, rect + 2);
			if (rc) {
				throw "Could not set window buffer size";
			}

			rc = screen_set_window_property_iv(window,
					SCREEN_PROPERTY_SOURCE_SIZE, rect + 2);
			if (rc) {
				throw "could not set window property source size";
			}

			int z = -5;
			rc = screen_set_window_property_iv(window, SCREEN_PROPERTY_ZORDER,
					&z);
			if (rc) {
				throw "Could not set window property z order";
			}

			rc = screen_set_window_property_iv(window, SCREEN_PROPERTY_POSITION,
					rect);
			if (rc) {
				throw "Could not set window property position";
			}

		} else {
			rc = screen_get_display_property_iv(display, SCREEN_PROPERTY_SIZE,
					screen_resolution);
			if (rc) {
				throw "Could not get screen res";
			}
		}

		rc = screen_create_window_buffers(window, nBuffers);
		if (rc) {
			throw "Could not create buffers";
		}

		egl_surf = eglCreateWindowSurface(egl_disp, egl_conf[0], window, NULL);
		if (egl_surf == EGL_NO_SURFACE ) {
			throw "Could not create EGL surface";
		}

		rc = eglMakeCurrent(egl_disp, egl_surf, egl_surf, egl_ctx);
		if (rc != EGL_TRUE) {
			throw "Could not make display current";
		}

		rc = eglSwapInterval(egl_disp, interval);
		if (rc != EGL_TRUE) {
			throw "Could not set swap interval";
		}

		//Query width and height of the window surface created by utility code
		EGLint surface_width, surface_height;

		rc = eglQuerySurface(egl_disp, egl_surf, EGL_WIDTH, &surface_width);
		if (rc != EGL_TRUE) {
			throw "Could not get surface width";
		}
		rc = eglQuerySurface(egl_disp, egl_surf, EGL_HEIGHT, &surface_height);
		if (rc != EGL_TRUE) {
			throw "Could not get surface height";
		}

		g.log() << "Surface dimensions" << surface_width << surface_height;


		glShadeModel(GL_SMOOTH);

		//set clear color to white
		glClearColor(0, 0, 0, 1);

		glViewport(0, 0, surface_width, surface_height);
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();

		glOrthof(0.0f, (float) (surface_width) / (float) (surface_height), 0.0f,
				1.0f, -1.0f, 1.0f);

		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();

		glTranslatef((float) (surface_width) / (float) (surface_height) / 2,
				0.5f, 0.0f);

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		g.log() << "SUCCESS";

		return true;
	} catch (const char * where) {
		g.log() << where;
		return false;
	}
}

