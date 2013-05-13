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

#include "ObjectDrag.hpp"
#include <math.h>

#include <QDebug>

using namespace bb::cascades;

float ObjectDrag::cube_vertices[] = {
        // FRONT
        -2.0f, -2.0f,  2.0f,
         2.0f, -2.0f,  2.0f,
        -2.0f,  2.0f,  2.0f,
         2.0f,  2.0f,  2.0f,
        // BACK
        -2.0f, -2.0f, -2.0f, -2.0f, 2.0f, -2.0f, 2.0f, -2.0f,
        -2.0f,
        2.0f,
        2.0f,
        -2.0f,
        // LEFT
        -2.0f, -2.0f, 2.0f, -2.0f, 2.0f, 2.0f, -2.0f, -2.0f, -2.0f,
        -2.0f,
        2.0f,
        -2.0f,
        // RIGHT
        2.0f, -2.0f, -2.0f, 2.0f, 2.0f, -2.0f, 2.0f, -2.0f, 2.0f, 2.0f,
        2.0f,
        2.0f,
        // TOP
        -2.0f, 2.0f, 2.0f, 2.0f, 2.0f, 2.0f, -2.0f, 2.0f, -2.0f, 2.0f, 2.0f,
        -2.0f,
        // BOTTOM
        -2.0f, -2.0f, 2.0f, -2.0f, -2.0f, -2.0f, 2.0f, -2.0f, 2.0f, 2.0f, -2.0f,
        -2.0f, };

float ObjectDrag::cube_normals[] = {
        // FRONT
        0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
        0.0f,
        1.0f,
        // BACK
        0.0f, 0.0f, -1.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, -1.0f, 0.0f,
        0.0f,
        -1.0f,
        // LEFT
        -1.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, -1.0f, 0.0f,
        0.0f,
        // RIGHT
        1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
        // TOP
        0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
        // BOTTOM
        0.0f, -1.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, -1.0f,
        0.0f };


float ObjectDrag::pyramid_vertices[] = {
        // FRONT
        -2.0f, -2.0f,  2.0f,
         2.0f, -2.0f,  2.0f,
         0.0f,  2.0f,  0.0f,
        // BACK
         2.0f, -2.0f, -2.0f,
        -2.0f, -2.0f, -2.0f,
         0.0f,  2.0f,  0.0f,
        // LEFT
        -2.0f, -2.0f, -2.0f,
        -2.0f, -2.0f,  2.0f,
         0.0f,  2.0f,  0.0f,
        // RIGHT
         2.0f, -2.0f,  2.0f,
         2.0f, -2.0f, -2.0f,
         0.0f,  2.0f,  0.0f,
        // BOTTOM
        -2.0f, -2.0f, -2.0f,
         2.0f, -2.0f, -2.0f,
        -2.0f, -2.0f,  2.0f,
         2.0f, -2.0f,  2.0f,
         };

float ObjectDrag::pyramid_normals[] = {
        // FRONT
        0.0f,  0.6667f,  0.5f,
        0.0f,  0.6667f,  0.5f,
        0.0f,  0.6667f,  0.5f,
        0.0f,  0.6667f,  0.5f,
        // BACK
        0.0f,  0.6667f, -0.5f,
        0.0f,  0.6667f, -0.5f,
        0.0f,  0.6667f, -0.5f,
        0.0f,  0.6667f, -0.5f,
        // LEFT
       -0.5f,  0.6667f, 0.0f,
       -0.5f,  0.6667f, 0.0f,
       -0.5f,  0.6667f, 0.0f,
       -0.5f,  0.6667f, 0.0f,
        // RIGHT
        0.5f,  0.6667f, 0.0f,
        0.5f,  0.6667f, 0.0f,
        0.5f,  0.6667f, 0.0f,
        0.5f,  0.6667f, 0.0f,
        // BOTTOM
        0.0f, -1.0f, 0.0f,
        0.0f, -1.0f, 0.0f,
        0.0f, -1.0f, 0.0f,
        0.0f, -1.0f, 0.0f
};


float ObjectDrag::sphere_vertices[3 * 128 * 16] = { 0.0 };

float ObjectDrag::sphere_normals[3 * 128 * 16] = { 0.0 };


GLfloat ObjectDrag::light_ambient[] = { 0.5f, 0.5f, 0.5f, 1.0f };
GLfloat ObjectDrag::light_diffuse[] = { 0.8f, 0.8f, 0.8f, 1.0f };
GLfloat ObjectDrag::light_pos[] = { 0.0f, 25.0f, 0.0f, 1.0f };
GLfloat ObjectDrag::light_direction[] = { 0.0f, 0.0f, -30.0f, 1.0f };


