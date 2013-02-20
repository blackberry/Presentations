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
#ifndef WINDOW_HPP_
#define WINDOW_HPP_

#include <QObject>
#include <QList>
#include <QThread>
#include <QMutex>
#include <QMutexLocker>

#include <screen/screen.h>
#include "../log/GateFactory.hpp"
#include "../graphics/scope.hpp"

#include <EGL/egl.h>
#include <GLES/gl.h>
#include <GLES/glext.h>

class WindowThread;

class ScreenFunctions;

class WindowInfo {
public:
	WindowInfo(const QString & group, const QString & id, int x, int y,
			int width, int height) {
		_group = group;
		_id = id;
		_which = _count = -1;
		_x = x;
		_y = y;
		_width = width;
		_height = height;
	}
	WindowInfo(int count, int which) {
		_count = count;
		_which = which;
	}
	WindowInfo(const WindowInfo & w) {
		_group = w.group();
		_id = w.id();
		_count = w.count();
		_which = w.which();
		_x = w.x();
		_y = w.y();
		_width = w.width();
		_height = w.height();
	}

	const QString group() const {
		return _group;
	}
	const QString id() const {
		return _id;
	}
	int count() const {
		return _count;
	}
	int which() const {
		return _which;
	}
	int width() const {
		return _width;
	}
	int height() const {
		return _height;
	}
	int x() const {
		return _x;
	}
	int y() const {
		return _y;
	}
private:
	QString _group, _id;
	int _x, _y, _width, _height;
	int _count, _which;
};

class WindowThread: public QThread, public GateFactory {
Q_OBJECT
public:
	WindowThread(ScreenFunctions * owner, const WindowInfo & info);
	virtual ~WindowThread();

	virtual void run();

	void stopWindow();

	WindowInfo info;

private:
	ScreenFunctions * owner;

	bool initScreen();
	bool initGL();

	void tidyUp();

	screen_context_t screenContext;
	screen_display_t display;
	screen_window_t window;

	EGLDisplay egl_disp;
	EGLConfig * egl_conf;
	EGLContext egl_ctx;
	EGLSurface egl_surf;

	void exec();

	virtual void render();

	int rect[4];
	int pos;

	bool isShutdown;

	Scope * scope;

};

#endif /* WINDOW_HPP_ */
