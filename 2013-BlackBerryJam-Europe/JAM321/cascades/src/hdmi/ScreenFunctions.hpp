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
#ifndef SCREENFUNCTIONS_HPP_
#define SCREENFUNCTIONS_HPP_

#include <QObject>
#include <QScopedPointer>
#include <QTimer>
#include <QList>
#include <QMutex>

#include <screen/screen.h>
#include "../log/GateFactory.hpp"

#include "../graphics/scope.hpp"

class WindowThread;

class ScreenFunctions : public QObject, public GateFactory {
	Q_OBJECT
public:
	ScreenFunctions(Scope * scope, QObject * owner = NULL);
	virtual ~ScreenFunctions();

	Q_PROPERTY (bool isHDMIAttached READ isHDMIAttached NOTIFY onHDMIAttached)
	Q_PROPERTY (QString message READ message)

	Q_PROPERTY (bool embeddedRendering READ isEmbeddedRendering NOTIFY onEmbeddedRendering)
	Q_PROPERTY (bool hdmiRendering READ isHDMIRendering NOTIFY onHDMIRendering)

	Q_INVOKABLE void createHDMIWindow();
	Q_INVOKABLE void createEmbeddedWindow();

	Q_INVOKABLE void stopRendering();

	bool isHDMIAttached();
	QString message();

	bool isEmbeddedRendering();
	bool isHDMIRendering();

	Scope * scope();

	void recordThreadExit(WindowThread * which);

signals:
	void onHDMIAttached(bool);
	void onHDMIRendering(bool);

	void onEmbeddedRendering(bool);

private:
	Q_INVOKABLE void recordThreadExitInternal();

	screen_context_t screenContext;

	bool enumerateDisplays();
	void deleteWindows();

	QList<screen_display_t> displays;
	QList<class WindowThread *> windows;
	QList<class WindowThread *> windowsToDelete;
	QTimer timer;
	QMutex windowsLocker;

	bool hdmiAttached;
	bool embeddedRendering;
	bool hdmiRendering;

	Scope * theScope;

private slots:
	void onTimeout();
};

#endif /* SCREENFUNCTIONS_HPP_ */
