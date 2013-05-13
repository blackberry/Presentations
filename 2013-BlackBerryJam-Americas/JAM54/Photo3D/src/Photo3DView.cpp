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

#include "Photo3DView.hpp"
#include <math.h>

#include <QDebug>
#include <QFile>

using namespace bb;
using namespace bb::cascades;
using namespace bb::utility;


Photo3DView::Photo3DView(VIEW_DISPLAY display)
{
		setDisplay(display);

		_leftPhotoFilename = NULL;
		_rightPhotoFilename = NULL;
		leftPhoto = 0;
		rightPhoto = 0;
}

Photo3DView::~Photo3DView() {
	// TODO Auto-generated destructor stub
}

int Photo3DView::initialize() {

	setZ(-5);

	int returnCode = OpenGLView::initGL();
	if (returnCode == EXIT_SUCCESS) {
		reset();

		// reset stale flag for initialization only
		setStale(true);

		//Common gl setup
		glShadeModel(GL_SMOOTH);
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	}

	qDebug()  << "Photo3DView::initialize " << ":" << returnCode;

    return returnCode;
}

int Photo3DView::regenerate()
{
	int returnCode = OpenGLView::regenerate();

	leftPhoto = 0;
	rightPhoto = 0;

	reset();

	return returnCode;
}

void Photo3DView::cleanup() {
	OpenGLView::cleanup();
}

void Photo3DView::render() {
//	if (m_display == DISPLAY_DEVICE) {
		glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		reset();

		setup2DView();

		//glColor4f(1.0f, 0.0f, 0.0f, 1.0f);

		if (leftPhoto > 0) {
			switch(m_mode) {
			case VIEW_3D_TYPE_TOP_BOTTOM_LEFT_RIGHT:
			case VIEW_3D_TYPE_SIDE_BY_SIDE_LEFT_RIGHT:
			case VIEW_3D_TYPE_ANAGLYPH_RED_CYAN:

				glEnable(GL_TEXTURE_2D);

				glEnableClientState(GL_VERTEX_ARRAY);
				glEnableClientState(GL_TEXTURE_COORD_ARRAY);

				glVertexPointer(2, GL_FLOAT, 0, leftPhotoVertices);
				glTexCoordPointer(2, GL_FLOAT, 0, leftPhotoTexCoord);
				glBindTexture(GL_TEXTURE_2D, leftPhoto);

				glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

				glDisableClientState(GL_TEXTURE_COORD_ARRAY);
				glDisableClientState(GL_VERTEX_ARRAY);
				glDisable(GL_TEXTURE_2D);
				break;
			}
		}

		if (rightPhoto > 0) {
			switch(m_mode) {
			case VIEW_3D_TYPE_TOP_BOTTOM_LEFT_RIGHT:
			case VIEW_3D_TYPE_SIDE_BY_SIDE_LEFT_RIGHT:
				glEnable(GL_TEXTURE_2D);


				glEnableClientState(GL_VERTEX_ARRAY);
				glEnableClientState(GL_TEXTURE_COORD_ARRAY);

				glVertexPointer(2, GL_FLOAT, 0, rightPhotoVertices);
				glTexCoordPointer(2, GL_FLOAT, 0, rightPhotoTexCoord);
				glBindTexture(GL_TEXTURE_2D, rightPhoto);

				glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

				glDisableClientState(GL_TEXTURE_COORD_ARRAY);
				glDisableClientState(GL_VERTEX_ARRAY);
				glDisable(GL_TEXTURE_2D);
				break;
			}

			qDebug() << "right photo vertices: " << rightPhotoVertices[0] << ":" << rightPhotoVertices[1]
								 << ":" << rightPhotoVertices[2] << ":" << rightPhotoVertices[3] << ":" << rightPhotoVertices[4]
								 << ":" << rightPhotoVertices[5] << ":" << rightPhotoVertices[6] << ":" << rightPhotoVertices[7];

			qDebug() << "right photo tex coords: " << rightPhotoTexCoord[0] << ":" << rightPhotoTexCoord[1]
								 << ":" << rightPhotoTexCoord[2] << ":" << rightPhotoTexCoord[3] << ":" << rightPhotoTexCoord[4]
								 << ":" << rightPhotoTexCoord[5] << ":" << rightPhotoTexCoord[6] << ":" << rightPhotoTexCoord[7];
		}
	// after rendering this view is no longer stale
	setStale(false);
}

// properties and slots

QString Photo3DView::leftPhotoFilename() {
	QString *leftPhotoFilename;

	m_viewMutex.lock();

	leftPhotoFilename = new QString(*_leftPhotoFilename);

	m_viewMutex.unlock();

	return *leftPhotoFilename;
}

void Photo3DView::setLeftPhotoFilename(QString leftPhotoFilename) {

	m_viewMutex.lock();

	if (_leftPhotoFilename != NULL) {
		delete _leftPhotoFilename;
	}

	_leftPhotoFilename = new QString(leftPhotoFilename);

	m_viewMutex.unlock();

	setStale(true);
}

QString Photo3DView::rightPhotoFilename() {
	QString *rightPhotoFilename;

	m_viewMutex.lock();

	rightPhotoFilename = new QString(*_rightPhotoFilename);

	m_viewMutex.unlock();

	return *rightPhotoFilename;
}

void Photo3DView::setRightPhotoFilename(QString rightPhotoFilename) {

	m_viewMutex.lock();

	if (_rightPhotoFilename != NULL) {
		delete _rightPhotoFilename;
	}

	_rightPhotoFilename = new QString(rightPhotoFilename);

	m_viewMutex.unlock();

	setStale(true);
}


void Photo3DView::setup2DView() {
    glViewport(0, 0, m_width, m_height);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    glOrthof(0.0f, (float)m_width / (float)m_height, 0.0f, 1.0f, -1.0f, 1.0f);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glScalef(1.0f / (float)m_height, 1.0f / (float)m_height, 1.0f);
}

void Photo3DView::reset() {
	//Load left photo textures
	float tex_x = 1.0f, tex_y = 1.0f;

	//if (initialized() && enabled() && visible()) {
		if (_leftPhotoFilename != NULL && _rightPhotoFilename != NULL) {
			switch(m_mode) {
			case VIEW_3D_TYPE_ANAGLYPH_RED_CYAN:

				if (EXIT_SUCCESS
						!= loadJPGPhotos(*(_leftPhotoFilename), *(_rightPhotoFilename), NULL, NULL,
								&tex_x, &tex_y, &leftPhoto)) {
					fprintf(stderr, "Unable to load left photo\n");
				} else {

					leftPhotoPosX = 0.0f;
					leftPhotoPosY = 0.0f;

					leftPhotoSizeX = 1920.0f;
					leftPhotoSizeY = 1080.0f;

					leftPhotoTexCoord[0] = 0.0f;
					leftPhotoTexCoord[1] = tex_y;
					leftPhotoTexCoord[2] = tex_x;
					leftPhotoTexCoord[3] = tex_y;
					leftPhotoTexCoord[4] = 0.0f;
					leftPhotoTexCoord[5] = 0.0f;
					leftPhotoTexCoord[6] = tex_x;
					leftPhotoTexCoord[7] = 0.0f;

					leftPhotoVertices[0] = leftPhotoPosX;
					leftPhotoVertices[1] = leftPhotoPosY;
					leftPhotoVertices[2] = leftPhotoPosX + leftPhotoSizeX;
					leftPhotoVertices[3] = leftPhotoPosY;
					leftPhotoVertices[4] = leftPhotoPosX;
					leftPhotoVertices[5] = leftPhotoPosY + leftPhotoSizeY;
					leftPhotoVertices[6] = leftPhotoPosX + leftPhotoSizeX;
					leftPhotoVertices[7] = leftPhotoPosY + leftPhotoSizeY;
				}

				break;

			case VIEW_3D_TYPE_TOP_BOTTOM_LEFT_RIGHT:
			case VIEW_3D_TYPE_SIDE_BY_SIDE_LEFT_RIGHT:

				if (EXIT_SUCCESS
						!= loadJPGPhoto(*(_leftPhotoFilename), NULL, NULL,
								&tex_x, &tex_y, &leftPhoto)) {
					fprintf(stderr, "Unable to load left photo\n");
				} else {

					//Load right photo textures
					if (EXIT_SUCCESS
							!= loadJPGPhoto(*(_rightPhotoFilename), NULL, NULL,
									&tex_x, &tex_y, &rightPhoto)) {
						fprintf(stderr, "Unable to load right photo\n");
					} else {


						switch(m_mode) {

						case VIEW_3D_TYPE_TOP_BOTTOM_LEFT_RIGHT:
							leftPhotoPosX = 0.0f;
							leftPhotoPosY = 540.0f;

							leftPhotoSizeX = 1920.0f;
							leftPhotoSizeY = 540.0f;

							leftPhotoTexCoord[0] = 0.0f;
							leftPhotoTexCoord[1] = tex_y;
							leftPhotoTexCoord[2] = tex_x;
							leftPhotoTexCoord[3] = tex_y;
							leftPhotoTexCoord[4] = 0.0f;
							leftPhotoTexCoord[5] = 0.0f;
							leftPhotoTexCoord[6] = tex_x;
							leftPhotoTexCoord[7] = 0.0f;

							leftPhotoVertices[0] = leftPhotoPosX;
							leftPhotoVertices[1] = leftPhotoPosY;
							leftPhotoVertices[2] = leftPhotoPosX + leftPhotoSizeX;
							leftPhotoVertices[3] = leftPhotoPosY;
							leftPhotoVertices[4] = leftPhotoPosX;
							leftPhotoVertices[5] = leftPhotoPosY + leftPhotoSizeY;
							leftPhotoVertices[6] = leftPhotoPosX + leftPhotoSizeX;
							leftPhotoVertices[7] = leftPhotoPosY + leftPhotoSizeY;


							rightPhotoPosX = 0.0f;
							rightPhotoPosY = 0.0f;

							rightPhotoSizeX = 1920.0f;
							rightPhotoSizeY = 540.0f;

							rightPhotoTexCoord[0] = 0.0f;
							rightPhotoTexCoord[1] = tex_y;
							rightPhotoTexCoord[2] = tex_x;
							rightPhotoTexCoord[3] = tex_y;
							rightPhotoTexCoord[4] = 0.0f;
							rightPhotoTexCoord[5] = 0.0f;
							rightPhotoTexCoord[6] = tex_x;
							rightPhotoTexCoord[7] = 0.0f;

							rightPhotoVertices[0] = rightPhotoPosX;
							rightPhotoVertices[1] = rightPhotoPosY;
							rightPhotoVertices[2] = rightPhotoPosX + rightPhotoSizeX;
							rightPhotoVertices[3] = rightPhotoPosY;
							rightPhotoVertices[4] = rightPhotoPosX;
							rightPhotoVertices[5] = rightPhotoPosY + rightPhotoSizeY;
							rightPhotoVertices[6] = rightPhotoPosX + rightPhotoSizeX;
							rightPhotoVertices[7] = rightPhotoPosY + rightPhotoSizeY;
							break;

						case VIEW_3D_TYPE_SIDE_BY_SIDE_LEFT_RIGHT:
							leftPhotoPosX = 0.0f;
							leftPhotoPosY = 0.0f;

							leftPhotoSizeX = 960.0f;
							leftPhotoSizeY = 1080.0f;

							leftPhotoTexCoord[0] = 0.0f;
							leftPhotoTexCoord[1] = tex_y;
							leftPhotoTexCoord[2] = tex_x;
							leftPhotoTexCoord[3] = tex_y;
							leftPhotoTexCoord[4] = 0.0f;
							leftPhotoTexCoord[5] = 0.0f;
							leftPhotoTexCoord[6] = tex_x;
							leftPhotoTexCoord[7] = 0.0f;

							leftPhotoVertices[0] = leftPhotoPosX;
							leftPhotoVertices[1] = leftPhotoPosY;
							leftPhotoVertices[2] = leftPhotoPosX + leftPhotoSizeX;
							leftPhotoVertices[3] = leftPhotoPosY;
							leftPhotoVertices[4] = leftPhotoPosX;
							leftPhotoVertices[5] = leftPhotoPosY + leftPhotoSizeY;
							leftPhotoVertices[6] = leftPhotoPosX + leftPhotoSizeX;
							leftPhotoVertices[7] = leftPhotoPosY + leftPhotoSizeY;


							rightPhotoPosX = 960.0f;
							rightPhotoPosY = 0.0f;

							rightPhotoSizeX = 960.0f;
							rightPhotoSizeY = 1080.0f;

							rightPhotoTexCoord[0] = 0.0f;
							rightPhotoTexCoord[1] = tex_y;
							rightPhotoTexCoord[2] = tex_x;
							rightPhotoTexCoord[3] = tex_y;
							rightPhotoTexCoord[4] = 0.0f;
							rightPhotoTexCoord[5] = 0.0f;
							rightPhotoTexCoord[6] = tex_x;
							rightPhotoTexCoord[7] = 0.0f;

							rightPhotoVertices[0] = rightPhotoPosX;
							rightPhotoVertices[1] = rightPhotoPosY;
							rightPhotoVertices[2] = rightPhotoPosX + rightPhotoSizeX;
							rightPhotoVertices[3] = rightPhotoPosY;
							rightPhotoVertices[4] = rightPhotoPosX;
							rightPhotoVertices[5] = rightPhotoPosY + rightPhotoSizeY;
							rightPhotoVertices[6] = rightPhotoPosX + rightPhotoSizeX;
							rightPhotoVertices[7] = rightPhotoPosY + rightPhotoSizeY;
							break;
						}
					}
				}
				break;
			}
		}
	//}
}

void Photo3DView::setDisplayMode(int mode) {
	m_viewMutex.lock();

	m_mode = mode;

	m_viewMutex.unlock();

	setStale(true);
}

