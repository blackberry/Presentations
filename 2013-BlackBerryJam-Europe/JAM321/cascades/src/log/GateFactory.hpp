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
#ifndef GATEFACTORY_HPP_
#define GATEFACTORY_HPP_

#include <QString>
#include <QMutex>
#include <QMutexLocker>
#include <QList>

#include "Gate.hpp"

class NameAndPointer;

class GateFactory {
friend class Gate;
public:
	GateFactory(const char * name);
	GateFactory(const QString & name);
	virtual ~GateFactory();

	static QMutex lock;
	static QList<NameAndPointer*> allocs;

	static void summary();

private:
	QString name;
};

#endif /* GATEFACTORY_HPP_ */
