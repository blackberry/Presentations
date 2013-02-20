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
#include <stdio.h>

#ifndef BOUNDEDARRAY_HPP_
#define BOUNDEDARRAY_HPP_

#include "../log/GateFactory.hpp"

template<class T> class BoundedArray: public GateFactory {
public:
	BoundedArray() :
			GateFactory("BoundedArray") {
		body = NULL;
		size = 0;
	}
	BoundedArray(int size) :
			GateFactory("BoundedArray") {
		body = NULL;
		this->size = 0;
		setSize(size);
	}
	BoundedArray(const BoundedArray<T> & b) :
			GateFactory("BoundedArray") {
		if (b.body == NULL) {
			body = NULL;
			size = 0;
		} else {
			size = b.size;
			body = new T[size + 4];
			for (int i = 0; i < size + 4; ++i) {
				body[i] = b.body[i];
			}
		}
	}
	virtual ~BoundedArray() {
		if (body != NULL) {
			if (body[0] != 0 || body[1] != 0 || body[size + 2] != 0
					|| body[size + 3] != 0) {
				fprintf(stderr,"Bad sentinels\n");
			}
			delete[] body;
			body = NULL;
		} else {
			fprintf(stderr,"Empty bounded array deleted\n");
		}
	}

	void setSize(int size) {
		if (body != NULL) {
			delete[] body;
		}
		this->size = size;
		body = new T[size + 4];
		body[0] = 0;
		body[1] = 0;
		body[size + 2] = 0;
		body[size + 3] = 0;
	}

	T & operator[](int i) {
		if (i < 0 || i >= size) {
			fprintf(stderr,"OOB %d %d\n",i,size);
		}
		return body[i + 2];
	}
	T operator[](int i) const {
		if (i < 0 || i > size) {
			fprintf(stderr,"OOB %d %d\n",i,size);
		}
		return body[i + 2];
	}

	T * data() {
		return body + 2;
	}

	int size;
	T* body;
};

#endif /* BOUNDEDARRAY_HPP_ */