int Photo3DView::loadJPGPhoto(const QString& filename, int* width, int* height, float* tex_x, float* tex_y, unsigned int *tex) {
    GLuint format;

    if (!tex) {
        return EXIT_FAILURE;
    }

    QString imageFilePath = (new QDir())->absoluteFilePath(filename);

    ImageData image = ImageConverter::decode(QUrl::fromLocalFile(imageFilePath));

    //variables to pass to get info
    PixelFormat::Type imageFormat;
    int imageWidth, imageHeight;
    imageWidth = image.width();
    imageHeight = image.height();
    imageFormat = image.format();
    qDebug() << "Image attributes: " << imageFilePath << ":" << imageWidth << ":" << imageHeight << ":" << imageFormat;

    switch (imageFormat)
    {
        case PixelFormat::RGBA_Premultiplied:
            format = GL_RGBA;
            break;
        case PixelFormat::RGBX:
            format = GL_RGBA;
            //format = GL_RGB;
            break;
        default:
        	qDebug() << "Unsupported format (" << (int)imageFormat << ") for photo: " << filename.toAscii();
            return EXIT_FAILURE;
    }

    // photos need to be mapped to a 1920x540 area of the screen so calculate scaling factor to fill this area
    float imageScale = 1.0;
    int adjustX, adjustY, adjustWidth, adjustHeight, adjustSample;

    adjustWidth = imageWidth;
    adjustHeight = imageHeight;
    adjustX = 0;
    adjustY = 0;

    if (imageHeight > 1080) {
    	adjustHeight = 1080;
    	imageScale = 1080.0 / imageHeight;
    	adjustWidth = (int)((float)imageWidth * imageScale);
    	adjustX = (1920 - adjustWidth) / 2;
    	adjustY = (1080 - adjustHeight) / 2;
    } else {
    	adjustX = (1920 - imageWidth) / 2;
    	adjustY = (1080 - imageHeight) / 2;
    }

    qDebug() << "adjust Image attributes: " << ":" << adjustX << ":" << adjustY << ":" << adjustWidth << ":" << adjustHeight;
    qDebug() << "adjust Image attributes: " << ":" << imageScale << ":" << (adjustWidth*adjustWidth);

    if (adjustWidth < 1920) {
    	adjustWidth = 1920;
    	imageScale = 1920.0 / imageWidth;
    	adjustHeight = (int)((float)imageHeight * imageScale);
    	adjustX = (1920 - adjustWidth) / 2;
    	adjustY = (1080 - adjustHeight) / 2;
    }

    qDebug() << "adjust Image attributes: " << ":" << imageScale << ":" << adjustX << ":" << adjustY << ":" << adjustWidth << ":" << adjustHeight;
    qDebug() << "adjust Image attributes: " << ":" << imageScale << ":" << (adjustWidth*adjustWidth);

    // adjust scale to allow for even multiple of bytes for scaling
    int checkWidth, checkHeight;
    if (imageScale < 0.25) {
    	checkWidth = imageWidth / 4;
    	checkHeight = imageHeight / 4;
		adjustSample = 4;
		imageScale = 0.25;
		adjustWidth = 1920;
		adjustHeight = 1080;
		adjustX = (1920 - checkWidth) / 2;
		adjustY = (1080 - checkHeight) / 2;
    } else
    if (imageScale > 0.25 && imageScale < 0.33333333333) {
    	checkWidth = imageWidth / 3;
    	checkHeight = imageHeight / 3;
   		adjustSample = 3;
		imageScale = 0.33333333333;
		adjustWidth = 1920;
		adjustHeight = 1080;
		adjustX = (1920 - checkWidth) / 2;
		adjustY = (1080 - checkHeight) / 2;
    } else
    if (imageScale > 0.33333333333 && imageScale < 0.5) {
    	checkWidth = imageWidth / 2;
    	checkHeight = imageHeight / 2;
		adjustSample = 2;
		imageScale = 0.5;
		adjustWidth = 1920;
		adjustHeight = 1080;
		adjustX = (1920 - checkWidth) / 2;
		adjustY = (1080 - checkHeight) / 2;
    } else
    if (imageScale > 0.5 && imageScale < 1) {
    	checkWidth = imageWidth;
    	checkHeight = imageHeight;
		adjustSample = 1;
		imageScale = 1;
		adjustWidth = 1920;
		adjustHeight = 1080;
		adjustX = (1920 - checkWidth) / 2;
		adjustY = (1080 - checkHeight) / 2;
    }

    qDebug() << "adjust Image attributes: " << ":" << imageScale << ":" << adjustX << ":" << adjustY << ":" << checkWidth << ":" << checkHeight << ":" << adjustWidth << ":" << adjustHeight;
    qDebug() << "adjust Image attributes: " << ":" << imageScale << ":" << (adjustWidth*adjustHeight) << ":" << adjustSample;

    ImageData* adjustImage = &image;
    if (adjustY != 0 || (adjustWidth != imageWidth || adjustHeight != imageHeight)) {
	   adjustImage = new ImageData(bb::PixelFormat::RGBA_Premultiplied, adjustWidth, adjustHeight);

		unsigned char *dstLine = adjustImage->pixels();

		float red, green, blue, alpha;
		int x, y, xi, yi;
		for (yi = 0; yi < adjustImage->height(); yi++) {
			//qDebug() << "Image copy y: " << y;
			unsigned char * dst = dstLine;
			for (xi = 0; xi < adjustImage->width(); xi++) {
				// for sampling not implemented yet, produce a red image
				red = (float)(255);
				green = (float)(0);
				blue = (float)(0);
				alpha = (float)(255);

				x = xi - adjustX;
				y = yi - adjustY;

				if (adjustSample == 1) {
					red = (float)(*(image.pixels() + y*image.bytesPerLine() + x*4 + 0));
					green = (float)(*(image.pixels() + y*image.bytesPerLine() + x*4 + 1));
					blue = (float)(*(image.pixels() + y*image.bytesPerLine() + x*4 + 2));
					alpha = (float)(*(image.pixels() + y*image.bytesPerLine() + x*4 + 3));
				} else if (adjustSample == 2) {
					red =
						0.25*((float)(*(image.pixels() + (y*2+0)*image.bytesPerLine() + (x*2+0)*4 + 0))) +
						0.25*((float)(*(image.pixels() + (y*2+0)*image.bytesPerLine() + (x*2+1)*4 + 0))) +
						0.25*((float)(*(image.pixels() + (y*2+1)*image.bytesPerLine() + (x*2+0)*4 + 0))) +
						0.25*((float)(*(image.pixels() + (y*2+1)*image.bytesPerLine() + (x*2+1)*4 + 0)));
					green =
						0.25*((float)(*(image.pixels() + (y*2+0)*image.bytesPerLine() + (x*2+0)*4 + 1))) +
						0.25*((float)(*(image.pixels() + (y*2+0)*image.bytesPerLine() + (x*2+1)*4 + 1))) +
						0.25*((float)(*(image.pixels() + (y*2+1)*image.bytesPerLine() + (x*2+0)*4 + 1))) +
						0.25*((float)(*(image.pixels() + (y*2+1)*image.bytesPerLine() + (x*2+1)*4 + 1)));
					blue =
						0.25*((float)(*(image.pixels() + (y*2+0)*image.bytesPerLine() + (x*2+0)*4 + 2))) +
						0.25*((float)(*(image.pixels() + (y*2+0)*image.bytesPerLine() + (x*2+1)*4 + 2))) +
						0.25*((float)(*(image.pixels() + (y*2+1)*image.bytesPerLine() + (x*2+0)*4 + 2))) +
						0.25*((float)(*(image.pixels() + (y*2+1)*image.bytesPerLine() + (x*2+1)*4 + 2)));
					alpha =
						0.25*((float)(*(image.pixels() + (y*2+0)*image.bytesPerLine() + (x*2+0)*4 + 3))) +
						0.25*((float)(*(image.pixels() + (y*2+0)*image.bytesPerLine() + (x*2+1)*4 + 3))) +
						0.25*((float)(*(image.pixels() + (y*2+1)*image.bytesPerLine() + (x*2+0)*4 + 3))) +
						0.25*((float)(*(image.pixels() + (y*2+1)*image.bytesPerLine() + (x*2+1)*4 + 3)));
				} else if (adjustSample == 3) {
					red =
						0.11111*((float)(*(image.pixels() + (y*3+0)*image.bytesPerLine() + (x*3+0)*4 + 0))) +
						0.11111*((float)(*(image.pixels() + (y*3+0)*image.bytesPerLine() + (x*3+1)*4 + 0))) +
						0.11111*((float)(*(image.pixels() + (y*3+0)*image.bytesPerLine() + (x*3+2)*4 + 0))) +
						0.11111*((float)(*(image.pixels() + (y*3+1)*image.bytesPerLine() + (x*3+0)*4 + 0))) +
						0.11111*((float)(*(image.pixels() + (y*3+1)*image.bytesPerLine() + (x*3+1)*4 + 0))) +
						0.11111*((float)(*(image.pixels() + (y*3+1)*image.bytesPerLine() + (x*3+2)*4 + 0))) +
						0.11111*((float)(*(image.pixels() + (y*3+2)*image.bytesPerLine() + (x*3+0)*4 + 0))) +
						0.11111*((float)(*(image.pixels() + (y*3+2)*image.bytesPerLine() + (x*3+1)*4 + 0))) +
						0.11111*((float)(*(image.pixels() + (y*3+2)*image.bytesPerLine() + (x*3+2)*4 + 0)));
					green =
						0.11111*((float)(*(image.pixels() + (y*3+0)*image.bytesPerLine() + (x*3+0)*4 + 1))) +
						0.11111*((float)(*(image.pixels() + (y*3+0)*image.bytesPerLine() + (x*3+1)*4 + 1))) +
						0.11111*((float)(*(image.pixels() + (y*3+0)*image.bytesPerLine() + (x*3+2)*4 + 1))) +
						0.11111*((float)(*(image.pixels() + (y*3+1)*image.bytesPerLine() + (x*3+0)*4 + 1))) +
						0.11111*((float)(*(image.pixels() + (y*3+1)*image.bytesPerLine() + (x*3+1)*4 + 1))) +
						0.11111*((float)(*(image.pixels() + (y*3+1)*image.bytesPerLine() + (x*3+2)*4 + 1))) +
						0.11111*((float)(*(image.pixels() + (y*3+2)*image.bytesPerLine() + (x*3+0)*4 + 1))) +
						0.11111*((float)(*(image.pixels() + (y*3+2)*image.bytesPerLine() + (x*3+1)*4 + 1))) +
						0.11111*((float)(*(image.pixels() + (y*3+2)*image.bytesPerLine() + (x*3+2)*4 + 1)));
					blue =
						0.11111*((float)(*(image.pixels() + (y*3+0)*image.bytesPerLine() + (x*3+0)*4 + 2))) +
						0.11111*((float)(*(image.pixels() + (y*3+0)*image.bytesPerLine() + (x*3+1)*4 + 2))) +
						0.11111*((float)(*(image.pixels() + (y*3+0)*image.bytesPerLine() + (x*3+2)*4 + 2))) +
						0.11111*((float)(*(image.pixels() + (y*3+1)*image.bytesPerLine() + (x*3+0)*4 + 2))) +
						0.11111*((float)(*(image.pixels() + (y*3+1)*image.bytesPerLine() + (x*3+1)*4 + 2))) +
						0.11111*((float)(*(image.pixels() + (y*3+1)*image.bytesPerLine() + (x*3+2)*4 + 2))) +
						0.11111*((float)(*(image.pixels() + (y*3+2)*image.bytesPerLine() + (x*3+0)*4 + 2))) +
						0.11111*((float)(*(image.pixels() + (y*3+2)*image.bytesPerLine() + (x*3+1)*4 + 2))) +
						0.11111*((float)(*(image.pixels() + (y*3+2)*image.bytesPerLine() + (x*3+2)*4 + 2)));
					alpha =
						0.11111*((float)(*(image.pixels() + (y*3+0)*image.bytesPerLine() + (x*3+0)*4 + 3))) +
						0.11111*((float)(*(image.pixels() + (y*3+0)*image.bytesPerLine() + (x*3+1)*4 + 3))) +
						0.11111*((float)(*(image.pixels() + (y*3+0)*image.bytesPerLine() + (x*3+2)*4 + 3))) +
						0.11111*((float)(*(image.pixels() + (y*3+1)*image.bytesPerLine() + (x*3+0)*4 + 3))) +
						0.11111*((float)(*(image.pixels() + (y*3+1)*image.bytesPerLine() + (x*3+1)*4 + 3))) +
						0.11111*((float)(*(image.pixels() + (y*3+1)*image.bytesPerLine() + (x*3+2)*4 + 3))) +
						0.11111*((float)(*(image.pixels() + (y*3+2)*image.bytesPerLine() + (x*3+0)*4 + 3))) +
						0.11111*((float)(*(image.pixels() + (y*3+2)*image.bytesPerLine() + (x*3+1)*4 + 3))) +
						0.11111*((float)(*(image.pixels() + (y*3+2)*image.bytesPerLine() + (x*3+2)*4 + 3)));
				} else if (adjustSample == 4) {
					red =
						0.0625*((float)(*(image.pixels() + (y*4+0)*image.bytesPerLine() + (x*4+0)*4 + 0))) +
						0.0625*((float)(*(image.pixels() + (y*4+0)*image.bytesPerLine() + (x*4+1)*4 + 0))) +
						0.0625*((float)(*(image.pixels() + (y*4+0)*image.bytesPerLine() + (x*4+2)*4 + 0))) +
						0.0625*((float)(*(image.pixels() + (y*4+0)*image.bytesPerLine() + (x*4+3)*4 + 0))) +
						0.0625*((float)(*(image.pixels() + (y*4+1)*image.bytesPerLine() + (x*4+0)*4 + 0))) +
						0.0625*((float)(*(image.pixels() + (y*4+1)*image.bytesPerLine() + (x*4+1)*4 + 0))) +
						0.0625*((float)(*(image.pixels() + (y*4+1)*image.bytesPerLine() + (x*4+2)*4 + 0))) +
						0.0625*((float)(*(image.pixels() + (y*4+1)*image.bytesPerLine() + (x*4+3)*4 + 0))) +
						0.0625*((float)(*(image.pixels() + (y*4+2)*image.bytesPerLine() + (x*4+0)*4 + 0))) +
						0.0625*((float)(*(image.pixels() + (y*4+2)*image.bytesPerLine() + (x*4+1)*4 + 0))) +
						0.0625*((float)(*(image.pixels() + (y*4+2)*image.bytesPerLine() + (x*4+2)*4 + 0))) +
						0.0625*((float)(*(image.pixels() + (y*4+2)*image.bytesPerLine() + (x*4+3)*4 + 0))) +
						0.0625*((float)(*(image.pixels() + (y*4+3)*image.bytesPerLine() + (x*4+0)*4 + 0))) +
						0.0625*((float)(*(image.pixels() + (y*4+3)*image.bytesPerLine() + (x*4+1)*4 + 0))) +
						0.0625*((float)(*(image.pixels() + (y*4+3)*image.bytesPerLine() + (x*4+2)*4 + 0))) +
						0.0625*((float)(*(image.pixels() + (y*4+3)*image.bytesPerLine() + (x*4+3)*4 + 0)));
					green =
						0.0625*((float)(*(image.pixels() + (y*4+0)*image.bytesPerLine() + (x*4+0)*4 + 1))) +
						0.0625*((float)(*(image.pixels() + (y*4+0)*image.bytesPerLine() + (x*4+1)*4 + 1))) +
						0.0625*((float)(*(image.pixels() + (y*4+0)*image.bytesPerLine() + (x*4+2)*4 + 1))) +
						0.0625*((float)(*(image.pixels() + (y*4+0)*image.bytesPerLine() + (x*4+3)*4 + 1))) +
						0.0625*((float)(*(image.pixels() + (y*4+1)*image.bytesPerLine() + (x*4+0)*4 + 1))) +
						0.0625*((float)(*(image.pixels() + (y*4+1)*image.bytesPerLine() + (x*4+1)*4 + 1))) +
						0.0625*((float)(*(image.pixels() + (y*4+1)*image.bytesPerLine() + (x*4+2)*4 + 1))) +
						0.0625*((float)(*(image.pixels() + (y*4+1)*image.bytesPerLine() + (x*4+3)*4 + 1))) +
						0.0625*((float)(*(image.pixels() + (y*4+2)*image.bytesPerLine() + (x*4+0)*4 + 1))) +
						0.0625*((float)(*(image.pixels() + (y*4+2)*image.bytesPerLine() + (x*4+1)*4 + 1))) +
						0.0625*((float)(*(image.pixels() + (y*4+2)*image.bytesPerLine() + (x*4+2)*4 + 1))) +
						0.0625*((float)(*(image.pixels() + (y*4+2)*image.bytesPerLine() + (x*4+3)*4 + 1))) +
						0.0625*((float)(*(image.pixels() + (y*4+3)*image.bytesPerLine() + (x*4+0)*4 + 1))) +
						0.0625*((float)(*(image.pixels() + (y*4+3)*image.bytesPerLine() + (x*4+1)*4 + 1))) +
						0.0625*((float)(*(image.pixels() + (y*4+3)*image.bytesPerLine() + (x*4+2)*4 + 1))) +
						0.0625*((float)(*(image.pixels() + (y*4+3)*image.bytesPerLine() + (x*4+3)*4 + 1)));
					blue =
						0.0625*((float)(*(image.pixels() + (y*4+0)*image.bytesPerLine() + (x*4+0)*4 + 2))) +
						0.0625*((float)(*(image.pixels() + (y*4+0)*image.bytesPerLine() + (x*4+1)*4 + 2))) +
						0.0625*((float)(*(image.pixels() + (y*4+0)*image.bytesPerLine() + (x*4+2)*4 + 2))) +
						0.0625*((float)(*(image.pixels() + (y*4+0)*image.bytesPerLine() + (x*4+3)*4 + 2))) +
						0.0625*((float)(*(image.pixels() + (y*4+1)*image.bytesPerLine() + (x*4+0)*4 + 2))) +
						0.0625*((float)(*(image.pixels() + (y*4+1)*image.bytesPerLine() + (x*4+1)*4 + 2))) +
						0.0625*((float)(*(image.pixels() + (y*4+1)*image.bytesPerLine() + (x*4+2)*4 + 2))) +
						0.0625*((float)(*(image.pixels() + (y*4+1)*image.bytesPerLine() + (x*4+3)*4 + 2))) +
						0.0625*((float)(*(image.pixels() + (y*4+2)*image.bytesPerLine() + (x*4+0)*4 + 2))) +
						0.0625*((float)(*(image.pixels() + (y*4+2)*image.bytesPerLine() + (x*4+1)*4 + 2))) +
						0.0625*((float)(*(image.pixels() + (y*4+2)*image.bytesPerLine() + (x*4+2)*4 + 2))) +
						0.0625*((float)(*(image.pixels() + (y*4+2)*image.bytesPerLine() + (x*4+3)*4 + 2))) +
						0.0625*((float)(*(image.pixels() + (y*4+3)*image.bytesPerLine() + (x*4+0)*4 + 2))) +
						0.0625*((float)(*(image.pixels() + (y*4+3)*image.bytesPerLine() + (x*4+1)*4 + 2))) +
						0.0625*((float)(*(image.pixels() + (y*4+3)*image.bytesPerLine() + (x*4+2)*4 + 2))) +
						0.0625*((float)(*(image.pixels() + (y*4+3)*image.bytesPerLine() + (x*4+3)*4 + 2)));
					alpha =
						0.0625*((float)(*(image.pixels() + (y*4+0)*image.bytesPerLine() + (x*4+0)*4 + 3))) +
						0.0625*((float)(*(image.pixels() + (y*4+0)*image.bytesPerLine() + (x*4+1)*4 + 3))) +
						0.0625*((float)(*(image.pixels() + (y*4+0)*image.bytesPerLine() + (x*4+2)*4 + 3))) +
						0.0625*((float)(*(image.pixels() + (y*4+0)*image.bytesPerLine() + (x*4+3)*4 + 3))) +
						0.0625*((float)(*(image.pixels() + (y*4+1)*image.bytesPerLine() + (x*4+0)*4 + 3))) +
						0.0625*((float)(*(image.pixels() + (y*4+1)*image.bytesPerLine() + (x*4+1)*4 + 3))) +
						0.0625*((float)(*(image.pixels() + (y*4+1)*image.bytesPerLine() + (x*4+2)*4 + 3))) +
						0.0625*((float)(*(image.pixels() + (y*4+1)*image.bytesPerLine() + (x*4+3)*4 + 3))) +
						0.0625*((float)(*(image.pixels() + (y*4+2)*image.bytesPerLine() + (x*4+0)*4 + 3))) +
						0.0625*((float)(*(image.pixels() + (y*4+2)*image.bytesPerLine() + (x*4+1)*4 + 3))) +
						0.0625*((float)(*(image.pixels() + (y*4+2)*image.bytesPerLine() + (x*4+2)*4 + 3))) +
						0.0625*((float)(*(image.pixels() + (y*4+2)*image.bytesPerLine() + (x*4+3)*4 + 3))) +
						0.0625*((float)(*(image.pixels() + (y*4+3)*image.bytesPerLine() + (x*4+0)*4 + 3))) +
						0.0625*((float)(*(image.pixels() + (y*4+3)*image.bytesPerLine() + (x*4+1)*4 + 3))) +
						0.0625*((float)(*(image.pixels() + (y*4+3)*image.bytesPerLine() + (x*4+2)*4 + 3))) +
						0.0625*((float)(*(image.pixels() + (y*4+3)*image.bytesPerLine() + (x*4+3)*4 + 3)));
				}

				if (imageFormat == PixelFormat::RGBX) {
					alpha = 255.0f;
				}

				*dst++ = (char)(red);
				*dst++ = (char)(green);
				*dst++ = (char)(blue);
				*dst++ = (char)(alpha);

			}
			dstLine += adjustImage->bytesPerLine();
		}
	}

    int texWidth, texHeight;

    texWidth = nextp2(adjustWidth);
    texHeight = nextp2(adjustHeight);

    getGLContext();

    if ((*tex) > 0) {
        qDebug() << "glBindTexture: " << 0;
        glBindTexture(GL_TEXTURE_2D, 0);
    } else {
		qDebug() << "glGenTextures: " << tex << *tex;
		glGenTextures(1, tex);
		qDebug() << "glGenTextures: " << tex << *tex;
    }

    qDebug() << "glBindTexture: " << tex << *tex;
    glBindTexture(GL_TEXTURE_2D, (*tex));
    qDebug() << "glBindTexture: " << tex << *tex;

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    glTexImage2D(GL_TEXTURE_2D, 0, format, texWidth, texHeight, 0, format, GL_UNSIGNED_BYTE, NULL);
    glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, adjustWidth, adjustHeight, format, GL_UNSIGNED_BYTE, adjustImage->pixels());

    GLint err = glGetError();

    //clean up memory and close stuff
    if (adjustImage != &image) {
    	delete adjustImage;
    }

   if (err == 0) {
        //Return physical with and height of texture if pointers are not null
        if(width) {
            *width = adjustWidth;
        }
        if (height) {
            *height = adjustHeight;
        }
        //Return modified texture coordinates if pointers are not null
        if(tex_x) {
            *tex_x = ((float) adjustWidth - 0.5f) / ((float)texWidth);
        }
        if(tex_y) {
            *tex_y = ((float) adjustHeight - 0.5f) / ((float)texHeight);
        }
        return EXIT_SUCCESS;
    } else {
        fprintf(stderr, "GL error %i \n", err);
        return EXIT_FAILURE;
    }

}

