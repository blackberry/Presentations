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
#ifndef SCOPE_HPP_
#define SCOPE_HPP_

#include <QObject>
#include <QMutex>

#include "Line.hpp"
#include "Shape.hpp"
#include "Hit.hpp"

#include "../log/GateFactory.hpp"

#include <list>

class Scope : public QObject, public GateFactory {
	Q_OBJECT
public:
	Scope(QObject * owner = NULL);
	virtual ~Scope();

	virtual void draw();

    void setEnabled(int enabled);

public slots:
	void recordHit(int angle, int distance);

private:
	QList<Line*> rings;
	QList<Line*> spokes;
	QList<Line*> ticks;
	QList<Hit*> hits;

	int enabled;

	QMutex lock;
};

#endif /* SCOPE_HPP_ */
