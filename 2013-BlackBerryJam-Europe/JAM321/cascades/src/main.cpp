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
#include <bb/cascades/Application>
#include <bb/cascades/QmlDocument>
#include <bb/cascades/AbstractPane>
#include <bb/cascades/Window>

#include <QLocale>
#include <QTranslator>
#include <QDeclarativeEngine>
#include <QDeclarativeContext>
#include <Qt/qdeclarativedebug.h>
#include "JAM321.hpp"

#include "io/AAPIO.hpp"

#include "hdmi/ScreenFunctions.hpp"
#include "hdmi/WindowThread.hpp"

#include "bt/BlueToothManager.hpp"

#include "log/GateFactory.hpp"

using namespace bb::cascades;

void msgHandler(QtMsgType type, const char * c) {
	fprintf(stderr,"%s\n",c);
}

Q_DECL_EXPORT int main(int argc, char **argv) {
	int toReturn;
	{
		Gate g("main");

		qInstallMsgHandler(msgHandler);

		Application app(argc, argv);

		Scope scope;

		ScreenFunctions screen(&scope);

		BlueToothManager btm(&scope);

		AAPIO aapio(&scope, &btm);

		QmlDocument::defaultDeclarativeEngine()->rootContext()->setContextProperty(
				"screen", &screen);
		QmlDocument::defaultDeclarativeEngine()->rootContext()->setContextProperty(
				"aapio", &aapio);
		QmlDocument::defaultDeclarativeEngine()->rootContext()->setContextProperty(
				"bluetoothManager", &btm);

		app.mainWindow()->setScreenIdleMode(ScreenIdleMode::KeepAwake);

		// localization support
		QTranslator translator;
		QString locale_string = QLocale().name();
		QString filename = QString("JAM321_%1").arg(locale_string);
		if (translator.load(filename, "app/native/qm")) {
			app.installTranslator(&translator);
		}

		new JAM321(&app, &scope, &btm);

		toReturn = Application::exec();

		g.log() << "main wants to return" << toReturn;

	}

	GateFactory::summary();

	return toReturn;
}