int Photo3DView::loadJPGPhotos(const QString& filename, const QString& filename1, int* width, int* height, float* tex_x, float* tex_y, unsigned int *tex) {
    GLuint format;

    if (!tex) {
        return EXIT_FAILURE;
    }

    QString imageFilePath = (new QDir())->absoluteFilePath(filename);

    ImageData image = ImageConverter::decode(QUrl::fromLocalFile(imageFilePath));

    imageFilePath = (new QDir())->absoluteFilePath(filename1);

    ImageData image1 = ImageConverter::decode(QUrl::fromLocalFile(imageFilePath));

    //variables to pass to get info
    PixelFormat::Type imageFormat;
    int imageWidth, imageHeight;
    imageWidth = image.width();
    imageHeight = image.height();
    imageFormat = image.format();
    qDebug() << "Image attributes: " << imageFilePath << ":" << imageWidth << ":" << imageHeight << ":" << imageFormat;

    switch (imageFormat)
    {
        case PixelFormat::RGBA_Premultiplied:
            format = GL_RGBA;
            break;
        case PixelFormat::RGBX:
            format = GL_RGBA;
            //format = GL_RGB;
            break;
        default:
        	qDebug() << "Unsupported format (" << (int)imageFormat << ") for photo: " << filename.toAscii();
            return EXIT_FAILURE;
    }

    // photos need to be mapped to a 1920x540 area of the screen so calculate scaling factor to fill this area
    float imageScale = 1.0;
    int adjustX, adjustY, adjustWidth, adjustHeight, adjustSample;

    adjustWidth = imageWidth;
    adjustHeight = imageHeight;
    adjustX = 0;
    adjustY = 0;

    if (imageHeight > 1080) {
    	adjustHeight = 1080;
    	imageScale = 1080.0 / imageHeight;
    	adjustWidth = (int)((float)imageWidth * imageScale);
    	adjustX = (1920 - adjustWidth) / 2;
    	adjustY = (1080 - adjustHeight) / 2;
    } else {
    	adjustX = (1920 - imageWidth) / 2;
    	adjustY = (1080 - imageHeight) / 2;
    }

    qDebug() << "adjust Image attributes: " << ":" << adjustX << ":" << adjustY << ":" << adjustWidth << ":" << adjustHeight;
    qDebug() << "adjust Image attributes: " << ":" << imageScale << ":" << (adjustWidth*adjustWidth);

    if (adjustWidth < 1920) {
    	adjustWidth = 1920;
    	imageScale = 1920.0 / imageWidth;
    	adjustHeight = (int)((float)imageHeight * imageScale);
    	adjustX = (1920 - adjustWidth) / 2;
    	adjustY = (1080 - adjustHeight) / 2;
    }

    qDebug() << "adjust Image attributes: " << ":" << imageScale << ":" << adjustX << ":" << adjustY << ":" << adjustWidth << ":" << adjustHeight;
    qDebug() << "adjust Image attributes: " << ":" << imageScale << ":" << (adjustWidth*adjustWidth);

    // adjust scale to allow for even multiple of bytes for scaling
    int checkWidth, checkHeight;
    if (imageScale < 0.25) {
    	checkWidth = imageWidth / 4;
    	checkHeight = imageHeight / 4;
		adjustSample = 4;
		imageScale = 0.25;
		adjustWidth = 1920;
		adjustHeight = 1080;
		adjustX = (1920 - checkWidth) / 2;
		adjustY = (1080 - checkHeight) / 2;
    } else
    if (imageScale > 0.25 && imageScale < 0.33333333333) {
    	checkWidth = imageWidth / 3;
    	checkHeight = imageHeight / 3;
   		adjustSample = 3;
		imageScale = 0.33333333333;
		adjustWidth = 1920;
		adjustHeight = 1080;
		adjustX = (1920 - checkWidth) / 2;
		adjustY = (1080 - checkHeight) / 2;
    } else
    if (imageScale > 0.33333333333 && imageScale < 0.5) {
    	checkWidth = imageWidth / 2;
    	checkHeight = imageHeight / 2;
		adjustSample = 2;
		imageScale = 0.5;
		adjustWidth = 1920;
		adjustHeight = 1080;
		adjustX = (1920 - checkWidth) / 2;
		adjustY = (1080 - checkHeight) / 2;
    } else
    if (imageScale > 0.5 && imageScale < 1) {
    	checkWidth = imageWidth;
    	checkHeight = imageHeight;
		adjustSample = 1;
		imageScale = 1;
		adjustWidth = 1920;
		adjustHeight = 1080;
		adjustX = (1920 - checkWidth) / 2;
		adjustY = (1080 - checkHeight) / 2;
    }

    qDebug() << "adjust Image attributes: " << ":" << imageScale << ":" << adjustX << ":" << adjustY << ":" << checkWidth << ":" << checkHeight << ":" << adjustWidth << ":" << adjustHeight;
    qDebug() << "adjust Image attributes: " << ":" << imageScale << ":" << (adjustWidth*adjustHeight) << ":" << adjustSample;

    ImageData* adjustImage = &image;
    if (adjustY != 0 || (adjustWidth != imageWidth || adjustHeight != imageHeight)) {
	   adjustImage = new ImageData(bb::PixelFormat::RGBA_Premultiplied, adjustWidth, adjustHeight);

		unsigned char *dstLine = adjustImage->pixels();

		float red, green, blue, alpha;
		int x, y, xi, yi;
		for (yi = 0; yi < adjustImage->height(); yi++) {
			//qDebug() << "Image copy y: " << y;
			unsigned char * dst = dstLine;
			for (xi = 0; xi < adjustImage->width(); xi++) {
				// for sampling not implemented yet, produce a red image
				red = (float)(255);
				green = (float)(0);
				blue = (float)(0);
				alpha = (float)(255);

				x = xi - adjustX;
				y = yi - adjustY;

				if (adjustSample == 1) {
					red = (float)(*(image.pixels() + y*image.bytesPerLine() + x*4 + 0));
					green = (float)(*(image.pixels() + y*image.bytesPerLine() + x*4 + 1));
					blue = (float)(*(image.pixels() + y*image.bytesPerLine() + x*4 + 2));
					alpha = (float)(*(image.pixels() + y*image.bytesPerLine() + x*4 + 3));
				} else if (adjustSample == 2) {
					red =
						0.25*((float)(*(image.pixels() + (y*2+0)*image.bytesPerLine() + (x*2+0)*4 + 0))) +
						0.25*((float)(*(image.pixels() + (y*2+0)*image.bytesPerLine() + (x*2+1)*4 + 0))) +
						0.25*((float)(*(image.pixels() + (y*2+1)*image.bytesPerLine() + (x*2+0)*4 + 0))) +
						0.25*((float)(*(image.pixels() + (y*2+1)*image.bytesPerLine() + (x*2+1)*4 + 0)));
					green =
						0.25*((float)(*(image.pixels() + (y*2+0)*image.bytesPerLine() + (x*2+0)*4 + 1))) +
						0.25*((float)(*(image.pixels() + (y*2+0)*image.bytesPerLine() + (x*2+1)*4 + 1))) +
						0.25*((float)(*(image.pixels() + (y*2+1)*image.bytesPerLine() + (x*2+0)*4 + 1))) +
						0.25*((float)(*(image.pixels() + (y*2+1)*image.bytesPerLine() + (x*2+1)*4 + 1)));
					blue =
						0.25*((float)(*(image.pixels() + (y*2+0)*image.bytesPerLine() + (x*2+0)*4 + 2))) +
						0.25*((float)(*(image.pixels() + (y*2+0)*image.bytesPerLine() + (x*2+1)*4 + 2))) +
						0.25*((float)(*(image.pixels() + (y*2+1)*image.bytesPerLine() + (x*2+0)*4 + 2))) +
						0.25*((float)(*(image.pixels() + (y*2+1)*image.bytesPerLine() + (x*2+1)*4 + 2)));
					alpha =
						0.25*((float)(*(image.pixels() + (y*2+0)*image.bytesPerLine() + (x*2+0)*4 + 3))) +
						0.25*((float)(*(image.pixels() + (y*2+0)*image.bytesPerLine() + (x*2+1)*4 + 3))) +
						0.25*((float)(*(image.pixels() + (y*2+1)*image.bytesPerLine() + (x*2+0)*4 + 3))) +
						0.25*((float)(*(image.pixels() + (y*2+1)*image.bytesPerLine() + (x*2+1)*4 + 3)));
				} else if (adjustSample == 3) {
					red =
						0.11111*((float)(*(image.pixels() + (y*3+0)*image.bytesPerLine() + (x*3+0)*4 + 0))) +
						0.11111*((float)(*(image.pixels() + (y*3+0)*image.bytesPerLine() + (x*3+1)*4 + 0))) +
						0.11111*((float)(*(image.pixels() + (y*3+0)*image.bytesPerLine() + (x*3+2)*4 + 0))) +
						0.11111*((float)(*(image.pixels() + (y*3+1)*image.bytesPerLine() + (x*3+0)*4 + 0))) +
						0.11111*((float)(*(image.pixels() + (y*3+1)*image.bytesPerLine() + (x*3+1)*4 + 0))) +
						0.11111*((float)(*(image.pixels() + (y*3+1)*image.bytesPerLine() + (x*3+2)*4 + 0))) +
						0.11111*((float)(*(image.pixels() + (y*3+2)*image.bytesPerLine() + (x*3+0)*4 + 0))) +
						0.11111*((float)(*(image.pixels() + (y*3+2)*image.bytesPerLine() + (x*3+1)*4 + 0))) +
						0.11111*((float)(*(image.pixels() + (y*3+2)*image.bytesPerLine() + (x*3+2)*4 + 0)));
					green =
						0.11111*((float)(*(image.pixels() + (y*3+0)*image.bytesPerLine() + (x*3+0)*4 + 1))) +
						0.11111*((float)(*(image.pixels() + (y*3+0)*image.bytesPerLine() + (x*3+1)*4 + 1))) +
						0.11111*((float)(*(image.pixels() + (y*3+0)*image.bytesPerLine() + (x*3+2)*4 + 1))) +
						0.11111*((float)(*(image.pixels() + (y*3+1)*image.bytesPerLine() + (x*3+0)*4 + 1))) +
						0.11111*((float)(*(image.pixels() + (y*3+1)*image.bytesPerLine() + (x*3+1)*4 + 1))) +
						0.11111*((float)(*(image.pixels() + (y*3+1)*image.bytesPerLine() + (x*3+2)*4 + 1))) +
						0.11111*((float)(*(image.pixels() + (y*3+2)*image.bytesPerLine() + (x*3+0)*4 + 1))) +
						0.11111*((float)(*(image.pixels() + (y*3+2)*image.bytesPerLine() + (x*3+1)*4 + 1))) +
						0.11111*((float)(*(image.pixels() + (y*3+2)*image.bytesPerLine() + (x*3+2)*4 + 1)));
					blue =
						0.11111*((float)(*(image.pixels() + (y*3+0)*image.bytesPerLine() + (x*3+0)*4 + 2))) +
						0.11111*((float)(*(image.pixels() + (y*3+0)*image.bytesPerLine() + (x*3+1)*4 + 2))) +
						0.11111*((float)(*(image.pixels() + (y*3+0)*image.bytesPerLine() + (x*3+2)*4 + 2))) +
						0.11111*((float)(*(image.pixels() + (y*3+1)*image.bytesPerLine() + (x*3+0)*4 + 2))) +
						0.11111*((float)(*(image.pixels() + (y*3+1)*image.bytesPerLine() + (x*3+1)*4 + 2))) +
						0.11111*((float)(*(image.pixels() + (y*3+1)*image.bytesPerLine() + (x*3+2)*4 + 2))) +
						0.11111*((float)(*(image.pixels() + (y*3+2)*image.bytesPerLine() + (x*3+0)*4 + 2))) +
						0.11111*((float)(*(image.pixels() + (y*3+2)*image.bytesPerLine() + (x*3+1)*4 + 2))) +
						0.11111*((float)(*(image.pixels() + (y*3+2)*image.bytesPerLine() + (x*3+2)*4 + 2)));
					alpha =
						0.11111*((float)(*(image.pixels() + (y*3+0)*image.bytesPerLine() + (x*3+0)*4 + 3))) +
						0.11111*((float)(*(image.pixels() + (y*3+0)*image.bytesPerLine() + (x*3+1)*4 + 3))) +
						0.11111*((float)(*(image.pixels() + (y*3+0)*image.bytesPerLine() + (x*3+2)*4 + 3))) +
						0.11111*((float)(*(image.pixels() + (y*3+1)*image.bytesPerLine() + (x*3+0)*4 + 3))) +
						0.11111*((float)(*(image.pixels() + (y*3+1)*image.bytesPerLine() + (x*3+1)*4 + 3))) +
						0.11111*((float)(*(image.pixels() + (y*3+1)*image.bytesPerLine() + (x*3+2)*4 + 3))) +
						0.11111*((float)(*(image.pixels() + (y*3+2)*image.bytesPerLine() + (x*3+0)*4 + 3))) +
						0.11111*((float)(*(image.pixels() + (y*3+2)*image.bytesPerLine() + (x*3+1)*4 + 3))) +
						0.11111*((float)(*(image.pixels() + (y*3+2)*image.bytesPerLine() + (x*3+2)*4 + 3)));
				} else if (adjustSample == 4) {
					red =
						0.0625*((float)(*(image.pixels() + (y*4+0)*image.bytesPerLine() + (x*4+0)*4 + 0))) +
						0.0625*((float)(*(image.pixels() + (y*4+0)*image.bytesPerLine() + (x*4+1)*4 + 0))) +
						0.0625*((float)(*(image.pixels() + (y*4+0)*image.bytesPerLine() + (x*4+2)*4 + 0))) +
						0.0625*((float)(*(image.pixels() + (y*4+0)*image.bytesPerLine() + (x*4+3)*4 + 0))) +
						0.0625*((float)(*(image.pixels() + (y*4+1)*image.bytesPerLine() + (x*4+0)*4 + 0))) +
						0.0625*((float)(*(image.pixels() + (y*4+1)*image.bytesPerLine() + (x*4+1)*4 + 0))) +
						0.0625*((float)(*(image.pixels() + (y*4+1)*image.bytesPerLine() + (x*4+2)*4 + 0))) +
						0.0625*((float)(*(image.pixels() + (y*4+1)*image.bytesPerLine() + (x*4+3)*4 + 0))) +
						0.0625*((float)(*(image.pixels() + (y*4+2)*image.bytesPerLine() + (x*4+0)*4 + 0))) +
						0.0625*((float)(*(image.pixels() + (y*4+2)*image.bytesPerLine() + (x*4+1)*4 + 0))) +
						0.0625*((float)(*(image.pixels() + (y*4+2)*image.bytesPerLine() + (x*4+2)*4 + 0))) +
						0.0625*((float)(*(image.pixels() + (y*4+2)*image.bytesPerLine() + (x*4+3)*4 + 0))) +
						0.0625*((float)(*(image.pixels() + (y*4+3)*image.bytesPerLine() + (x*4+0)*4 + 0))) +
						0.0625*((float)(*(image.pixels() + (y*4+3)*image.bytesPerLine() + (x*4+1)*4 + 0))) +
						0.0625*((float)(*(image.pixels() + (y*4+3)*image.bytesPerLine() + (x*4+2)*4 + 0))) +
						0.0625*((float)(*(image.pixels() + (y*4+3)*image.bytesPerLine() + (x*4+3)*4 + 0)));
					green =
						0.0625*((float)(*(image.pixels() + (y*4+0)*image.bytesPerLine() + (x*4+0)*4 + 1))) +
						0.0625*((float)(*(image.pixels() + (y*4+0)*image.bytesPerLine() + (x*4+1)*4 + 1))) +
						0.0625*((float)(*(image.pixels() + (y*4+0)*image.bytesPerLine() + (x*4+2)*4 + 1))) +
						0.0625*((float)(*(image.pixels() + (y*4+0)*image.bytesPerLine() + (x*4+3)*4 + 1))) +
						0.0625*((float)(*(image.pixels() + (y*4+1)*image.bytesPerLine() + (x*4+0)*4 + 1))) +
						0.0625*((float)(*(image.pixels() + (y*4+1)*image.bytesPerLine() + (x*4+1)*4 + 1))) +
						0.0625*((float)(*(image.pixels() + (y*4+1)*image.bytesPerLine() + (x*4+2)*4 + 1))) +
						0.0625*((float)(*(image.pixels() + (y*4+1)*image.bytesPerLine() + (x*4+3)*4 + 1))) +
						0.0625*((float)(*(image.pixels() + (y*4+2)*image.bytesPerLine() + (x*4+0)*4 + 1))) +
						0.0625*((float)(*(image.pixels() + (y*4+2)*image.bytesPerLine() + (x*4+1)*4 + 1))) +
						0.0625*((float)(*(image.pixels() + (y*4+2)*image.bytesPerLine() + (x*4+2)*4 + 1))) +
						0.0625*((float)(*(image.pixels() + (y*4+2)*image.bytesPerLine() + (x*4+3)*4 + 1))) +
						0.0625*((float)(*(image.pixels() + (y*4+3)*image.bytesPerLine() + (x*4+0)*4 + 1))) +
						0.0625*((float)(*(image.pixels() + (y*4+3)*image.bytesPerLine() + (x*4+1)*4 + 1))) +
						0.0625*((float)(*(image.pixels() + (y*4+3)*image.bytesPerLine() + (x*4+2)*4 + 1))) +
						0.0625*((float)(*(image.pixels() + (y*4+3)*image.bytesPerLine() + (x*4+3)*4 + 1)));
					blue =
						0.0625*((float)(*(image.pixels() + (y*4+0)*image.bytesPerLine() + (x*4+0)*4 + 2))) +
						0.0625*((float)(*(image.pixels() + (y*4+0)*image.bytesPerLine() + (x*4+1)*4 + 2))) +
						0.0625*((float)(*(image.pixels() + (y*4+0)*image.bytesPerLine() + (x*4+2)*4 + 2))) +
						0.0625*((float)(*(image.pixels() + (y*4+0)*image.bytesPerLine() + (x*4+3)*4 + 2))) +
						0.0625*((float)(*(image.pixels() + (y*4+1)*image.bytesPerLine() + (x*4+0)*4 + 2))) +
						0.0625*((float)(*(image.pixels() + (y*4+1)*image.bytesPerLine() + (x*4+1)*4 + 2))) +
						0.0625*((float)(*(image.pixels() + (y*4+1)*image.bytesPerLine() + (x*4+2)*4 + 2))) +
						0.0625*((float)(*(image.pixels() + (y*4+1)*image.bytesPerLine() + (x*4+3)*4 + 2))) +
						0.0625*((float)(*(image.pixels() + (y*4+2)*image.bytesPerLine() + (x*4+0)*4 + 2))) +
						0.0625*((float)(*(image.pixels() + (y*4+2)*image.bytesPerLine() + (x*4+1)*4 + 2))) +
						0.0625*((float)(*(image.pixels() + (y*4+2)*image.bytesPerLine() + (x*4+2)*4 + 2))) +
						0.0625*((float)(*(image.pixels() + (y*4+2)*image.bytesPerLine() + (x*4+3)*4 + 2))) +
						0.0625*((float)(*(image.pixels() + (y*4+3)*image.bytesPerLine() + (x*4+0)*4 + 2))) +
						0.0625*((float)(*(image.pixels() + (y*4+3)*image.bytesPerLine() + (x*4+1)*4 + 2))) +
						0.0625*((float)(*(image.pixels() + (y*4+3)*image.bytesPerLine() + (x*4+2)*4 + 2))) +
						0.0625*((float)(*(image.pixels() + (y*4+3)*image.bytesPerLine() + (x*4+3)*4 + 2)));
					alpha =
						0.0625*((float)(*(image.pixels() + (y*4+0)*image.bytesPerLine() + (x*4+0)*4 + 3))) +
						0.0625*((float)(*(image.pixels() + (y*4+0)*image.bytesPerLine() + (x*4+1)*4 + 3))) +
						0.0625*((float)(*(image.pixels() + (y*4+0)*image.bytesPerLine() + (x*4+2)*4 + 3))) +
						0.0625*((float)(*(image.pixels() + (y*4+0)*image.bytesPerLine() + (x*4+3)*4 + 3))) +
						0.0625*((float)(*(image.pixels() + (y*4+1)*image.bytesPerLine() + (x*4+0)*4 + 3))) +
						0.0625*((float)(*(image.pixels() + (y*4+1)*image.bytesPerLine() + (x*4+1)*4 + 3))) +
						0.0625*((float)(*(image.pixels() + (y*4+1)*image.bytesPerLine() + (x*4+2)*4 + 3))) +
						0.0625*((float)(*(image.pixels() + (y*4+1)*image.bytesPerLine() + (x*4+3)*4 + 3))) +
						0.0625*((float)(*(image.pixels() + (y*4+2)*image.bytesPerLine() + (x*4+0)*4 + 3))) +
						0.0625*((float)(*(image.pixels() + (y*4+2)*image.bytesPerLine() + (x*4+1)*4 + 3))) +
						0.0625*((float)(*(image.pixels() + (y*4+2)*image.bytesPerLine() + (x*4+2)*4 + 3))) +
						0.0625*((float)(*(image.pixels() + (y*4+2)*image.bytesPerLine() + (x*4+3)*4 + 3))) +
						0.0625*((float)(*(image.pixels() + (y*4+3)*image.bytesPerLine() + (x*4+0)*4 + 3))) +
						0.0625*((float)(*(image.pixels() + (y*4+3)*image.bytesPerLine() + (x*4+1)*4 + 3))) +
						0.0625*((float)(*(image.pixels() + (y*4+3)*image.bytesPerLine() + (x*4+2)*4 + 3))) +
						0.0625*((float)(*(image.pixels() + (y*4+3)*image.bytesPerLine() + (x*4+3)*4 + 3)));
				}

				if (imageFormat == PixelFormat::RGBX) {
					alpha = 255.0f;
				}

				switch(m_mode) {
					case VIEW_3D_TYPE_ANAGLYPH_RED_CYAN:
					case VIEW_3D_TYPE_ANAGLYPH_RED_GREEN:
						*dst++ = (char)(red);
						//*dst++ = (char)(0);
						//*dst++ = (char)(green);
						*dst++ = (char)(0);
						//*dst++ = (char)(blue);
						*dst++ = (char)(0);
						*dst++ = (char)(alpha);
						//*dst++ = (char)(0);
						break;

					case VIEW_3D_TYPE_ANAGLYPH_CYAN_RED:
						//*dst++ = (char)(red);
						*dst++ = (char)(0);
						*dst++ = (char)(green);
						//*dst++ = (char)(0);
						*dst++ = (char)(blue);
						//*dst++ = (char)(0);
						*dst++ = (char)(alpha);
						//*dst++ = (char)(0);
						break;

					case VIEW_3D_TYPE_ANAGLYPH_GREEN_RED:
						//*dst++ = (char)(red);
						*dst++ = (char)(0);
						*dst++ = (char)(green);
						//*dst++ = (char)(0);
						//*dst++ = (char)(blue);
						*dst++ = (char)(0);
						*dst++ = (char)(alpha);
						//*dst++ = (char)(0);
						break;

					case VIEW_3D_TYPE_ANAGLYPH_BLUE_YELLOW:
						//*dst++ = (char)(red);
						*dst++ = (char)(0);
						//*dst++ = (char)(green);
						*dst++ = (char)(0);
						*dst++ = (char)(blue);
						//*dst++ = (char)(0);
						*dst++ = (char)(alpha);
						//*dst++ = (char)(0);
						break;

					case VIEW_3D_TYPE_ANAGLYPH_YELLOW_BLUE:
						*dst++ = (char)(red);
						//*dst++ = (char)(0);
						*dst++ = (char)(green);
						//*dst++ = (char)(0);
						//*dst++ = (char)(blue);
						*dst++ = (char)(0);
						*dst++ = (char)(alpha);
						//*dst++ = (char)(0);
						break;

					case VIEW_3D_TYPE_ANAGLYPH_CLEAR_VIOLET:
						*dst++ = (char)(red / 2);
						//*dst++ = (char)(0);
						*dst++ = (char)(green);
						//*dst++ = (char)(0);
						//*dst++ = (char)(blue);
						*dst++ = (char)(0);
						*dst++ = (char)(alpha);
						//*dst++ = (char)(0);
						break;
				}

			}
			dstLine += adjustImage->bytesPerLine();
		}
	}


    //variables to pass to get info
    imageWidth = image1.width();
    imageHeight = image1.height();
    imageFormat = image1.format();
    qDebug() << "Image attributes: " << imageFilePath << ":" << imageWidth << ":" << imageHeight << ":" << imageFormat;

    switch (imageFormat)
    {
        case PixelFormat::RGBA_Premultiplied:
            format = GL_RGBA;
            break;
        case PixelFormat::RGBX:
            format = GL_RGBA;
            //format = GL_RGB;
            break;
        default:
        	qDebug() << "Unsupported format (" << (int)format << ") for photo: " << filename.toAscii();
            return EXIT_FAILURE;
    }

    qDebug() << "adjust Image attributes: " << ":" << imageScale << ":" << adjustX << ":" << adjustY << ":" << adjustWidth << ":" << adjustHeight;
    qDebug() << "adjust Image attributes: " << ":" << imageScale << ":" << (adjustWidth*adjustHeight) << ":" << adjustSample;

    if (adjustY != 0 || (adjustWidth != imageWidth || adjustHeight != imageHeight)) {
	   adjustImage = new ImageData(bb::PixelFormat::RGBA_Premultiplied, adjustWidth, adjustHeight);

		unsigned char *dstLine = adjustImage->pixels();

		float red, green, blue, alpha;
		int x, y, xi, yi;
		for (yi = 0; yi < adjustImage->height(); yi++) {
			//qDebug() << "Image copy y: " << y;
			unsigned char * dst = dstLine;
			for (xi = 0; xi < adjustImage->width(); xi++) {
				// for sampling not implemented yet, produce a red image
				red = (float)(255);
				green = (float)(0);
				blue = (float)(0);
				alpha = (float)(255);

				x = xi - adjustX;
				y = yi - adjustY;

				if (adjustSample == 1) {
					red = (float)(*(image1.pixels() + y*image1.bytesPerLine() + x*4 + 0));
					green = (float)(*(image1.pixels() + y*image1.bytesPerLine() + x*4 + 1));
					blue = (float)(*(image1.pixels() + y*image1.bytesPerLine() + x*4 + 2));
					alpha = (float)(*(image1.pixels() + y*image1.bytesPerLine() + x*4 + 3));
				} else if (adjustSample == 2) {
					red =
						0.25*((float)(*(image1.pixels() + (y*2+0)*image1.bytesPerLine() + (x*2+0)*4 + 0))) +
						0.25*((float)(*(image1.pixels() + (y*2+0)*image1.bytesPerLine() + (x*2+1)*4 + 0))) +
						0.25*((float)(*(image1.pixels() + (y*2+1)*image1.bytesPerLine() + (x*2+0)*4 + 0))) +
						0.25*((float)(*(image1.pixels() + (y*2+1)*image1.bytesPerLine() + (x*2+1)*4 + 0)));
					green =
						0.25*((float)(*(image1.pixels() + (y*2+0)*image1.bytesPerLine() + (x*2+0)*4 + 1))) +
						0.25*((float)(*(image1.pixels() + (y*2+0)*image1.bytesPerLine() + (x*2+1)*4 + 1))) +
						0.25*((float)(*(image1.pixels() + (y*2+1)*image1.bytesPerLine() + (x*2+0)*4 + 1))) +
						0.25*((float)(*(image1.pixels() + (y*2+1)*image1.bytesPerLine() + (x*2+1)*4 + 1)));
					blue =
						0.25*((float)(*(image1.pixels() + (y*2+0)*image1.bytesPerLine() + (x*2+0)*4 + 2))) +
						0.25*((float)(*(image1.pixels() + (y*2+0)*image1.bytesPerLine() + (x*2+1)*4 + 2))) +
						0.25*((float)(*(image1.pixels() + (y*2+1)*image1.bytesPerLine() + (x*2+0)*4 + 2))) +
						0.25*((float)(*(image1.pixels() + (y*2+1)*image1.bytesPerLine() + (x*2+1)*4 + 2)));
					alpha =
						0.25*((float)(*(image1.pixels() + (y*2+0)*image1.bytesPerLine() + (x*2+0)*4 + 3))) +
						0.25*((float)(*(image1.pixels() + (y*2+0)*image1.bytesPerLine() + (x*2+1)*4 + 3))) +
						0.25*((float)(*(image1.pixels() + (y*2+1)*image1.bytesPerLine() + (x*2+0)*4 + 3))) +
						0.25*((float)(*(image1.pixels() + (y*2+1)*image1.bytesPerLine() + (x*2+1)*4 + 3)));
				} else if (adjustSample == 3) {
					red =
						0.11111*((float)(*(image1.pixels() + (y*3+0)*image1.bytesPerLine() + (x*3+0)*4 + 0))) +
						0.11111*((float)(*(image1.pixels() + (y*3+0)*image1.bytesPerLine() + (x*3+1)*4 + 0))) +
						0.11111*((float)(*(image1.pixels() + (y*3+0)*image1.bytesPerLine() + (x*3+2)*4 + 0))) +
						0.11111*((float)(*(image1.pixels() + (y*3+1)*image1.bytesPerLine() + (x*3+0)*4 + 0))) +
						0.11111*((float)(*(image1.pixels() + (y*3+1)*image1.bytesPerLine() + (x*3+1)*4 + 0))) +
						0.11111*((float)(*(image1.pixels() + (y*3+1)*image1.bytesPerLine() + (x*3+2)*4 + 0))) +
						0.11111*((float)(*(image1.pixels() + (y*3+2)*image1.bytesPerLine() + (x*3+0)*4 + 0))) +
						0.11111*((float)(*(image1.pixels() + (y*3+2)*image1.bytesPerLine() + (x*3+1)*4 + 0))) +
						0.11111*((float)(*(image1.pixels() + (y*3+2)*image1.bytesPerLine() + (x*3+2)*4 + 0)));
					green =
						0.11111*((float)(*(image1.pixels() + (y*3+0)*image1.bytesPerLine() + (x*3+0)*4 + 1))) +
						0.11111*((float)(*(image1.pixels() + (y*3+0)*image1.bytesPerLine() + (x*3+1)*4 + 1))) +
						0.11111*((float)(*(image1.pixels() + (y*3+0)*image1.bytesPerLine() + (x*3+2)*4 + 1))) +
						0.11111*((float)(*(image1.pixels() + (y*3+1)*image1.bytesPerLine() + (x*3+0)*4 + 1))) +
						0.11111*((float)(*(image1.pixels() + (y*3+1)*image1.bytesPerLine() + (x*3+1)*4 + 1))) +
						0.11111*((float)(*(image1.pixels() + (y*3+1)*image1.bytesPerLine() + (x*3+2)*4 + 1))) +
						0.11111*((float)(*(image1.pixels() + (y*3+2)*image1.bytesPerLine() + (x*3+0)*4 + 1))) +
						0.11111*((float)(*(image1.pixels() + (y*3+2)*image1.bytesPerLine() + (x*3+1)*4 + 1))) +
						0.11111*((float)(*(image1.pixels() + (y*3+2)*image1.bytesPerLine() + (x*3+2)*4 + 1)));
					blue =
						0.11111*((float)(*(image1.pixels() + (y*3+0)*image1.bytesPerLine() + (x*3+0)*4 + 2))) +
						0.11111*((float)(*(image1.pixels() + (y*3+0)*image1.bytesPerLine() + (x*3+1)*4 + 2))) +
						0.11111*((float)(*(image1.pixels() + (y*3+0)*image1.bytesPerLine() + (x*3+2)*4 + 2))) +
						0.11111*((float)(*(image1.pixels() + (y*3+1)*image1.bytesPerLine() + (x*3+0)*4 + 2))) +
						0.11111*((float)(*(image1.pixels() + (y*3+1)*image1.bytesPerLine() + (x*3+1)*4 + 2))) +
						0.11111*((float)(*(image1.pixels() + (y*3+1)*image1.bytesPerLine() + (x*3+2)*4 + 2))) +
						0.11111*((float)(*(image1.pixels() + (y*3+2)*image1.bytesPerLine() + (x*3+0)*4 + 2))) +
						0.11111*((float)(*(image1.pixels() + (y*3+2)*image1.bytesPerLine() + (x*3+1)*4 + 2))) +
						0.11111*((float)(*(image1.pixels() + (y*3+2)*image1.bytesPerLine() + (x*3+2)*4 + 2)));
					alpha =
						0.11111*((float)(*(image1.pixels() + (y*3+0)*image1.bytesPerLine() + (x*3+0)*4 + 3))) +
						0.11111*((float)(*(image1.pixels() + (y*3+0)*image1.bytesPerLine() + (x*3+1)*4 + 3))) +
						0.11111*((float)(*(image1.pixels() + (y*3+0)*image1.bytesPerLine() + (x*3+2)*4 + 3))) +
						0.11111*((float)(*(image1.pixels() + (y*3+1)*image1.bytesPerLine() + (x*3+0)*4 + 3))) +
						0.11111*((float)(*(image1.pixels() + (y*3+1)*image1.bytesPerLine() + (x*3+1)*4 + 3))) +
						0.11111*((float)(*(image1.pixels() + (y*3+1)*image1.bytesPerLine() + (x*3+2)*4 + 3))) +
						0.11111*((float)(*(image1.pixels() + (y*3+2)*image1.bytesPerLine() + (x*3+0)*4 + 3))) +
						0.11111*((float)(*(image1.pixels() + (y*3+2)*image1.bytesPerLine() + (x*3+1)*4 + 3))) +
						0.11111*((float)(*(image1.pixels() + (y*3+2)*image1.bytesPerLine() + (x*3+2)*4 + 3)));
				} else if (adjustSample == 4) {
					red =
						0.0625*((float)(*(image1.pixels() + (y*4+0)*image1.bytesPerLine() + (x*4+0)*4 + 0))) +
						0.0625*((float)(*(image1.pixels() + (y*4+0)*image1.bytesPerLine() + (x*4+1)*4 + 0))) +
						0.0625*((float)(*(image1.pixels() + (y*4+0)*image1.bytesPerLine() + (x*4+2)*4 + 0))) +
						0.0625*((float)(*(image1.pixels() + (y*4+0)*image1.bytesPerLine() + (x*4+3)*4 + 0))) +
						0.0625*((float)(*(image1.pixels() + (y*4+1)*image1.bytesPerLine() + (x*4+0)*4 + 0))) +
						0.0625*((float)(*(image1.pixels() + (y*4+1)*image1.bytesPerLine() + (x*4+1)*4 + 0))) +
						0.0625*((float)(*(image1.pixels() + (y*4+1)*image1.bytesPerLine() + (x*4+2)*4 + 0))) +
						0.0625*((float)(*(image1.pixels() + (y*4+1)*image1.bytesPerLine() + (x*4+3)*4 + 0))) +
						0.0625*((float)(*(image1.pixels() + (y*4+2)*image1.bytesPerLine() + (x*4+0)*4 + 0))) +
						0.0625*((float)(*(image1.pixels() + (y*4+2)*image1.bytesPerLine() + (x*4+1)*4 + 0))) +
						0.0625*((float)(*(image1.pixels() + (y*4+2)*image1.bytesPerLine() + (x*4+2)*4 + 0))) +
						0.0625*((float)(*(image1.pixels() + (y*4+2)*image1.bytesPerLine() + (x*4+3)*4 + 0))) +
						0.0625*((float)(*(image1.pixels() + (y*4+3)*image1.bytesPerLine() + (x*4+0)*4 + 0))) +
						0.0625*((float)(*(image1.pixels() + (y*4+3)*image1.bytesPerLine() + (x*4+1)*4 + 0))) +
						0.0625*((float)(*(image1.pixels() + (y*4+3)*image1.bytesPerLine() + (x*4+2)*4 + 0))) +
						0.0625*((float)(*(image1.pixels() + (y*4+3)*image1.bytesPerLine() + (x*4+3)*4 + 0)));
					green =
						0.0625*((float)(*(image1.pixels() + (y*4+0)*image1.bytesPerLine() + (x*4+0)*4 + 1))) +
						0.0625*((float)(*(image1.pixels() + (y*4+0)*image1.bytesPerLine() + (x*4+1)*4 + 1))) +
						0.0625*((float)(*(image1.pixels() + (y*4+0)*image1.bytesPerLine() + (x*4+2)*4 + 1))) +
						0.0625*((float)(*(image1.pixels() + (y*4+0)*image1.bytesPerLine() + (x*4+3)*4 + 1))) +
						0.0625*((float)(*(image1.pixels() + (y*4+1)*image1.bytesPerLine() + (x*4+0)*4 + 1))) +
						0.0625*((float)(*(image1.pixels() + (y*4+1)*image1.bytesPerLine() + (x*4+1)*4 + 1))) +
						0.0625*((float)(*(image1.pixels() + (y*4+1)*image1.bytesPerLine() + (x*4+2)*4 + 1))) +
						0.0625*((float)(*(image1.pixels() + (y*4+1)*image1.bytesPerLine() + (x*4+3)*4 + 1))) +
						0.0625*((float)(*(image1.pixels() + (y*4+2)*image1.bytesPerLine() + (x*4+0)*4 + 1))) +
						0.0625*((float)(*(image1.pixels() + (y*4+2)*image1.bytesPerLine() + (x*4+1)*4 + 1))) +
						0.0625*((float)(*(image1.pixels() + (y*4+2)*image1.bytesPerLine() + (x*4+2)*4 + 1))) +
						0.0625*((float)(*(image1.pixels() + (y*4+2)*image1.bytesPerLine() + (x*4+3)*4 + 1))) +
						0.0625*((float)(*(image1.pixels() + (y*4+3)*image1.bytesPerLine() + (x*4+0)*4 + 1))) +
						0.0625*((float)(*(image1.pixels() + (y*4+3)*image1.bytesPerLine() + (x*4+1)*4 + 1))) +
						0.0625*((float)(*(image1.pixels() + (y*4+3)*image1.bytesPerLine() + (x*4+2)*4 + 1))) +
						0.0625*((float)(*(image1.pixels() + (y*4+3)*image1.bytesPerLine() + (x*4+3)*4 + 1)));
					blue =
						0.0625*((float)(*(image1.pixels() + (y*4+0)*image1.bytesPerLine() + (x*4+0)*4 + 2))) +
						0.0625*((float)(*(image1.pixels() + (y*4+0)*image1.bytesPerLine() + (x*4+1)*4 + 2))) +
						0.0625*((float)(*(image1.pixels() + (y*4+0)*image1.bytesPerLine() + (x*4+2)*4 + 2))) +
						0.0625*((float)(*(image1.pixels() + (y*4+0)*image1.bytesPerLine() + (x*4+3)*4 + 2))) +
						0.0625*((float)(*(image1.pixels() + (y*4+1)*image1.bytesPerLine() + (x*4+0)*4 + 2))) +
						0.0625*((float)(*(image1.pixels() + (y*4+1)*image1.bytesPerLine() + (x*4+1)*4 + 2))) +
						0.0625*((float)(*(image1.pixels() + (y*4+1)*image1.bytesPerLine() + (x*4+2)*4 + 2))) +
						0.0625*((float)(*(image1.pixels() + (y*4+1)*image1.bytesPerLine() + (x*4+3)*4 + 2))) +
						0.0625*((float)(*(image1.pixels() + (y*4+2)*image1.bytesPerLine() + (x*4+0)*4 + 2))) +
						0.0625*((float)(*(image1.pixels() + (y*4+2)*image1.bytesPerLine() + (x*4+1)*4 + 2))) +
						0.0625*((float)(*(image1.pixels() + (y*4+2)*image1.bytesPerLine() + (x*4+2)*4 + 2))) +
						0.0625*((float)(*(image1.pixels() + (y*4+2)*image1.bytesPerLine() + (x*4+3)*4 + 2))) +
						0.0625*((float)(*(image1.pixels() + (y*4+3)*image1.bytesPerLine() + (x*4+0)*4 + 2))) +
						0.0625*((float)(*(image1.pixels() + (y*4+3)*image1.bytesPerLine() + (x*4+1)*4 + 2))) +
						0.0625*((float)(*(image1.pixels() + (y*4+3)*image1.bytesPerLine() + (x*4+2)*4 + 2))) +
						0.0625*((float)(*(image1.pixels() + (y*4+3)*image1.bytesPerLine() + (x*4+3)*4 + 2)));
					alpha =
						0.0625*((float)(*(image1.pixels() + (y*4+0)*image1.bytesPerLine() + (x*4+0)*4 + 3))) +
						0.0625*((float)(*(image1.pixels() + (y*4+0)*image1.bytesPerLine() + (x*4+1)*4 + 3))) +
						0.0625*((float)(*(image1.pixels() + (y*4+0)*image1.bytesPerLine() + (x*4+2)*4 + 3))) +
						0.0625*((float)(*(image1.pixels() + (y*4+0)*image1.bytesPerLine() + (x*4+3)*4 + 3))) +
						0.0625*((float)(*(image1.pixels() + (y*4+1)*image1.bytesPerLine() + (x*4+0)*4 + 3))) +
						0.0625*((float)(*(image1.pixels() + (y*4+1)*image1.bytesPerLine() + (x*4+1)*4 + 3))) +
						0.0625*((float)(*(image1.pixels() + (y*4+1)*image1.bytesPerLine() + (x*4+2)*4 + 3))) +
						0.0625*((float)(*(image1.pixels() + (y*4+1)*image1.bytesPerLine() + (x*4+3)*4 + 3))) +
						0.0625*((float)(*(image1.pixels() + (y*4+2)*image1.bytesPerLine() + (x*4+0)*4 + 3))) +
						0.0625*((float)(*(image1.pixels() + (y*4+2)*image1.bytesPerLine() + (x*4+1)*4 + 3))) +
						0.0625*((float)(*(image1.pixels() + (y*4+2)*image1.bytesPerLine() + (x*4+2)*4 + 3))) +
						0.0625*((float)(*(image1.pixels() + (y*4+2)*image1.bytesPerLine() + (x*4+3)*4 + 3))) +
						0.0625*((float)(*(image1.pixels() + (y*4+3)*image1.bytesPerLine() + (x*4+0)*4 + 3))) +
						0.0625*((float)(*(image1.pixels() + (y*4+3)*image1.bytesPerLine() + (x*4+1)*4 + 3))) +
						0.0625*((float)(*(image1.pixels() + (y*4+3)*image1.bytesPerLine() + (x*4+2)*4 + 3))) +
						0.0625*((float)(*(image1.pixels() + (y*4+3)*image1.bytesPerLine() + (x*4+3)*4 + 3)));
				}

				if (imageFormat == PixelFormat::RGBX) {
					alpha = 255.0f;
				}

				switch(m_mode) {
					case VIEW_3D_TYPE_ANAGLYPH_RED_CYAN:
						dst++;
						*dst++ = (char)(green);
						*dst++ = (char)(blue);
						dst++;
						break;

					case VIEW_3D_TYPE_ANAGLYPH_RED_GREEN:
						dst++;
						*dst++ = (char)(green);
						dst++;
						dst++;
						break;

					case VIEW_3D_TYPE_ANAGLYPH_CYAN_RED:
					case VIEW_3D_TYPE_ANAGLYPH_GREEN_RED:
						*dst++ = (char)(red);
						dst++;
						dst++;
						dst++;
						break;

					case VIEW_3D_TYPE_ANAGLYPH_BLUE_YELLOW:
						*dst++ = (char)(red);
						*dst++ = (char)(green);
						*dst++ = (char)(0);
						dst++;
						dst++;
						break;

					case VIEW_3D_TYPE_ANAGLYPH_YELLOW_BLUE:
						dst++;
						dst++;
						*dst++ = (char)(blue);
						dst++;
						break;

					case VIEW_3D_TYPE_ANAGLYPH_CLEAR_VIOLET:
						*dst++ += (char)(red / 2);
						dst++;
						*dst++ = (char)(blue);
						dst++;
						break;
				}
			}
			dstLine += adjustImage->bytesPerLine();
		}
	}



    int texWidth, texHeight;

    texWidth = nextp2(adjustWidth);
    texHeight = nextp2(adjustHeight);

    getGLContext();

    if ((*tex) > 0) {
        qDebug() << "glBindTexture: " << 0;
        glBindTexture(GL_TEXTURE_2D, 0);
    } else {
		qDebug() << "glGenTextures: " << tex << *tex;
		glGenTextures(1, tex);
		qDebug() << "glGenTextures: " << tex << *tex;
    }

    qDebug() << "glBindTexture: " << tex << *tex;
    glBindTexture(GL_TEXTURE_2D, (*tex));
    qDebug() << "glBindTexture: " << tex << *tex;

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    glTexImage2D(GL_TEXTURE_2D, 0, format, texWidth, texHeight, 0, format, GL_UNSIGNED_BYTE, NULL);
    glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, adjustWidth, adjustHeight, format, GL_UNSIGNED_BYTE, adjustImage->pixels());

    GLint err = glGetError();

    //clean up memory and close stuff
    if (adjustImage != &image) {
    	delete adjustImage;
    }

   if (err == 0) {
        //Return physical with and height of texture if pointers are not null
        if(width) {
            *width = adjustWidth;
        }
        if (height) {
            *height = adjustHeight;
        }
        //Return modified texture coordinates if pointers are not null
        if(tex_x) {
            *tex_x = ((float) adjustWidth - 0.5f) / ((float)texWidth);
        }
        if(tex_y) {
            *tex_y = ((float) adjustHeight - 0.5f) / ((float)texHeight);
        }
        return EXIT_SUCCESS;
    } else {
        fprintf(stderr, "GL error %i \n", err);
        return EXIT_FAILURE;
    }

}

