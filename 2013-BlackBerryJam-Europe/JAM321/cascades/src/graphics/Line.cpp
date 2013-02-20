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
#include "Line.hpp"

#include <EGL/egl.h>
#include <GLES/gl.h>
#include <GLES/glext.h>

Line::Line(BoundedArray<GLfloat> & vert, int vcount) :
		GateFactory("Line"), vertices(vert), count(vcount) {
}

Line::~Line() {
}

void Line::draw() {
	glEnableClientState(GL_VERTEX_ARRAY);

	glVertexPointer(2, GL_FLOAT, 0, vertices.data());
	glDrawArrays(GL_LINE_STRIP, 0, count);

	glDisableClientState(GL_VERTEX_ARRAY);
}

