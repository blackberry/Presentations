/*
 * Copyright (c) 2013 SurF-X Interactive
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

#ifndef PHOTO3DVIEW_HPP
#define PHOTO3DVIEW_HPP

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

#include <bb/utility/ImageConverter>
#include <bb/ImageData>
#include <bb/PixelFormat>
#include <bb/cascades/TouchEvent>
#include <bb/utility/ImageConverter>

#include <QtCore/QString>

#include "OpenGLView.hpp"
#include "OpenGLThread.hpp"

//
typedef enum VIEW_3D_TYPE {VIEW_3D_TYPE_TOP_BOTTOM_LEFT_RIGHT = 0, VIEW_3D_TYPE_SIDE_BY_SIDE_LEFT_RIGHT, VIEW_3D_TYPE_ANAGLYPH_RED_CYAN, VIEW_3D_TYPE_ANAGLYPH_CYAN_RED, VIEW_3D_TYPE_ANAGLYPH_RED_GREEN, VIEW_3D_TYPE_ANAGLYPH_GREEN_RED, VIEW_3D_TYPE_ANAGLYPH_BLUE_YELLOW, VIEW_3D_TYPE_ANAGLYPH_YELLOW_BLUE, VIEW_3D_TYPE_ANAGLYPH_CLEAR_VIOLET } VIEW_3D_TYPE;

class Photo3DView : public OpenGLView {

Q_OBJECT

Q_PROPERTY(QString leftPhotoFilename READ leftPhotoFilename WRITE setLeftPhotoFilename) // left Photo Filename
Q_PROPERTY(QString rightPhotoFilename READ rightPhotoFilename WRITE setRightPhotoFilename) // right Photo Filename

public:
	Photo3DView(VIEW_DISPLAY display);

	virtual ~Photo3DView();

	// property signals
	QString leftPhotoFilename();
	QString rightPhotoFilename();

public Q_SLOTS:
	// property slots
	void setLeftPhotoFilename(QString leftPhotoFilename);
	void setRightPhotoFilename(QString rightPhotoFilename);

	// action slots
	void reset();

	void setDisplayMode(int mode);

	int loadJPGPhoto(const QString& filename, int* width, int* height, float* tex_x, float* tex_y, unsigned int *tex);
	int loadJPGPhotos(const QString& filename, const QString& filename1, int* width, int* height, float* tex_x, float* tex_y, unsigned int *tex);

	int genJPGThumbnail(const QString& filename, const QString& thumbFilename);

	// touch handler
	void onTouch(bb::cascades::TouchEvent *event);

public:
	// overriden methods from OpenGLView
	int initialize();
	int regenerate();
	void cleanup();

	void handleScreenEvent(bps_event_t *event);

	void update();
	void render();

private:
	// view transform functions
	void setup2DView();

	// photo file names
	QString* _leftPhotoFilename;
	QString* _rightPhotoFilename;

	// left / anaglyph photo data
	GLfloat leftPhotoVertices[8];
	GLfloat leftPhotoTexCoord[8];
	GLuint leftPhoto;
	float leftPhotoPosX, leftPhotoPosY, leftPhotoSizeX, leftPhotoSizeY;

	// right photo data
	GLfloat rightPhotoVertices[8];
	GLfloat rightPhotoTexCoord[8];
	GLuint rightPhoto;
	float rightPhotoPosX, rightPhotoPosY, rightPhotoSizeX, rightPhotoSizeY;

	int m_mode;
};

#endif /* PHOTO3DVIEW_HPP */