int Photo3DView::genJPGThumbnail(const QString& filename, const QString& thumbFilename) {
    QString imageFilePath = (new QDir())->absoluteFilePath(filename);

    ImageData image = ImageConverter::decode(QUrl::fromLocalFile(imageFilePath));

    //variables to pass to get info
    PixelFormat::Type imageFormat;
    int imageWidth, imageHeight;
    imageWidth = image.width();
    imageHeight = image.height();
    imageFormat = image.format();
    qDebug() << "Image attributes: " << imageFilePath << ":" << imageWidth << ":" << imageHeight << ":" << imageFormat;

    // photos need to be mapped to a 1920x540 area of the screen so calculate scaling factor to fill this area
    float imageScale = 1.0;
    int adjustX, adjustY, adjustWidth, adjustHeight, adjustSample;

    adjustWidth = imageWidth;
    adjustHeight = imageHeight;
    adjustX = 0;
    adjustY = 0;

    if (imageHeight > 1080) {
    	adjustHeight = 1080;
    	imageScale = 1080.0 / imageHeight;
    	adjustWidth = (int)((float)imageWidth * imageScale);
    	adjustX = (1920 - adjustWidth) / 2;
    	adjustY = (1080 - adjustHeight) / 2;
    } else {
    	adjustX = (1920 - imageWidth) / 2;
    	adjustY = (1080 - imageHeight) / 2;
    }

    qDebug() << "adjust Image attributes: " << ":" << adjustX << ":" << adjustY << ":" << adjustWidth << ":" << adjustHeight;
    qDebug() << "adjust Image attributes: " << ":" << imageScale << ":" << (adjustWidth*adjustHeight);

    if (adjustWidth < 1920) {
    	adjustWidth = 1920;
    	imageScale = 1920.0 / imageWidth;
    	adjustHeight = (int)((float)imageHeight * imageScale);
    	adjustX = (1920 - adjustWidth) / 2;
    	adjustY = (1080 - adjustHeight) / 2;
    }

    qDebug() << "adjust Image attributes: " << ":" << imageScale << ":" << adjustX << ":" << adjustY << ":" << adjustWidth << ":" << adjustHeight;
    qDebug() << "adjust Image attributes: " << ":" << imageScale << ":" << (adjustWidth*adjustHeight);

    qDebug() << "adjust Image attributes: " << ":" << imageScale << ":" << adjustX << ":" << adjustY << ":" << adjustWidth << ":" << adjustHeight;
    qDebug() << "adjust Image attributes: " << ":" << imageScale << ":" << (adjustWidth*adjustWidth);

    // adjust scale to allow for even multiple of bytes for scaling
    int checkWidth, checkHeight;
    if (imageScale < 0.25) {
    	checkWidth = imageWidth / 4;
    	checkHeight = imageHeight / 4;
		adjustSample = 4;
		imageScale = 0.25;
		adjustWidth = 1920;
		adjustHeight = 1080;
		adjustX = (1920 - checkWidth) / 2;
		adjustY = (1080 - checkHeight) / 2;
    } else
    if (imageScale > 0.25 && imageScale < 0.33333333333) {
    	checkWidth = imageWidth / 3;
    	checkHeight = imageHeight / 3;
   		adjustSample = 3;
		imageScale = 0.33333333333;
		adjustWidth = 1920;
		adjustHeight = 1080;
		adjustX = (1920 - checkWidth) / 2;
		adjustY = (1080 - checkHeight) / 2;
    } else
    if (imageScale > 0.33333333333 && imageScale < 0.5) {
    	checkWidth = imageWidth / 2;
    	checkHeight = imageHeight / 2;
		adjustSample = 2;
		imageScale = 0.5;
		adjustWidth = 1920;
		adjustHeight = 1080;
		adjustX = (1920 - checkWidth) / 2;
		adjustY = (1080 - checkHeight) / 2;
    } else
    if (imageScale > 0.5 && imageScale < 1) {
    	checkWidth = imageWidth;
    	checkHeight = imageHeight;
		adjustSample = 1;
		imageScale = 1;
		adjustWidth = 1920;
		adjustHeight = 1080;
		adjustX = (1920 - checkWidth) / 2;
		adjustY = (1080 - checkHeight) / 2;
    }

    qDebug() << "adjust Image attributes: " << ":" << imageScale << ":" << adjustX << ":" << adjustY << ":" << checkWidth << ":" << checkHeight << ":" << adjustWidth << ":" << adjustHeight;
    qDebug() << "adjust Image attributes: " << ":" << imageScale << ":" << (adjustWidth*adjustHeight) << ":" << adjustSample;

    ImageData* adjustImage = &image;
    if (adjustY != 0 || (adjustWidth != imageWidth || adjustHeight != imageHeight)) {
	   adjustImage = new ImageData(bb::PixelFormat::RGBA_Premultiplied, adjustWidth, adjustHeight);

		unsigned char *dstLine = adjustImage->pixels();

		float red, green, blue, alpha;
		int x, y, xi, yi;
		for (yi = 0; yi < adjustImage->height(); yi++) {
			//qDebug() << "Image copy y: " << y;
			unsigned char * dst = dstLine;
			for (xi = 0; xi < adjustImage->width(); xi++) {
				// for sampling not implemented yet, produce a red image
				red = (float)(255);
				green = (float)(0);
				blue = (float)(0);
				alpha = (float)(255);

				x = xi - adjustX;
				y = yi - adjustY;

				if (adjustSample == 1) {
					red = (float)(*(image.pixels() + y*image.bytesPerLine() + x*4 + 0));
					green = (float)(*(image.pixels() + y*image.bytesPerLine() + x*4 + 1));
					blue = (float)(*(image.pixels() + y*image.bytesPerLine() + x*4 + 2));
					alpha = (float)(*(image.pixels() + y*image.bytesPerLine() + x*4 + 3));
				} else if (adjustSample == 2) {
					red =
						0.25*((float)(*(image.pixels() + (y*2+0)*image.bytesPerLine() + (x*2+0)*4 + 0))) +
						0.25*((float)(*(image.pixels() + (y*2+0)*image.bytesPerLine() + (x*2+1)*4 + 0))) +
						0.25*((float)(*(image.pixels() + (y*2+1)*image.bytesPerLine() + (x*2+0)*4 + 0))) +
						0.25*((float)(*(image.pixels() + (y*2+1)*image.bytesPerLine() + (x*2+1)*4 + 0)));
					green =
						0.25*((float)(*(image.pixels() + (y*2+0)*image.bytesPerLine() + (x*2+0)*4 + 1))) +
						0.25*((float)(*(image.pixels() + (y*2+0)*image.bytesPerLine() + (x*2+1)*4 + 1))) +
						0.25*((float)(*(image.pixels() + (y*2+1)*image.bytesPerLine() + (x*2+0)*4 + 1))) +
						0.25*((float)(*(image.pixels() + (y*2+1)*image.bytesPerLine() + (x*2+1)*4 + 1)));
					blue =
						0.25*((float)(*(image.pixels() + (y*2+0)*image.bytesPerLine() + (x*2+0)*4 + 2))) +
						0.25*((float)(*(image.pixels() + (y*2+0)*image.bytesPerLine() + (x*2+1)*4 + 2))) +
						0.25*((float)(*(image.pixels() + (y*2+1)*image.bytesPerLine() + (x*2+0)*4 + 2))) +
						0.25*((float)(*(image.pixels() + (y*2+1)*image.bytesPerLine() + (x*2+1)*4 + 2)));
					alpha =
						0.25*((float)(*(image.pixels() + (y*2+0)*image.bytesPerLine() + (x*2+0)*4 + 3))) +
						0.25*((float)(*(image.pixels() + (y*2+0)*image.bytesPerLine() + (x*2+1)*4 + 3))) +
						0.25*((float)(*(image.pixels() + (y*2+1)*image.bytesPerLine() + (x*2+0)*4 + 3))) +
						0.25*((float)(*(image.pixels() + (y*2+1)*image.bytesPerLine() + (x*2+1)*4 + 3)));
				} else if (adjustSample == 3) {
					red =
						0.11111*((float)(*(image.pixels() + (y*3+0)*image.bytesPerLine() + (x*3+0)*4 + 0))) +
						0.11111*((float)(*(image.pixels() + (y*3+0)*image.bytesPerLine() + (x*3+1)*4 + 0))) +
						0.11111*((float)(*(image.pixels() + (y*3+0)*image.bytesPerLine() + (x*3+2)*4 + 0))) +
						0.11111*((float)(*(image.pixels() + (y*3+1)*image.bytesPerLine() + (x*3+0)*4 + 0))) +
						0.11111*((float)(*(image.pixels() + (y*3+1)*image.bytesPerLine() + (x*3+1)*4 + 0))) +
						0.11111*((float)(*(image.pixels() + (y*3+1)*image.bytesPerLine() + (x*3+2)*4 + 0))) +
						0.11111*((float)(*(image.pixels() + (y*3+2)*image.bytesPerLine() + (x*3+0)*4 + 0))) +
						0.11111*((float)(*(image.pixels() + (y*3+2)*image.bytesPerLine() + (x*3+1)*4 + 0))) +
						0.11111*((float)(*(image.pixels() + (y*3+2)*image.bytesPerLine() + (x*3+2)*4 + 0)));
					green =
						0.11111*((float)(*(image.pixels() + (y*3+0)*image.bytesPerLine() + (x*3+0)*4 + 1))) +
						0.11111*((float)(*(image.pixels() + (y*3+0)*image.bytesPerLine() + (x*3+1)*4 + 1))) +
						0.11111*((float)(*(image.pixels() + (y*3+0)*image.bytesPerLine() + (x*3+2)*4 + 1))) +
						0.11111*((float)(*(image.pixels() + (y*3+1)*image.bytesPerLine() + (x*3+0)*4 + 1))) +
						0.11111*((float)(*(image.pixels() + (y*3+1)*image.bytesPerLine() + (x*3+1)*4 + 1))) +
						0.11111*((float)(*(image.pixels() + (y*3+1)*image.bytesPerLine() + (x*3+2)*4 + 1))) +
						0.11111*((float)(*(image.pixels() + (y*3+2)*image.bytesPerLine() + (x*3+0)*4 + 1))) +
						0.11111*((float)(*(image.pixels() + (y*3+2)*image.bytesPerLine() + (x*3+1)*4 + 1))) +
						0.11111*((float)(*(image.pixels() + (y*3+2)*image.bytesPerLine() + (x*3+2)*4 + 1)));
					blue =
						0.11111*((float)(*(image.pixels() + (y*3+0)*image.bytesPerLine() + (x*3+0)*4 + 2))) +
						0.11111*((float)(*(image.pixels() + (y*3+0)*image.bytesPerLine() + (x*3+1)*4 + 2))) +
						0.11111*((float)(*(image.pixels() + (y*3+0)*image.bytesPerLine() + (x*3+2)*4 + 2))) +
						0.11111*((float)(*(image.pixels() + (y*3+1)*image.bytesPerLine() + (x*3+0)*4 + 2))) +
						0.11111*((float)(*(image.pixels() + (y*3+1)*image.bytesPerLine() + (x*3+1)*4 + 2))) +
						0.11111*((float)(*(image.pixels() + (y*3+1)*image.bytesPerLine() + (x*3+2)*4 + 2))) +
						0.11111*((float)(*(image.pixels() + (y*3+2)*image.bytesPerLine() + (x*3+0)*4 + 2))) +
						0.11111*((float)(*(image.pixels() + (y*3+2)*image.bytesPerLine() + (x*3+1)*4 + 2))) +
						0.11111*((float)(*(image.pixels() + (y*3+2)*image.bytesPerLine() + (x*3+2)*4 + 2)));
					alpha =
						0.11111*((float)(*(image.pixels() + (y*3+0)*image.bytesPerLine() + (x*3+0)*4 + 3))) +
						0.11111*((float)(*(image.pixels() + (y*3+0)*image.bytesPerLine() + (x*3+1)*4 + 3))) +
						0.11111*((float)(*(image.pixels() + (y*3+0)*image.bytesPerLine() + (x*3+2)*4 + 3))) +
						0.11111*((float)(*(image.pixels() + (y*3+1)*image.bytesPerLine() + (x*3+0)*4 + 3))) +
						0.11111*((float)(*(image.pixels() + (y*3+1)*image.bytesPerLine() + (x*3+1)*4 + 3))) +
						0.11111*((float)(*(image.pixels() + (y*3+1)*image.bytesPerLine() + (x*3+2)*4 + 3))) +
						0.11111*((float)(*(image.pixels() + (y*3+2)*image.bytesPerLine() + (x*3+0)*4 + 3))) +
						0.11111*((float)(*(image.pixels() + (y*3+2)*image.bytesPerLine() + (x*3+1)*4 + 3))) +
						0.11111*((float)(*(image.pixels() + (y*3+2)*image.bytesPerLine() + (x*3+2)*4 + 3)));
				} else if (adjustSample == 4) {
					red =
						0.0625*((float)(*(image.pixels() + (y*4+0)*image.bytesPerLine() + (x*4+0)*4 + 0))) +
						0.0625*((float)(*(image.pixels() + (y*4+0)*image.bytesPerLine() + (x*4+1)*4 + 0))) +
						0.0625*((float)(*(image.pixels() + (y*4+0)*image.bytesPerLine() + (x*4+2)*4 + 0))) +
						0.0625*((float)(*(image.pixels() + (y*4+0)*image.bytesPerLine() + (x*4+3)*4 + 0))) +
						0.0625*((float)(*(image.pixels() + (y*4+1)*image.bytesPerLine() + (x*4+0)*4 + 0))) +
						0.0625*((float)(*(image.pixels() + (y*4+1)*image.bytesPerLine() + (x*4+1)*4 + 0))) +
						0.0625*((float)(*(image.pixels() + (y*4+1)*image.bytesPerLine() + (x*4+2)*4 + 0))) +
						0.0625*((float)(*(image.pixels() + (y*4+1)*image.bytesPerLine() + (x*4+3)*4 + 0))) +
						0.0625*((float)(*(image.pixels() + (y*4+2)*image.bytesPerLine() + (x*4+0)*4 + 0))) +
						0.0625*((float)(*(image.pixels() + (y*4+2)*image.bytesPerLine() + (x*4+1)*4 + 0))) +
						0.0625*((float)(*(image.pixels() + (y*4+2)*image.bytesPerLine() + (x*4+2)*4 + 0))) +
						0.0625*((float)(*(image.pixels() + (y*4+2)*image.bytesPerLine() + (x*4+3)*4 + 0))) +
						0.0625*((float)(*(image.pixels() + (y*4+3)*image.bytesPerLine() + (x*4+0)*4 + 0))) +
						0.0625*((float)(*(image.pixels() + (y*4+3)*image.bytesPerLine() + (x*4+1)*4 + 0))) +
						0.0625*((float)(*(image.pixels() + (y*4+3)*image.bytesPerLine() + (x*4+2)*4 + 0))) +
						0.0625*((float)(*(image.pixels() + (y*4+3)*image.bytesPerLine() + (x*4+3)*4 + 0)));
					green =
						0.0625*((float)(*(image.pixels() + (y*4+0)*image.bytesPerLine() + (x*4+0)*4 + 1))) +
						0.0625*((float)(*(image.pixels() + (y*4+0)*image.bytesPerLine() + (x*4+1)*4 + 1))) +
						0.0625*((float)(*(image.pixels() + (y*4+0)*image.bytesPerLine() + (x*4+2)*4 + 1))) +
						0.0625*((float)(*(image.pixels() + (y*4+0)*image.bytesPerLine() + (x*4+3)*4 + 1))) +
						0.0625*((float)(*(image.pixels() + (y*4+1)*image.bytesPerLine() + (x*4+0)*4 + 1))) +
						0.0625*((float)(*(image.pixels() + (y*4+1)*image.bytesPerLine() + (x*4+1)*4 + 1))) +
						0.0625*((float)(*(image.pixels() + (y*4+1)*image.bytesPerLine() + (x*4+2)*4 + 1))) +
						0.0625*((float)(*(image.pixels() + (y*4+1)*image.bytesPerLine() + (x*4+3)*4 + 1))) +
						0.0625*((float)(*(image.pixels() + (y*4+2)*image.bytesPerLine() + (x*4+0)*4 + 1))) +
						0.0625*((float)(*(image.pixels() + (y*4+2)*image.bytesPerLine() + (x*4+1)*4 + 1))) +
						0.0625*((float)(*(image.pixels() + (y*4+2)*image.bytesPerLine() + (x*4+2)*4 + 1))) +
						0.0625*((float)(*(image.pixels() + (y*4+2)*image.bytesPerLine() + (x*4+3)*4 + 1))) +
						0.0625*((float)(*(image.pixels() + (y*4+3)*image.bytesPerLine() + (x*4+0)*4 + 1))) +
						0.0625*((float)(*(image.pixels() + (y*4+3)*image.bytesPerLine() + (x*4+1)*4 + 1))) +
						0.0625*((float)(*(image.pixels() + (y*4+3)*image.bytesPerLine() + (x*4+2)*4 + 1))) +
						0.0625*((float)(*(image.pixels() + (y*4+3)*image.bytesPerLine() + (x*4+3)*4 + 1)));
					blue =
						0.0625*((float)(*(image.pixels() + (y*4+0)*image.bytesPerLine() + (x*4+0)*4 + 2))) +
						0.0625*((float)(*(image.pixels() + (y*4+0)*image.bytesPerLine() + (x*4+1)*4 + 2))) +
						0.0625*((float)(*(image.pixels() + (y*4+0)*image.bytesPerLine() + (x*4+2)*4 + 2))) +
						0.0625*((float)(*(image.pixels() + (y*4+0)*image.bytesPerLine() + (x*4+3)*4 + 2))) +
						0.0625*((float)(*(image.pixels() + (y*4+1)*image.bytesPerLine() + (x*4+0)*4 + 2))) +
						0.0625*((float)(*(image.pixels() + (y*4+1)*image.bytesPerLine() + (x*4+1)*4 + 2))) +
						0.0625*((float)(*(image.pixels() + (y*4+1)*image.bytesPerLine() + (x*4+2)*4 + 2))) +
						0.0625*((float)(*(image.pixels() + (y*4+1)*image.bytesPerLine() + (x*4+3)*4 + 2))) +
						0.0625*((float)(*(image.pixels() + (y*4+2)*image.bytesPerLine() + (x*4+0)*4 + 2))) +
						0.0625*((float)(*(image.pixels() + (y*4+2)*image.bytesPerLine() + (x*4+1)*4 + 2))) +
						0.0625*((float)(*(image.pixels() + (y*4+2)*image.bytesPerLine() + (x*4+2)*4 + 2))) +
						0.0625*((float)(*(image.pixels() + (y*4+2)*image.bytesPerLine() + (x*4+3)*4 + 2))) +
						0.0625*((float)(*(image.pixels() + (y*4+3)*image.bytesPerLine() + (x*4+0)*4 + 2))) +
						0.0625*((float)(*(image.pixels() + (y*4+3)*image.bytesPerLine() + (x*4+1)*4 + 2))) +
						0.0625*((float)(*(image.pixels() + (y*4+3)*image.bytesPerLine() + (x*4+2)*4 + 2))) +
						0.0625*((float)(*(image.pixels() + (y*4+3)*image.bytesPerLine() + (x*4+3)*4 + 2)));
					alpha =
						0.0625*((float)(*(image.pixels() + (y*4+0)*image.bytesPerLine() + (x*4+0)*4 + 3))) +
						0.0625*((float)(*(image.pixels() + (y*4+0)*image.bytesPerLine() + (x*4+1)*4 + 3))) +
						0.0625*((float)(*(image.pixels() + (y*4+0)*image.bytesPerLine() + (x*4+2)*4 + 3))) +
						0.0625*((float)(*(image.pixels() + (y*4+0)*image.bytesPerLine() + (x*4+3)*4 + 3))) +
						0.0625*((float)(*(image.pixels() + (y*4+1)*image.bytesPerLine() + (x*4+0)*4 + 3))) +
						0.0625*((float)(*(image.pixels() + (y*4+1)*image.bytesPerLine() + (x*4+1)*4 + 3))) +
						0.0625*((float)(*(image.pixels() + (y*4+1)*image.bytesPerLine() + (x*4+2)*4 + 3))) +
						0.0625*((float)(*(image.pixels() + (y*4+1)*image.bytesPerLine() + (x*4+3)*4 + 3))) +
						0.0625*((float)(*(image.pixels() + (y*4+2)*image.bytesPerLine() + (x*4+0)*4 + 3))) +
						0.0625*((float)(*(image.pixels() + (y*4+2)*image.bytesPerLine() + (x*4+1)*4 + 3))) +
						0.0625*((float)(*(image.pixels() + (y*4+2)*image.bytesPerLine() + (x*4+2)*4 + 3))) +
						0.0625*((float)(*(image.pixels() + (y*4+2)*image.bytesPerLine() + (x*4+3)*4 + 3))) +
						0.0625*((float)(*(image.pixels() + (y*4+3)*image.bytesPerLine() + (x*4+0)*4 + 3))) +
						0.0625*((float)(*(image.pixels() + (y*4+3)*image.bytesPerLine() + (x*4+1)*4 + 3))) +
						0.0625*((float)(*(image.pixels() + (y*4+3)*image.bytesPerLine() + (x*4+2)*4 + 3))) +
						0.0625*((float)(*(image.pixels() + (y*4+3)*image.bytesPerLine() + (x*4+3)*4 + 3)));
				}

				if (imageFormat == PixelFormat::RGBX) {
					alpha = (float)255;
				}

				*dst++ = (char)(red);
				*dst++ = (char)(green);
				*dst++ = (char)(blue);
				*dst++ = (char)(alpha);

			}
			dstLine += adjustImage->bytesPerLine();
		}
	}
    qDebug() << "save to: " << QUrl::fromLocalFile(thumbFilename);

    if (ImageConverter::encode (QUrl::fromLocalFile(thumbFilename), *adjustImage, 100)) {
	    //clean up memory and close stuff
	    if (adjustImage != &image) {
	    	delete adjustImage;
	    }

        return EXIT_SUCCESS;
    } else {
	    //clean up memory and close stuff
	    if (adjustImage != &image) {
	    	delete adjustImage;
	    }

        return EXIT_FAILURE;
    }
}


