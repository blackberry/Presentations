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
#include <QThread>

#include "Gate.hpp"
#include "GateFactory.hpp"

#include <QMutex>
#include <QMutexLocker>
#include <QMap>
#include <QDateTime>

#include <stdio.h>

class GateState {
public:
	GateState() {
		depth = 0;
		newLinePending = 0;
	}
	int depth;
	bool newLinePending;
};

QMutex gateDepthsMutex;
QMap<int,GateState*> depths;

GateState * getGateState() {
	QMutexLocker locker(&gateDepthsMutex);
	int handle = (int)QThread::currentThreadId();
	if (depths.contains(handle)) {
		return depths[handle];
	} else {
		GateState * toReturn = new GateState();
		depths[handle] = toReturn;
		return toReturn;
	}
}

Gate::Gate(GateFactory * gf, const QString message) {
	setMessage(gf,message.toAscii().constData());
}

Gate::Gate(const char * message) {
	setMessage(NULL,message);
}

Gate::Gate(GateFactory * gf, const char * message) {
	setMessage(gf,message);
}

void Gate::setMessage(GateFactory * gf, const char * mess) {
	gateState = getGateState();
	message = "";
	if (gf!=NULL) {
		message += gf->name;
		message += "::";
	}
	message += mess;
	log(">>");
	gateState->depth++;
}

Gate::~Gate() {
	gateState->depth--;
	log("<<");
}

Gate & Gate::log(QString what) {
	if (gateState->newLinePending) {
		fprintf(stderr,"\n");
		gateState->newLinePending = false;
	}

	QString line = "%1: %2 %3 %4 %5";
	line = line.arg(QDateTime::currentDateTime().toString());
	line = line.arg((int)QThread::currentThreadId(),4);
	line = line.arg(" ",gateState->depth);
	line = line.arg(message);
	line = line.arg(what);
	fprintf(stderr,"%s\n",line.toAscii().constData());
	return *this;
}

Gate & Gate::log() {
	return *this;
}

Gate & Gate::operator<<(const char * message) {
	log(message);
	return *this;
}

Gate & Gate::operator<<(const int i) {
	log(QString("%1").arg(i));
	return *this;
}

Gate & Gate::operator<<(const QString & message) {
	log(message);
	return *this;
}
