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
#ifndef BLUETOOTHMANAGER_HPP_
#define BLUETOOTHMANAGER_HPP_

#include <QObject>
#include <QVariantList>
#include <QMutex>

#include "../log/GateFactory.hpp"
#include "../log/Gate.hpp"
#include "LocalDeviceInfo.hpp"
#include "RemoteDeviceInfo.hpp"

#include <bb/cascades/databinding/arraydatamodel.h>

class BlueToothThread: public QThread, public GateFactory {
Q_OBJECT
public:
	BlueToothThread(int fd, class Scope * scope,
			class BlueToothManager * owner);
	virtual ~BlueToothThread();

	virtual void run();

	virtual void writeData(char * buffer, int offset, int length);
	virtual void close();

	long long now();

private:
	QMutex fileLock;
	class BlueToothManager * owner;
	class Scope * scope;
	int fd;
};

class BlueToothManager: public QObject, public GateFactory {
Q_OBJECT
public:
	BlueToothManager(Scope * scope);
	virtual ~BlueToothManager();

	Q_INVOKABLE void startSPPServer();

	Q_INVOKABLE	void startSPPClient(int which);

	Q_INVOKABLE	bb::cascades::ArrayDataModel * getPairedDevices();

	void writeData(char * temp, int offset, int length);

	void sppCallback(int fd);

	static LocalDeviceInfo * ldev;
	RemoteDeviceInfo rdev;
private:
	QStringList addresses;
	bool started;
	Scope * scope;
	QMutex threadLock;
	BlueToothThread * thread;

signals:
	void sppCallbackSignal(int fd);

private slots:
	void sppCallbackInternal(int fd);
};

#endif /* BLUETOOTHMANAGER_HPP_ */