void Photo3DView::update() {
/*
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
*/
	//setStale(true);
}

void Photo3DView::onTouch(bb::cascades::TouchEvent *event) {
/*
	if (event->isDown()) {
		if (!_touchMode->compare("rotate")) {
			_autoRotate = false;

			setStale(true);
		}
		lastTouchX = lastTouchY = -1;

		qDebug() << "touch down: " << _touchMode->toLatin1() << " : " << event->screenX() << " : " << event->screenY();
	}
	if (event->isUp()) {
		if (!_touchMode->compare("rotate")) {
		    _autoRotate = true;

			setStale(true);
		}
		lastTouchX = lastTouchY = -1;

		qDebug() << "touch up: " << _touchMode->toLatin1() << " : " << event->screenX() << " : " << event->screenY();
	}
	if (event->isMove()) {
		float currTouchX = event->screenX();
		float currTouchY = event->screenY();

		float moveX = currTouchX - lastTouchX;
		float moveY = -(currTouchY - lastTouchY);

		qDebug() << "move: " << _touchMode->toLatin1() << " : " << moveX << " : " << moveY;

		if (!_touchMode->compare("scale")) {
			if (lastTouchX != -1) {
				float deltaX = moveX / (100.0f);
				float deltaY = moveY / (100.0f);

				float deltaAxis = sqrt(deltaX*deltaX + deltaY*deltaY);
				if (deltaY < 0) {
					deltaAxis *= -1;
				}

				if (!_toolAxis->compare("X")) {
					obj_scale_x += deltaAxis;
					if (obj_scale_x < 0) {
						obj_scale_x = 0;
					}
					//qDebug() << "x: " << deltaAxis << " : " << obj_scale_x;

					setStale(true);
				} else
				if (!_toolAxis->compare("Y")) {
					obj_scale_y += deltaAxis;
					if (obj_scale_y < 0) {
						obj_scale_y = 0;
					}
					//qDebug() << "y: " << deltaAxis << " : " << obj_scale_y;

					setStale(true);
				} else
				if (!_toolAxis->compare("Z")) {
					obj_scale_z += deltaAxis;
					if (obj_scale_z < 0) {
						obj_scale_z = 0;
					}
					//qDebug() << "z: " << deltaAxis << " : " << obj_scale_z;

					setStale(true);
				}
			}
		} else
		if (!_touchMode->compare("translate")) {
			if (lastTouchX != -1) {
				float deltaX = moveX / (100.0f);
				float deltaY = moveY / (100.0f);

				float deltaAxis = sqrt(deltaX*deltaX + deltaY*deltaY);
				if (deltaY > 0 && !_toolAxis->compare("Z")) {
					deltaAxis *= -1;
				}
				if (deltaY < 0 && _toolAxis->compare("Z")) {
					deltaAxis *= -1;
				}

				if (!_toolAxis->compare("X")) {
					obj_pos_x += deltaAxis;
					//qDebug() << "x: " << deltaAxis << " : " << obj_pos_x << " : " << obj_pos_y << " : " << obj_pos_z;

					setStale(true);
				} else
				if (!_toolAxis->compare("Y")) {
					obj_pos_y += deltaAxis;
					//qDebug() << "y: " << deltaAxis << " : " << obj_pos_x << " : " << obj_pos_y << " : " << obj_pos_z;

					setStale(true);
				} else
				if (!_toolAxis->compare("Z")) {
					obj_pos_z += deltaAxis;
					//qDebug() << "z: " << deltaAxis << " : " << obj_pos_x << " : " << obj_pos_y << " : " << obj_pos_z;

					setStale(true);
				}

				shadow_pos_x = 70.0f + (22.5f * 100.0f / (10.0f - obj_pos_z) ) * (obj_pos_x) - 2.25f * (obj_pos_z);
		        shadow_pos_y = 10.0f  - 5.25f * (obj_pos_z);
		        shadow_size_x = (float) 512 + 3.75f * obj_pos_z + 2.25f * (obj_pos_y);
		        shadow_size_y = (float) 256 + 2.0f * obj_pos_z + 2.25f * (obj_pos_y);

				//qDebug() << "shadow: " << shadow_pos_x << " : " << shadow_pos_y << " : " << shadow_size_x << " : " << shadow_size_y;

				shadow_vertices[0] = shadow_pos_x;
				shadow_vertices[1] = shadow_pos_y;
				shadow_vertices[2] = shadow_pos_x + shadow_size_x;
				shadow_vertices[3] = shadow_pos_y;
				shadow_vertices[4] = shadow_pos_x;
				shadow_vertices[5] = shadow_pos_y + shadow_size_y;
				shadow_vertices[6] = shadow_pos_x + shadow_size_x;
				shadow_vertices[7] = shadow_pos_y + shadow_size_y;
			}
		} else
		if (!_touchMode->compare("rotate")) {

			float cosTheta, sinTheta, lastCosTheta, lastSinTheta;

			if (lastTouchX != -1) {

				//cosTheta = radius      / denominator;
				//sinTheta = deltaRadius / denominator;
				cosTheta = (currTouchX - 384.0f) / 384.0f;
				sinTheta = -(currTouchY - 640.0f) / 640.0f;
				lastCosTheta = (lastTouchX - 384.0f) / 384.0f;
				lastSinTheta = -(currTouchY - 640.0f) / 640.0f;

				float currAngle, lastAngle, deltaAngle;

				if (cosTheta != 0) {
					currAngle = acos(cosTheta) * 180.0 / M_PI;
					if (sinTheta < 0) {
						currAngle = 360.0f - currAngle;
					}
				} else {
					if (sinTheta < 0) {
						currAngle = 270.0f;
					} else {
						currAngle = 90.0f;
					}
				}

				if (lastCosTheta != 0) {
					lastAngle = acos(lastCosTheta) * 180.0 / M_PI;
					if (sinTheta < 0) {
						lastAngle = 360.0f - lastAngle;
					}
				} else {
					if (lastSinTheta < 0) {
						lastAngle = 270.0f;
					} else {
						lastAngle = 90.0f;
					}
				}

				deltaAngle = currAngle - lastAngle;

				if (_toolAxis->compare("X")) {
					obj_angle_roll = fmod((obj_angle_roll + deltaAngle), 360.0 );
					//qDebug() << "roll: " << currAngle << " : " << lastAngle << " : " << deltaAngle << " : " << obj_angle_roll;

					setStale(true);
				} else
				if (_toolAxis->compare("Y")) {
					obj_angle_pitch = fmod((obj_angle_pitch + deltaAngle), 360.0 );
					//qDebug() << "pitch: " << currAngle << " : " << lastAngle << " : " << deltaAngle << " : " << obj_angle_pitch;

					setStale(true);
				} else
				if (_toolAxis->compare("Z")) {
					obj_angle_yaw = fmod((obj_angle_yaw + deltaAngle), 360.0 );
					//qDebug() << "yaw: " << currAngle << " : " << lastAngle << " : " << deltaAngle << " : " << obj_angle_yaw;

					setStale(true);
				}

			}
		}

		lastTouchX = currTouchX;
		lastTouchY = currTouchY;
	}
	*/
}

void Photo3DView::handleScreenEvent(bps_event_t *event) {
    int screenEvent;
    int buttons;
    int position[2];

    screen_event_t screen_event = screen_event_get_event(event);

    //Query type of screen event and its location on the screen
    screen_get_event_property_iv(screen_event, SCREEN_PROPERTY_TYPE,
            &screenEvent);
    screen_get_event_property_iv(screen_event, SCREEN_PROPERTY_SOURCE_POSITION,
    		position);

	switch (screenEvent) {
		case SCREEN_EVENT_MTOUCH_TOUCH:
			break;

		case SCREEN_EVENT_MTOUCH_MOVE:
			break;

		case SCREEN_EVENT_MTOUCH_RELEASE:
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
