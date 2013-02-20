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
#include "Shape.hpp"

#include <stdio.h>
#include "../log/Gate.hpp"

Shape::Shape(BoundedArray<GLfloat> & verts, int vcount) :
		GateFactory("Shape"), vertices(verts), count(vcount) {
}

Shape::Shape(const Shape & s) :
		GateFactory("Shape"), vertices(s.vertices) {
	count = s.count;
}

Shape::~Shape() {
}

void Shape::draw() {
#ifndef NO_GRAPHICS
	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(2, GL_FLOAT, 0, vertices.data());
	glDrawArrays(GL_TRIANGLES, 0, count);
	glDisableClientState(GL_VERTEX_ARRAY);
#endif
}

void Shape::translate(double x, double y) {
	for (int i = 0; i < count; ++i) {
		vertices[i * 2] += x;
		vertices[i * 2 + 1] += y;
	}
}

void Shape::scale(double x, double y) {
	for (int i = 0; i < count; ++i) {
		vertices[i * 2] *= x;
		vertices[i * 2 + 1] *= y;
	}
}
