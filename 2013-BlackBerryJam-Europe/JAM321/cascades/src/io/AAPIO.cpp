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
#include "AAPIO.hpp"

#include <unistd.h>
#include <fcntl.h>

#include <sys/select.h>

#include <QFile>
#include <QMutexLocker>

AAPIO::AAPIO(Scope * sc, BlueToothManager * bt, QObject * parent) :
		QObject(parent), GateFactory("AAPIO"), scope(sc), connected(false), select(NULL), blueToothManager(bt) {
	Gate g(this, "AAPIO()");

	timer = new QTimer(this);

	connect(timer, SIGNAL(timeout()), this, SLOT(checkForFileTimeout()));

	timer->setInterval(2000);
	timer->setSingleShot(false);
	timer->start();

}

AAPIO::~AAPIO() {
	Gate g(this, "~AAPIO()");

	AAPIOSelect * s = NULL;

	threadMutex.lock();
	s = select;
	threadMutex.unlock();

	if (s!=NULL) {
		g.log() << "Going to wait...";
		s->close();
		s->wait(200);
	}
}

void AAPIO::checkForFileTimeout() {
	Gate g(this, "checkForFileTimeout()");
	QFile file("/dev/aap0");
	bool exists = file.exists();
	g.log() << "Exists" << exists;
	setConnected(exists);
}

bool AAPIO::isConnected() {
	return connected;
}

void AAPIO::open() {
	Gate g(this, "open()");
	QMutexLocker theadLock(&threadMutex);
	if (select != NULL) {
		select->close();
		select = NULL;
	}
	select = new AAPIOSelect(this, scope, blueToothManager);
	select->start();
}

void AAPIO::setConnected(bool c) {
	Gate g(this, "setConnected()");
	g.log() << c;
	if (c != connected) {
		g.log() << "About to emit";
		connected = c;
		emit connection(connected);
		if (connected) {
			open();
		}
	}
}

AAPIOSelect::AAPIOSelect(AAPIO * own, Scope * sc, BlueToothManager * bt) :
		QThread(own), GateFactory("AAPIOSelect"), owner(own), scope(sc), blueToothManager(bt) {
	Gate g(this, "AAPIOSelect()");
}

AAPIOSelect::~AAPIOSelect() {
	Gate g(this, "~AAPIOSelect()");
}

void AAPIOSelect::run() {
	Gate g(this, "run()");
	fd = open("/dev/aap0", O_RDONLY);
	g.log() << "Opened" << fd;
	if (fd != -1) {
		int lastAngle = 0;
		int lastRange = 0;

		long long lastSend = 0;

		QByteArray pendingMessage;

		g.log() << "Opened";
//		fd_set set;

		bool finished = false;

		do {
			char temp[200];
			ssize_t bytesRead = read(fd, temp, 200);
			if (bytesRead > 0) {
				if (blueToothManager!=NULL) {
					blueToothManager->writeData(temp,0,bytesRead);
				}
				pendingMessage.append(temp, bytesRead);
				while (pendingMessage.size() >= 6) {
					if (pendingMessage[0] != (char)0xff
							|| pendingMessage[1] != (char)0xff) {
						pendingMessage.remove(0, 1);
					} else {
						int angle = pendingMessage[2] & 0xff;
						int range = (pendingMessage[3] & 0xff) << 8
								| (pendingMessage[4] & 0xff);
						long long now = this->now();
						long long delta = now - lastSend;
						if (lastAngle == angle && lastRange == range
								&& delta < 500) {
						} else {
							if (scope != NULL) {
								scope->recordHit(angle, range);
							} else {
								g.log() << QString("%1 %2").arg(angle).arg(range);
							}
							lastSend = now;
						}
						pendingMessage.remove(0, 6);

					}
				}
			} else if (bytesRead < 0) {
				finished = true;
			} else {
				g.log() << "read 0 - may want to tidy that!";
			}
		} while (!finished);

		::close(fd);
		fd = -1;
	}
	owner->threadFinished(this);
}

void AAPIOSelect::close() {
	Gate g(this, "close()");
	if (fd != -1) {
		g.log() << "closing" << fd;
		::close(fd);
	}
}

void AAPIO::threadFinished(AAPIOSelect * thread) {
	Gate g(this, "threadFinished()");
	QMutexLocker threadLocker(&threadMutex);
	if (thread == select) {
		select = NULL;
	}
	thread->deleteLater();
}

long long AAPIOSelect::now() {
	struct timeval tv;
	gettimeofday(&tv, NULL);

	long long now = (tv.tv_sec) * 1000 + (tv.tv_usec) / 1000;
	return now;
}
