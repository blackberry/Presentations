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
#include "Scope.hpp"
#include "DrawTools.hpp"
#include <stdio.h>
#include <stdlib.h>

#include <QMutexLocker>

#include "log/Gate.hpp"

#include <math.h>
#include <sys/time.h>
#include <stdlib.h>
#include <stddef.h>

long long now() {
	struct timeval tv;
	gettimeofday(&tv, NULL);

	long long now = (tv.tv_sec) * 1000 + (tv.tv_usec) / 1000;
	return now;
}

Scope::Scope(QObject* owner) :
		QObject(owner), GateFactory("Scope"), enabled(1) {
	Gate g(this, "Scope()");
	int count = 100;
	double scale = 1;
	// TODO Auto-generated constructor stub
	rings << DrawTools::drawArc(scale * 1, 0, M_PI * 2, count);
	rings << DrawTools::drawArc(scale * 0.9, 0, M_PI * 2, count);
	rings << DrawTools::drawArc(scale * 0.7, 0, M_PI * 2, count);
	rings << DrawTools::drawArc(scale * 0.3, 0, M_PI * 2, count);

	for (int i = 0; i < 360; ++i) {
		double angle = M_PI * 2 / 360 * i;
		if (i % 10 == 0) {
			spokes << DrawTools::drawRadialLine(scale * 0.2, scale * 1, angle);
		}
		ticks << DrawTools::drawRadialLine(scale * 1, scale * 1.03, angle);
	}

	for (int i = 0; i < 180; i += 9) {
		recordHit(i, 1024 * i / 180);
	}
}

Scope::~Scope() {
	Gate g(this, "~Scope()");

	while (rings.size() != 0) {
		delete rings[0];
		rings.pop_front();
	}

	while (spokes.size() != 0) {
		delete spokes[0];
		spokes.pop_front();
	}

	while (ticks.size() != 0) {
		delete ticks[0];
		ticks.pop_front();
	}

	while (hits.size() != 0) {
		delete hits[0];
		hits.pop_front();
	}
}

void Scope::draw() {
#ifndef NO_GRAPHICS
	glPushMatrix();
	glScalef(0.8, 0.8, 1);
	glTranslatef(0, -0.5, 0);

	glColor4f(0, 0.5, 0, 1);
	glLineWidth(1);

	for (int i = 0; i < ticks.size(); ++i) {
		ticks[i]->draw();
	}

	glLineWidth(2);
	for (int i = 0; i < spokes.size(); ++i) {
		spokes[i]->draw();
	}

	if (enabled) {
		glColor4f(0.2, 0.8, 0.2, 1);
	} else {
		glColor4f(0.8, 0.2, 0.2, 1);
	}
	for (int i = 0; i < 4; ++i) {
		glLineWidth(i == 0 ? 5 : 2);
		rings[i]->draw();
	}

	long long when = now();

	{
		QMutexLocker locker(&lock);

		for (int i = 0; i < hits.size(); ++i) {
			Hit * current = hits[i];
			if (current->refresh(when)) {
				current->draw();
			}
		}

	}

	glPopMatrix();
#endif
}

void Scope::recordHit(int angle, int distance) {
	long long min = 0;
	int whichMin = -1;

	QMutexLocker locker(&lock);

	double x = 1.0 * (1024 - distance) / 1024 * cos(1.0 * angle / 180 * M_PI);
	double y = 1.0 * (1024 - distance) / 1024 * sin(1.0 * angle / 180 * M_PI);

	if (hits.size() < 1000) {
		Gate g("<1000");
		g.log() << hits.size();

		Hit * toAdd = new Hit();
		hits << toAdd;
		whichMin = hits.size() - 1;
	} else {
		for (int i = 0; i < hits.size(); ++i) {
			Hit & current = *hits[i];
			long long when = current.created();
			if (when == -1) {
				whichMin = i;
				break;
			}
			if (i == 0 || when < min) {
				whichMin = i;
			}
		}
	}

	hits[whichMin]->set(x, y);
}

void Scope::setEnabled(int enabled) {
	QMutexLocker locker(&lock);
	this->enabled = enabled;
}
