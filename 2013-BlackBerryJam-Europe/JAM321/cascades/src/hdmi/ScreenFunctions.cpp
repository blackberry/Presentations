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
#include "ScreenFunctions.hpp"
#include "WindowThread.hpp"

#include "../log/Gate.hpp"

#include <bb/cascades/Application>
#include <bb/cascades/Window>

ScreenFunctions::ScreenFunctions(Scope * sc, QObject * owner) :
		QObject(owner), GateFactory("ScreenFunctions"), theScope(sc), screenContext(
				0), hdmiAttached(false) {
	Gate g(this, "ScreenFunctions");

	if (screen_create_context(&screenContext, SCREEN_APPLICATION_CONTEXT)
			!= 0) {
		g.log() << "Failed to open screen context";
	}

	connect(&timer, SIGNAL(timeout()), this, SLOT(onTimeout()));

	enumerateDisplays();

	timer.setInterval(2000);
	timer.setSingleShot(false);
	timer.start();

	onTimeout();
}

ScreenFunctions::~ScreenFunctions() {
	Gate g(this, "~ScreenFunctions()");

	stopRendering();

	if (screenContext != 0) {
		screen_destroy_context(screenContext);
	}

	WindowThread * thread = NULL;
	int count = 0;
	do {
		g.log() << "About to lock";
		windowsLocker.lock();
		g.log() << "Locked";
		thread = windows.size() > 0 ? windows[0] : NULL;
		g.log() << "About to unlock";
		windowsLocker.unlock();
		g.log() << "Unlocked";

		if (thread != NULL) {
			g.log() << "About to wait";
			thread->wait(200);
			++count;
			if (count > 100) {
				g.log() << "Breaking wait";
				break;
			}
		}
	} while (thread != NULL);

	deleteWindows();
}

void ScreenFunctions::deleteWindows() {
	Gate g(this, "deleteWindows()");
	QMutexLocker locker(&windowsLocker);
	bool discoverHDMI = false;
	bool discoverEmbedded = false;
	while (windowsToDelete.size() != 0) {
		g.log() << "Size: " << windowsToDelete.size();
		windowsToDelete[0]->wait(100);

		if (windowsToDelete[0]->info.group().isNull()) {
			discoverHDMI = true;
		} else {
			discoverEmbedded = true;
		}
		delete windowsToDelete[0];
		windowsToDelete.pop_front();
	}

	if (discoverHDMI && hdmiRendering) {
		hdmiRendering = false;
		emit onHDMIRendering(hdmiRendering);
	}
	if (discoverEmbedded && embeddedRendering) {
		embeddedRendering = false;
		emit onEmbeddedRendering(embeddedRendering);
	}
}

void ScreenFunctions::stopRendering() {
	Gate g(this, "stopRendering()");
	QMutexLocker locker(&windowsLocker);
	for (int i = 0; i < windows.size(); ++i) {
		windows[i]->stopWindow();
	}
}

bool ScreenFunctions::enumerateDisplays() {
	Gate g(this, "enumerateDisplays()");

	int count(0);

	int rc = screen_get_context_property_iv(screenContext,
			SCREEN_PROPERTY_DISPLAY_COUNT, &count);
	if (rc != 0) {
		g.log() << "Failed to get display count";
		return false;
	}

	screen_display_t tempDisplays[count];

	rc = screen_get_context_property_pv(screenContext, SCREEN_PROPERTY_DISPLAYS,
			(void**) tempDisplays);
	if (rc != 0) {
		g.log() << "Failed to get display pointers";
		return false;
	}

	for (int i = 0; i < count; ++i) {
		displays << tempDisplays[i];
	}

	g.log() << "Display count" << count;

	return true;
}

void ScreenFunctions::onTimeout() {
	Gate g(this, "onTimeout()");
	int size[4];
	for (int i = 0; i < displays.size(); ++i) {
		int attached(0);
		screen_get_display_property_iv(displays[i], SCREEN_PROPERTY_DPI, size);
		g.log() << "Size" << i << size[0] << size[1] << size[2] << size[3];

		screen_get_display_property_iv(displays[i], SCREEN_PROPERTY_ATTACHED,
				&attached);
		if (i == 1) {
			bool ba = attached ? true : false;
			if (ba != hdmiAttached) {
				hdmiAttached = attached;
				emit onHDMIAttached(hdmiAttached);
			}
		}
	}
}

bool ScreenFunctions::isHDMIAttached() {
	Gate g(this, "isHDMIAttached()");
	g.log() << hdmiAttached;
	return hdmiAttached;
}

QString ScreenFunctions::message() {
	Gate g(this, "message()");
	return "This is a message";
}

void ScreenFunctions::recordThreadExit(WindowThread * which) {
	Gate g(this, "recordThreadExit()");
	QMutexLocker locker(&windowsLocker);
	windows.removeAll(which);
	windowsToDelete << which;

	QMetaObject::invokeMethod(this, "recordThreadExitInternal",
			Qt::QueuedConnection);
}

void ScreenFunctions::recordThreadExitInternal() {
	Gate g(this, "recordThreadExitInternal()");
	deleteWindows();
}

void ScreenFunctions::createHDMIWindow() {
	Gate g(this, "createHDMIWindow()");

	WindowInfo wi(2, 1);

	QMutexLocker locker(&windowsLocker);

	if (hdmiRendering) {
		g.log() << "Already rendering...";
		return;
	}

	hdmiRendering = true;
	emit onHDMIRendering(true);

	WindowThread * t = new WindowThread(this, wi);
	windows << t;
	t->start();
}

void ScreenFunctions::createEmbeddedWindow() {
	Gate g(this, "createEmbeddedWindow()");

	QMutexLocker locker(&windowsLocker);

	if (embeddedRendering) {
		g.log() << "Already rendering..";
		return;
	}

	embeddedRendering = true;
	emit onEmbeddedRendering(true);

	QString group =
			bb::cascades::Application::instance()->mainWindow()->groupId();
	QString id("JAM321WindowAppID");

	QByteArray groupArr = group.toAscii();
	QByteArray idArr = id.toAscii();

	WindowInfo wi(group, id, 0, 0, 768, 354);

	WindowThread * w = new WindowThread(this, wi);
	windows << w;
	w->start();
}

Scope * ScreenFunctions::scope() {
	return theScope;
}

bool ScreenFunctions::isEmbeddedRendering() {
	return embeddedRendering;
}

bool ScreenFunctions::isHDMIRendering() {
	return hdmiRendering;
}