ObjectDrag::ObjectDrag(VIEW_DISPLAY display)
{
		setDisplay(display);

	    _toolAxis = new QString("X");
	    _touchMode = new QString("none");
	    _model = new QString("pyramid");
	    _autoRotate = true;

	    // touch event handling
		currTouchX = -1;
		currTouchY = -1;
		lastTouchX = -1;
		lastTouchY = -1;

		// setup sphere
		float phi = M_PI / 2.0;
		float theta = 0.0;
		float deltaPhi = M_PI / 16.0;
		float deltaTheta = M_PI / 16.0;

		int phistep = 0;
		int thetastep = 0;
		int vertexIndex = 0;
		int normalIndex = 0;

		// build sphere vertices
		for (phistep = 0; phistep < 16; phistep++) {
			phi = M_PI / 2.0 - deltaPhi*phistep;

			for (thetastep = 0; thetastep < 32; thetastep++) {
				theta = 0.0 + deltaTheta*thetastep;
				//qDebug()  << "build sphere: "<< thetastep << ":" << phistep << theta << ":" << phi;

				sphere_vertices[vertexIndex++] = 2.0 * cos(phi) * cos(theta);
				sphere_vertices[vertexIndex++] = 2.0 * cos(phi) * sin(theta);
				sphere_vertices[vertexIndex++] = 2.0 * sin(phi);

				sphere_vertices[vertexIndex++] = 2.0 * cos(phi) * cos(theta + deltaTheta);
				sphere_vertices[vertexIndex++] = 2.0 * cos(phi) * sin(theta + deltaTheta);
				sphere_vertices[vertexIndex++] = 2.0 * sin(phi);

				sphere_vertices[vertexIndex++] = 2.0 * cos(phi - deltaPhi) * cos(theta);
				sphere_vertices[vertexIndex++] = 2.0 * cos(phi - deltaPhi) * sin(theta);
				sphere_vertices[vertexIndex++] = 2.0 * sin(phi - deltaPhi);

				sphere_vertices[vertexIndex++] = 2.0 * cos(phi - deltaPhi) * cos(theta + deltaTheta);
				sphere_vertices[vertexIndex++] = 2.0 * cos(phi - deltaPhi) * sin(theta + deltaTheta);
				sphere_vertices[vertexIndex++] = 2.0 * sin(phi - deltaPhi);


				sphere_normals[normalIndex++] = cos(phi) * cos(theta);
				sphere_normals[normalIndex++] = cos(phi) * sin(theta);
				sphere_normals[normalIndex++] = sin(phi);

				sphere_normals[normalIndex++] = cos(phi) * cos(theta + deltaTheta);
				sphere_normals[normalIndex++] = cos(phi) * sin(theta + deltaTheta);
				sphere_normals[normalIndex++] = sin(phi);

				sphere_normals[normalIndex++] = cos(phi - deltaPhi) * cos(theta);
				sphere_normals[normalIndex++] = cos(phi - deltaPhi) * sin(theta);
				sphere_normals[normalIndex++] = sin(phi - deltaPhi);

				sphere_normals[normalIndex++] = cos(phi - deltaPhi) * cos(theta + deltaTheta);
				sphere_normals[normalIndex++] = cos(phi - deltaPhi) * sin(theta + deltaTheta);
				sphere_normals[normalIndex++] = sin(phi - deltaPhi);

				//qDebug()  << "build sphere: "<< thetastep << ":" << phistep << ":" << theta << ":" << phi << ":" <<
				//		(2.0 * cos(phi) * cos(theta)) << ":" <<	(2.0 * cos(phi) * sin(theta)) << ":" <<  2.0 * sin(phi);
			}
		}
		//qDebug()  << "build sphere: "<< vertexIndex << ":" << normalIndex;

		moveWindow = false;

}

ObjectDrag::~ObjectDrag() {
	// TODO Auto-generated destructor stub
}

int ObjectDrag::initialize() {

	setZ(5);
	setTransparency(SCREEN_TRANSPARENCY_SOURCE_OVER);

	int returnCode = OpenGLView::initGL();
	if (returnCode == EXIT_SUCCESS) {
	    reset(false);

	    // reset stale flag for initialization only
	    setStale(false);

	    //Common gl setup
	    glShadeModel(GL_SMOOTH);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	    glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
	    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
	    glLightfv(GL_LIGHT0, GL_POSITION, light_pos);
	    glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, light_direction);

	    glEnable(GL_CULL_FACE);
	}

	qDebug()  << "ObjectDrag::initialize " << m_width << ":" << m_height;

    return returnCode;
}

