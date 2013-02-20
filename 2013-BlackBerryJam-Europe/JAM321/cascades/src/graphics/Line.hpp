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
#ifndef LINE_H_
#define LINE_H_

#include <EGL/egl.h>
#include <GLES/gl.h>

#include "../util/BoundedArray.hpp"
#include "../log/GateFactory.hpp"

class Line : public GateFactory {
public:
	Line(BoundedArray<GLfloat> & vertices, int count);
	virtual ~Line();

	virtual void draw();
private:
	BoundedArray<GLfloat> vertices;
	int count;
};

#endif /* LINE_H_ */
