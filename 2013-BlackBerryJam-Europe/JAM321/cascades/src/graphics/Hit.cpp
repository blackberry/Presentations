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
#include <EGL/egl.h>
#include <GLES/gl.h>
#include <GLES/glext.h>

#include "Hit.hpp"
#include "DrawTools.hpp"
#include "Shape.hpp"

#include <math.h>
#include <sys/time.h>
#include <stdlib.h>
#include <stddef.h>

#include "../log/Gate.hpp"
#include "../util/Now.hpp"

const int Hit::ringCount = 20;
const int Hit::subDiv = 32;

const int Hit::solid = 500;
const int Hit::fadeOut = 1500;

const double Hit::size = 0.05;

QList<Shape*> Hit::defaultRings;

int Hit::hitCount = 0;

QMutex Hit::hitCountLock;

Hit::Hit() :
		GateFactory("Hit"), _created(-1), currentAlpha(0), x(0), y(0) {
	QMutexLocker locker(&hitCountLock);
	hitCount++;
	if (hitCount == 1) {
		for (int i = 0; i < ringCount; ++i) {
			if (i >= defaultRings.size()) {
				defaultRings
						<< DrawTools::drawSolidArc(0,
								size * (ringCount - i) / ringCount, 0, M_PI * 2,
								subDiv);
			}
		}
	}
}

void Hit::set(double x, double y) {
	_created = Now::now();
	currentAlpha = 1;
	this->x = x;
	this->y = y;

}

Hit::~Hit() {
	QMutexLocker locker(&hitCountLock);
	--hitCount;
	if (hitCount == 0) {
		while (defaultRings.size() != 0) {
			delete defaultRings[0];
			defaultRings.pop_front();
		}
	}
}

void Hit::draw() {
	if (currentAlpha >= 0) {
		glPushMatrix();
		glTranslatef(x, y, 0);
		for (int i = 0; i < defaultRings.size(); ++i) {
			glColor4f(1.0, 1.0, 0, currentAlpha * (i + 1) / ringCount);
			defaultRings[i]->draw();
		}
		glPopMatrix();
	}
}

bool Hit::refresh(long long when) {
	if (_created == -1) {
		return false;
	}
	long delta = when - _created;
	if (delta < solid) {
		currentAlpha = 1.0;
	} else {
		delta = delta - solid;
		currentAlpha = 1.0 * (fadeOut - delta) / fadeOut;
	}
	if (currentAlpha <= 0) {
		_created = -1;
		return false;
	}
	return true;
}

long long Hit::created() {
	return _created;
}
