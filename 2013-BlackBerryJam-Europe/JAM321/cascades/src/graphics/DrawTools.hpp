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
#ifndef DRAWTOOLS_H_
#define DRAWTOOLS_H_

class Line;
class Shape;

class DrawTools {
public:
	static Line * drawArc(double radius, double beginAngle, double endAngle, int steps);
	static Line * drawRadialLine(double startRadius, double endRadius, double angle);
	static Shape * drawSolidArc(double startRadius, double endRadius, double startAngle, double endAngle, int steps);
};

#endif /* DRAWTOOLS_H_ */
