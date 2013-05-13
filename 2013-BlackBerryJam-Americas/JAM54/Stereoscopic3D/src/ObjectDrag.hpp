/*
 * Copyright (c) 2011-2012 Research In Motion Limited.
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


#ifndef OBJECTDRAG_HPP
#define OBJECTDRAG_HPP

#include <assert.h>
#include <screen/screen.h>
#include <stdarg.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

#include <bps/navigator.h>
#include <bps/screen.h>
#include <bps/bps.h>
#include <bps/event.h>

#include <EGL/egl.h>
#include <GLES/gl.h>

#include <bb/cascades/TouchEvent>

#include <QtCore/QObject>
#include <QtCore/QString>

#include "OpenGLView.hpp"
#include "OpenGLThread.hpp"

class ObjectDrag : public OpenGLView {

Q_OBJECT

Q_PROPERTY(QVariantList objectColor READ objectColor WRITE setObjectColor)
Q_PROPERTY(QString model READ model WRITE setModel)

public:
	ObjectDrag(VIEW_DISPLAY display);

	virtual ~ObjectDrag();

	QVariantList& objectColor();
	QString model();

Q_SIGNALS:
	//void result(const QString &result);

public Q_SLOTS:
	void setObjectColor(QVariantList objectColor);
	void setModel(QString touchMode);
	void reset(bool skipColour);

public:

	int initialize();
	virtual void cleanup();

	void handleScreenEvent(bps_event_t *event);

	int regenerate();

	void update();
	void render();

private:
	void enable_2D();
	void enable_3D();


	// geometry definitions
	// cube
	static float cube_vertices[];
	static float cube_normals[];
	static float pyramid_vertices[];
	static float pyramid_normals[];
	static float sphere_vertices[];
	static float sphere_normals[];
	float obj_color[4];

	float obj_pos_x, obj_pos_y, obj_pos_z;
	float obj_scale_x, obj_scale_y, obj_scale_z;

	bool _autoRotate;
	QString* _touchMode;
	QString* _model;
	QString* _toolAxis;
	float obj_angle_pitch, obj_angle_yaw, obj_angle_roll;
	int currTouchX, currTouchY;
	int lastTouchX, lastTouchY;

	// lights
	static GLfloat light_ambient[];
	static GLfloat light_diffuse[];
	static GLfloat light_pos[];
	static GLfloat light_direction[];

	// screens / windows
	float screenWidth, screenHeight;

	//static font_t* font;

	int drag_win_x, drag_win_y;
	int drag_anchor_x, drag_anchor_y;
	int drag_x, drag_y;

    bool moveWindow;
};

#endif /* OBJECTDRAG_HPP */

