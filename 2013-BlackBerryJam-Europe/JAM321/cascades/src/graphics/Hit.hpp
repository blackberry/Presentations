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
#ifndef HIT_H_
#define HIT_H_

#include "../log/GateFactory.hpp"
#include <QList>
#include <QMutex>
#include <QMutexLocker>

class Shape;

class Hit: public GateFactory {
public:
	Hit();
	virtual ~Hit();

	void draw();

	void set(double x, double y);

	bool refresh(long long when);

	long long created();

private:
	long long _created;

	double x;
	double y;
	double currentAlpha;


	static QList<Shape*> defaultRings;

	static const int ringCount;
	static const int subDiv;
	static const int solid;
	static const int fadeOut;
	static const double size;

	static int hitCount;
	static QMutex hitCountLock;
};

#endif /* HIT_H_ */