int ObjectDrag::regenerate()
{
	int returnCode = OpenGLView::regenerate();

	reset(true);

	return returnCode;
}

void ObjectDrag::cleanup() {
	OpenGLView::cleanup();
}

void ObjectDrag::render() {
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	//Typical render pass
 	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//Then render the cube
	enable_3D();
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_COLOR_MATERIAL);
	glEnable(GL_DEPTH_TEST);

	glTranslatef(obj_pos_x, obj_pos_y, obj_pos_z);

	glScalef(obj_scale_x, obj_scale_y, obj_scale_z);

	glRotatef(obj_angle_roll, 1.0f, 0.0f, 0.0f);
	glRotatef(obj_angle_yaw, 0.0f, 0.0f, 1.0f);
	glRotatef(obj_angle_pitch, 0.0f, 1.0f, 0.0f);

	glColor4f(obj_color[0], obj_color[1], obj_color[2], obj_color[3]);

	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);

	if (!_model->compare("cube")) {
		glVertexPointer(3, GL_FLOAT, 0, cube_vertices);
		glNormalPointer(GL_FLOAT, 0, cube_normals);

		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
		glDrawArrays(GL_TRIANGLE_STRIP, 4, 4);
		glDrawArrays(GL_TRIANGLE_STRIP, 8, 4);
		glDrawArrays(GL_TRIANGLE_STRIP, 12, 4);
		glDrawArrays(GL_TRIANGLE_STRIP, 16, 4);
		glDrawArrays(GL_TRIANGLE_STRIP, 20, 4);
	} else
	if (!_model->compare("pyramid")) {
		glVertexPointer(3, GL_FLOAT, 0, pyramid_vertices);
		glNormalPointer(GL_FLOAT, 0, pyramid_normals);

		glDrawArrays(GL_TRIANGLE_STRIP, 0, 3);
		glDrawArrays(GL_TRIANGLE_STRIP, 3, 3);
		glDrawArrays(GL_TRIANGLE_STRIP, 6, 3);
		glDrawArrays(GL_TRIANGLE_STRIP, 9, 3);
		glDrawArrays(GL_TRIANGLE_STRIP, 12, 4);
	} else
	if (!_model->compare("sphere")) {
		glVertexPointer(3, GL_FLOAT, 0, sphere_vertices);
		glNormalPointer(GL_FLOAT, 0, sphere_normals);

		int phistep = 0;
		int thetastep = 0;

		for (phistep = 0; phistep < 16; phistep++) {
			for (thetastep = 0; thetastep < 32; thetastep++) {
				glDrawArrays(GL_TRIANGLE_STRIP, phistep*128+thetastep*4, 4);
			}
		}
	}

	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_NORMAL_ARRAY);

	glDisable(GL_LIGHTING);
	glDisable(GL_LIGHT0);
	glDisable(GL_COLOR_MATERIAL);
	glDisable(GL_DEPTH_TEST);
}

// properties and slots
QVariantList& ObjectDrag::objectColor() {

	m_viewMutex.lock();

	QVariantList* objectColor = new QVariantList();
	*objectColor << obj_color[0] << obj_color[1] << obj_color[2] << obj_color[3];

	m_viewMutex.unlock();

	return *objectColor;
}

void ObjectDrag::setObjectColor(QVariantList color) {

	m_viewMutex.lock();

	obj_color[0] = color[0].value<float>();
	obj_color[1] = color[1].value<float>();
	obj_color[2] = color[2].value<float>();
	obj_color[3] = color[3].value<float>();

	m_viewMutex.unlock();

	setStale(true);
}

QString ObjectDrag::model() {
	QString *model;

	m_viewMutex.lock();

	model = new QString(*_model);

	m_viewMutex.unlock();

	return *model;
}

void ObjectDrag::setModel(QString model) {

	m_viewMutex.lock();

	delete _model;

	_model = new QString(model);

	m_viewMutex.unlock();

	setStale(true);
}


void ObjectDrag::enable_2D() {
    glViewport(0, 0, (int)m_width, (int) m_height);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    glOrthof(0.0f, m_width / m_height, 0.0f, 1.0f, -1.0f, 1.0f);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glScalef(1.0f / m_height, 1.0f / m_height, 1.0f);
}

