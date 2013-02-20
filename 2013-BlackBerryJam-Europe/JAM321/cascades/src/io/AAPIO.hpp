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
#ifndef AAPIO_HPP_
#define AAPIO_HPP_

#include <QObject>
#include <QThread>
#include <QTimer>
#include <QFile>
#include <QSocketNotifier>
#include <QMutex>

#include "../graphics/scope.hpp"
#include "../log/GateFactory.hpp"
#include "../bt/BlueToothManager.hpp"

class AAPIOSelect;

class AAPIO: public QObject, public GateFactory {
Q_OBJECT
public:
	AAPIO(Scope * scope, BlueToothManager * blueToothManager, QObject * parent = NULL);
	virtual ~AAPIO();

	Q_PROPERTY (bool connected READ isConnected NOTIFY connection)

	Q_INVOKABLE	bool isConnected();

signals:
	void connection(bool connected);

private slots:
	void checkForFileTimeout();

private:
	void setConnected(bool connected);
	void open();

	void threadFinished(AAPIOSelect*);

	bool connected;

	QTimer * timer;
	Scope * scope;
	BlueToothManager * blueToothManager;

	QByteArray pendingMessage;

	AAPIOSelect * select;

	QMutex threadMutex;

	friend class AAPIOSelect;
};

class AAPIOSelect: public QThread, public GateFactory {
Q_OBJECT
public:
	AAPIOSelect(AAPIO * owner, Scope * scope, BlueToothManager * blueToothManager);
	virtual ~AAPIOSelect();

	virtual void run();

	virtual void close();

private:
	AAPIO * owner;
	Scope * scope;
	BlueToothManager * blueToothManager;
	int fd;
	long long now();
};

#endif /* AAPIO_HPP_ */
