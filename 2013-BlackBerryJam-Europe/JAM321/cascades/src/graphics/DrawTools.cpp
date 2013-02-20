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
#include "DrawTools.hpp"
#include <math.h>

#include "Line.hpp"
#include "Shape.hpp"

Line * DrawTools::drawArc(double radius, double beginAngle, double endAngle, int steps) {
	int vertCount = steps + 1;

	BoundedArray<GLfloat> verts(2*vertCount);

	double delta = endAngle - beginAngle;
	double step = delta / steps;

	double angle = beginAngle;

	for (int i=0; i<=steps; ++i) {
		verts[i*2] = sin(angle) * radius;
		verts[i*2+1] = cos(angle) * radius;
		angle += step;
	}

	return new Line(verts,vertCount);
}

Line * DrawTools::drawRadialLine(double startRadius, double endRadius, double angle) {
	BoundedArray<GLfloat> verts(4);
	verts[0] = startRadius * sin(angle);
	verts[1] = startRadius * cos(angle);
	verts[2] = endRadius * sin(angle);
	verts[3] = endRadius * cos(angle);
	return new Line(verts,2);
}

Shape * DrawTools::drawSolidArc(double startRadius, double endRadius, double startAngle, double endAngle, int steps) {
	int vertCount = steps * 6;

	BoundedArray<GLfloat> verts(2 * vertCount);

	double delta = endAngle - startAngle;
	double step = delta / steps;

	int offset = 0;

	double angle = startAngle;

	for (int i=0; i<steps; ++i) {
		float tlx = endRadius * sin(angle);
		float tly = endRadius * cos(angle);
		float trx = endRadius * sin(angle+step);
		float trY = endRadius * cos(angle+step);

		float blx = startRadius * sin(angle);
		float bly = startRadius * cos(angle);
		float brx = startRadius * sin(angle+step);
		float bry = startRadius * cos(angle+step);

		verts[offset++] = tlx;
		verts[offset++] = tly;

		verts[offset++] = blx;
		verts[offset++] = bly;

		verts[offset++] = brx;
		verts[offset++] = bry;

		verts[offset++] = brx;
		verts[offset++] = bry;

		verts[offset++] = tlx;
		verts[offset++] = tly;

		verts[offset++] = trx;
		verts[offset++] = trY;

		angle += step;
	}

	return new Shape(verts,vertCount);
}