void ObjectDrag::enable_3D() {
    glViewport(0, 0, (int) m_width, (int) m_height);

    GLfloat aspect_ratio = m_width / m_height;

    GLfloat fovy = 13;
    GLfloat zNear = 1.0f;
    GLfloat zFar = 1000.0f;

    GLfloat top = tan(fovy * 0.0087266462599716478846184538424431f) * zNear;
    GLfloat bottom = -top;

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    glFrustumf(aspect_ratio * bottom, aspect_ratio * top, bottom, top, zNear,
            zFar);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void ObjectDrag::reset(bool skipColour) {
    obj_pos_x = 0.0f;
    obj_pos_x = 0.0f;
    obj_pos_x = 0.0f;

    if (m_width > m_height) {
        obj_pos_x = 2.9f;
        obj_pos_y = 0.3f;
        obj_pos_z = -20.0f;
    } else {
        obj_pos_x = 0.5f;
        obj_pos_y = 0.1f;
        obj_pos_z = -30.0f;
    }

    obj_angle_yaw = 0.0f;
    obj_angle_pitch = 30.0f;
    obj_angle_roll = 15.0f;

    obj_scale_x = 1.0f;
    obj_scale_y = 1.0f;
    obj_scale_z = 1.0f;

    if (!skipColour) {
		obj_color[0] = 1.0f;
		obj_color[1] = 0.0f;
		obj_color[2] = 0.0f;
		obj_color[3] = 1.0f;
    }

	setStale(true);
}

void ObjectDrag::update() {
	if (moveWindow) {
		int newpos[2];

		newpos[0] = m_x + drag_x - drag_anchor_x;
		newpos[1] = m_y + drag_y - drag_anchor_y;

		screen_set_window_property_iv(m_screen_win, SCREEN_PROPERTY_POSITION, newpos);

		qDebug()  << "update x,y: " << newpos[0] << ":" << newpos[1];

		m_x = newpos[0];
		m_y = newpos[1];

		drag_win_x = m_x;
		drag_win_y = m_y;

		drag_anchor_x = drag_x;
		drag_anchor_y = drag_y;

		moveWindow = false;

		setStale(true);
	}

	if (_autoRotate) {
		if (!_toolAxis->compare("X")) {
			obj_angle_roll = fmod((obj_angle_roll + 1.0f), 360.0 );

			setStale(true);
		} else
		if (!_toolAxis->compare("Y")) {
			obj_angle_pitch = fmod((obj_angle_pitch + 1.0f), 360.0 );

			setStale(true);
		} else
		if (!_toolAxis->compare("Z")) {
			obj_angle_yaw = fmod((obj_angle_yaw + 1.0f), 360.0 );

			setStale(true);
		}
	}
}

void ObjectDrag::handleScreenEvent(bps_event_t *event) {
    int screenEvent;
    int buttons;
    int position[2];
    int realPosition[2];

    screen_event_t screen_event = screen_event_get_event(event);

    //Query type of screen event and its location on the screen
    screen_get_event_property_iv(screen_event, SCREEN_PROPERTY_TYPE,
            &screenEvent);
    screen_get_event_property_iv(screen_event, SCREEN_PROPERTY_SOURCE_POSITION,
    		position);
    screen_get_event_property_iv(screen_event, SCREEN_PROPERTY_POSITION,
    		realPosition);

	switch (screenEvent) {
		case SCREEN_EVENT_MTOUCH_TOUCH:
			drag_anchor_x = realPosition[0];
			drag_anchor_y = realPosition[1];
			drag_win_x = m_x;
			drag_win_y = m_y;

			qDebug()  << "anchor: " << position[0] << ":" << position[1] << ":" << realPosition[0] << ":" << realPosition[1];

			moveWindow = false;

			break;

		case SCREEN_EVENT_MTOUCH_MOVE:
			int newpos[2];
			drag_x = realPosition[0];
			drag_y = realPosition[1];

			moveWindow = true;

			qDebug()  << "newpos: " << position[0] << ":" << position[1] << ":" << drag_anchor_x << ":" << drag_anchor_y << ":" << realPosition[0] << ":" << realPosition[1];

			break;

		case SCREEN_EVENT_MTOUCH_RELEASE:
			drag_x = realPosition[0];
			drag_y = realPosition[1];

			moveWindow = true;

			qDebug()  << "release: "<< newpos[0] << ":" << newpos[1] << ":" << realPosition[0] << ":" << realPosition[1];
			break;

		case SCREEN_EVENT_POINTER:
	        screen_get_event_property_iv(screen_event, SCREEN_PROPERTY_BUTTONS,
				&buttons);

	    	switch (buttons) {
	    		case SCREEN_LEFT_MOUSE_BUTTON:
	    		case SCREEN_RIGHT_MOUSE_BUTTON:
	                //handleClick(position[0], position[1]);
	    			break;
	    	}

			break;
	}
}
