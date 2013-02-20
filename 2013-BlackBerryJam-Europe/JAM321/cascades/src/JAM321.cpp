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
#include "JAM321.hpp"

#include <bb/cascades/Application>
#include <bb/cascades/QmlDocument>
#include <bb/cascades/AbstractPane>

using namespace bb::cascades;

JAM321::JAM321(bb::cascades::Application *app, Scope * sc,
		BlueToothManager * btm) :
		QObject(app), GateFactory("JAM321"), scope(sc), blueToothManager(btm), timer(
				NULL) {
	Gate g(this, "JAM321()");
	QmlDocument *qml = QmlDocument::create("asset:///main.qml").parent(this);

	AbstractPane *root = qml->createRootObject<AbstractPane>();
	app->setScene(root);

	if (scope != NULL) {
		timer = new QTimer(this);
		timer->setInterval(500); // 50); // /30);
		timer->setSingleShot(false);
		connect(timer, SIGNAL(timeout()), this, SLOT(onRandomTimeout()));
		timer->start();
	}

}

JAM321::~JAM321() {
	timer->stop();
}

void JAM321::onRandomTimeout() {
	if (scope != NULL) {
		for (int i = 0; i < 1; ++i) {
			if (qrand() % 20 == 0) {
				int angle = qrand() % 180;
				int value = qrand() % 1024;
				scope->recordHit(angle, value);
			}
		}
	}
}

