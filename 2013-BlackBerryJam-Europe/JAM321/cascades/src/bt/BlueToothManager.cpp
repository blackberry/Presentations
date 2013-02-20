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
#include "BlueToothManager.hpp"

#define SPP_SERVICE_UUID "00001101-0000-1000-8000-00805F9B34FB"

#include <unistd.h>
#include <fcntl.h>
#include <sys/select.h>
#include <time.h>

#include <btapi/btdevice.h>
#include <btapi/btspp.h>

#include <QByteArray>

#include <errno.h>

#include "../util/NamedArrayDataModel.hpp"

#include "../graphics/scope.hpp"

#include "../util/Now.hpp"

LocalDeviceInfo * BlueToothManager::ldev = NULL;

extern "C" {
	void btCallback(const int event, const char *bt_addr, const char *event_data) {
		Gate g("BlueToothManager::btCallback()");
		g.log() << event << bt_addr << event_data;
	}

	void btSppCallback(long param, int fd) {
		Gate g("BlueToothManager::btSppCallback()");
		g.log() << param << fd;

		if (fd >= 0) {
			BlueToothManager * instance = (BlueToothManager*) param;
			instance->sppCallback(fd);
		}
	}
}

BlueToothManager::BlueToothManager(Scope * sc) :
		GateFactory("BlueToothManager"), scope(sc), started(false), thread(NULL) {
	Gate g(this, "BlueToothManager()");
	if (ldev != NULL) {
		ldev = new LocalDeviceInfo();
	}
	g.log() << "init" << bt_device_init(btCallback);
	g.log() << "spp_init" << bt_spp_init();

	connect(this,SIGNAL(sppCallbackSignal(int)),this,SLOT(sppCallbackInternal(int)));
}

BlueToothManager::~BlueToothManager() {
	Gate g(this, "~BlueToothManager()");
	{
		QMutexLocker lock(&threadLock);
		if (thread != NULL) {
			thread->close();
		}
	}
	g.log() << "spp_deinit" << bt_spp_deinit();
	bt_device_deinit();
}

void BlueToothManager::writeData(char * temp, int offset, int length) {
	if (thread != NULL) {
		thread->writeData(temp, offset, length);
	}
}

void BlueToothManager::startSPPServer() {
	Gate g(this, "startSPPServer()");
	if (started) {
		g.log() << "Already started...";
		return;
	}

	g.log() << "Open server"
			<< bt_spp_open_server("", SPP_SERVICE_UUID, 0, btSppCallback, (int)this);
}

void BlueToothManager::startSPPClient(int which) {
	Gate g(this, "startSPPClient()");

	g.log() << "Size of addresses" << addresses.size();
	g.log() << "Requested" << which;
	g.log() << "Value" << addresses[which];

	QString address = addresses[which];
	QByteArray data = address.toAscii();
	char * dataPtr = data.data();

	g.log() << "Address" << dataPtr;
	int rc = bt_spp_open(dataPtr, SPP_SERVICE_UUID, false);
	g.log() << "bt_spp_open";
	if (rc == -1) {
		g.log() << "Errno" << errno;
	} else {
		sppCallback(rc);
	}
}

bb::cascades::ArrayDataModel * BlueToothManager::getPairedDevices() {
	Gate g(this, "getPairedList()");
	addresses.clear();
	int count(0);
	char friendlyName[200];
	char address[200];
	NamedArrayDataModel * toReturn = new NamedArrayDataModel();
	bt_remote_device_t ** devices(
			bt_disc_retrieve_devices(BT_DISCOVERY_PREKNOWN, &count));
	for (int i = 0; i < count; ++i) {
		QString message("%1");
		message = message.arg(i);
		Gate g2(this, (QString) message);
		bt_rdev_get_friendly_name(devices[i], friendlyName, 200);
		bt_rdev_get_addr(devices[i], address);
		addresses << address;
		QVariantMap map;
		QString summary = "%1 (%2)";
		summary = summary.arg(address).arg(friendlyName);
		map["name"] = summary;
		g2.log() << friendlyName << address;
		toReturn->append(map);
	}
	return toReturn;
}

void BlueToothManager::sppCallback(int fd) {
	Gate g(this,"sppCallback()");
	emit sppCallbackSignal(fd);
}

void BlueToothManager::sppCallbackInternal(int fd) {
	Gate g(this,"sppCallbackInternal()");
	QMutexLocker lock(&threadLock);
	thread = new BlueToothThread(fd, scope, this);
	thread->start();
}

BlueToothThread::BlueToothThread(int f, Scope * s, BlueToothManager * own) :
		QThread(own), GateFactory("BlueToothThread"), fd(f), owner(own), scope(
				s) {
	Gate g(this, "BlueToothThread()");
}

BlueToothThread::~BlueToothThread() {
	Gate g(this, "~BlueToothThread()");
}

void BlueToothThread::close() {
	QMutexLocker lock(&fileLock);
	if (fd != -1) {
		::close(fd);
	}
	fd = -1;
}

void BlueToothThread::run() {
	Gate g(this, "run");
	char buffer[200];
	QByteArray pendingMessage;

	int lastAngle = 0;
	int lastRange = 0;
	long long lastSend = 0;

	bool good = true;
	do {
		int f = 0;
		{
			QMutexLocker lock(&fileLock);
			f = fd;
		}
		if (f != -1) {
			ssize_t size = read(f, buffer, 200);
			if (size == -1) {
				QMutexLocker lock(&fileLock);
				fd = -1;
				good = false;
				g.log() << "FD closed";
			} else {
				pendingMessage.append(buffer, size);
				while (pendingMessage.size() >= 6) {
					if (pendingMessage[0] != (char) 0xff
							|| pendingMessage[1] != (char) 0xff) {
						pendingMessage.remove(0, 1);
					} else {
						int angle = pendingMessage[2] & 0xff;
						int range = (pendingMessage[3] & 0xff) << 8
								| (pendingMessage[4] & 0xff);
						long long now = Now::now();
						long long delta = now - lastSend;
						if (lastAngle == angle && lastRange == range
								&& delta < 500) {
						} else {
							if (scope != NULL) {
								scope->recordHit(angle, range);
							} else {
								g.log()
										<< QString("%1 %2").arg(angle).arg(
												range);
							}
							lastSend = now;
						}
						pendingMessage.remove(0, 6);
					}
				}
			}
		} else {
			good = false;
		}
	} while (good);
}

void BlueToothThread::writeData(char * buffer, int offset, int length) {
	Gate g(this, "writeData()");
	QMutexLocker lock(&fileLock);
	if (fd != -1) {
		write(fd, buffer + offset, length);
	}
}

