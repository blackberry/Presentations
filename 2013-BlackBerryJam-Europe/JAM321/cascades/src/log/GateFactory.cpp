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
#include "GateFactory.hpp"

#include <stdio.h>

class NameAndPointer {
public:
	NameAndPointer(const char * n, void * p);
	virtual ~NameAndPointer();
	QString name;
	void * pointer;
};

NameAndPointer::NameAndPointer(const char * n, void * p) :
		name(n), pointer(p) {

}

NameAndPointer::~NameAndPointer() {

}

QList<NameAndPointer*> GateFactory::allocs;
QMutex GateFactory::lock;

GateFactory::GateFactory(const char * name) {
//	fprintf(stderr,"GF: %s\n",name);
	this->name = name;
	QMutexLocker locker(&lock);
	NameAndPointer * nap = new NameAndPointer(name, this);
	allocs << nap;
}

GateFactory::~GateFactory() {
	QMutexLocker locker(&lock);
	bool removed = false;
	for (int i = 0; i < allocs.size(); ++i) {
		NameAndPointer * nap = allocs[i];
		if (nap->pointer == this) {
			allocs.removeAt(i);
			removed = true;
			break;
		}
	}

	if (!removed) {
		fprintf(stderr,"Ouch! Double delete?\n");
	}
}

void GateFactory::summary() {
	fprintf(stderr,"===== Non Freed Object Summary\n");
	for (int i = 0; i < allocs.size(); ++i) {
		NameAndPointer * nap = allocs[i];
		fprintf(stderr,"%s\n",nap->name.toAscii().data());
	}
	fprintf(stderr,"=end= Non Freed Object Summary\n");
}

