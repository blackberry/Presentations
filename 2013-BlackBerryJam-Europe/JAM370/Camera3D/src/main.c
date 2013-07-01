/*
 * Copyright (c) 2013 Research In Motion Limited.
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

#include <screen/screen.h>
#include <bps/navigator.h>
#include <bps/screen.h>
#include <bps/bps.h>
#include <bps/event.h>
#include <bps/orientation.h>
#include <stdarg.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <math.h>
#include <camera/camera_api.h>
#include <pthread.h>
#include <string.h>
#include <sys/neutrino.h>

#include <EGL/egl.h>
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>

#include "bbutil.h"
#include "matrix.h"

/* TODO:
 * Change plain cube shader to use a spotlight (and add normals)
 * Cleanup code
 * Rotate camera when device rotates
 * Figure out why FPS is inconsistent (one moment it's 40-50 FPS, the next it's 20). It's not like the filter is being changed.
 *
 * Additional (GL):
 * Switch to VBOs for normals and vertices (don't do VAOs as the shaders could change what vert att is used)
 * Render to framebuffers to do MSAA (but don't try rendering the texture to the cube, that will just cause problems)
 *
 * Other:
 * Flip UVs depending on if rear or front camera is being used (need the APIs to do so)
 */

//Uncomment this when debugging is desired
#define DEBUG_INFO_PRINTOUT

//Uncomment this to have a FPS count printed to console
#define DEBUG_FPS

//OpenGL variables
static GLfloat radio_btn_unselected_vertices[8], radio_btn_selected_vertices[8],
        background_portrait_vertices[8], background_landscape_vertices[8],
        *background_vertices;
static GLfloat radio_btn_unselected_tex_coord[8],
        radio_btn_selected_tex_coord[8], background_portrait_tex_coord[8],
        background_landscape_tex_coord[8], *background_tex_coord;
static GLuint radio_btn_unselected, radio_btn_selected, background_landscape,
        background_portrait, background;
static GLfloat* matrix_menu_projection, *matrix_menu_modelView,
		*matrix_cube_projection, *matrix_cube_modelView;
static GLuint program_menu;
static GLuint programs_cube[4];
static screen_context_t screen_cxt;
static font_t* font;
static float width, height, angle;
static int shutdown, menu_active, menu_hide_animation, menu_show_animation;
static int selected;
static float menu_animation, menu_height, button_size_x, button_size_y;
static float pos_x, pos_y;
static float cube_pos_x, cube_pos_y, cube_pos_z;

//Camera variables
static GLuint textureID;

static pthread_mutex_t bufMutex = PTHREAD_MUTEX_INITIALIZER;
static pthread_cond_t bufCond = PTHREAD_COND_INITIALIZER;
static camera_buffer_t *bufQueue = NULL;
static camera_buffer_t *cameraBuf = NULL;

static camera_handle_t handle;
static pthread_t vf_tid;
static int vf_chid;
static int vf_coid;
static struct sigevent vf_sigev;
#define VF_PULSE_CODE  (123)
static bool vf_stop = false;


GLfloat light_ambient[] = { 0.5f, 0.5f, 0.5f, 1.0f };
GLfloat light_diffuse[] = { 0.8f, 0.8f, 0.8f, 1.0f };
GLfloat light_pos[] = { 0.0f, 25.0f, 0.0f, 1.0f };
GLfloat light_direction[] = { 0.0f, 0.0f, -30.0f, 1.0f };

static float cube_vertices[] = {
        // FRONT
        -2.0f, -2.0f, 2.0f,
        2.0f, -2.0f, 2.0f,
        -2.0f, 2.0f, 2.0f,
        2.0f, 2.0f, 2.0f,
        // BACK
        -2.0f, -2.0f, -2.0f,
        -2.0f, 2.0f, -2.0f,
        2.0f, -2.0f, -2.0f,
        2.0f, 2.0f, -2.0f,
        // LEFT
        -2.0f, -2.0f, 2.0f,
        -2.0f, 2.0f, 2.0f,
        -2.0f, -2.0f, -2.0f,
        -2.0f, 2.0f, -2.0f,
        // RIGHT
        2.0f, -2.0f, -2.0f,
        2.0f, 2.0f, -2.0f,
        2.0f, -2.0f, 2.0f,
        2.0f, 2.0f, 2.0f,
        // TOP
        -2.0f, 2.0f, 2.0f,
        2.0f, 2.0f, 2.0f,
        -2.0f, 2.0f, -2.0f,
        2.0f, 2.0f, -2.0f,
        // BOTTOM
        -2.0f, -2.0f, 2.0f,
        -2.0f, -2.0f, -2.0f,
        2.0f, -2.0f, 2.0f,
        2.0f, -2.0f, -2.0f };

float cube_normals[] = {
        // FRONT
        0.0f, 0.0f, 1.0f,
        0.0f, 0.0f, 1.0f,
        0.0f, 0.0f, 1.0f,
        0.0f, 0.0f, 1.0f,
        // BACK
        0.0f, 0.0f, -1.0f,
        0.0f, 0.0f, -1.0f,
        0.0f, 0.0f, -1.0f,
        0.0f, 0.0f, -1.0f,
        // LEFT
        -1.0f, 0.0f, 0.0f,
        -1.0f, 0.0f, 0.0f,
        -1.0f, 0.0f, 0.0f,
        -1.0f, 0.0f, 0.0f,
        // RIGHT
        1.0f, 0.0f, 0.0f,
        1.0f, 0.0f, 0.0f,
        1.0f, 0.0f, 0.0f,
        1.0f, 0.0f, 0.0f,
        // TOP
        0.0f, 1.0f, 0.0f,
        0.0f, 1.0f, 0.0f,
        0.0f, 1.0f, 0.0f,
        0.0f, 1.0f, 0.0f,
        // BOTTOM
        0.0f, -1.0f, 0.0f,
        0.0f, -1.0f, 0.0f,
        0.0f, -1.0f, 0.0f,
        0.0f, -1.0f, 0.0f };

// note: since the textures are being cropped it may make sense to scale these values appropriately
float cube_tex_coords[] = {
		1.0f, 1.0f,	0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, //Front
		0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 0.0f,	//Back
		0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 0.0f,	//Left
		0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 0.0f,	//Right
		1.0f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f,	0.0f, 0.0f, //Top
		1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f};//Bottom
#define CUBE_TEX_COORDS_QUAD_COUNT (((sizeof(cube_tex_coords) / sizeof(*cube_tex_coords)) / 2) / 4)

int resize();
void update();
void render();
int read_from_file();
void save_to_file();

void handleClick(int x, int y) {
    if (menu_active) {
        if ((y > menu_height - 4 * button_size_y)
                && (y < menu_height - 3 * button_size_y) && (x > 0)
                && (x < button_size_x)) {
            selected = 3;
        } else if ((y > menu_height - 3 * button_size_y)
                && (y < menu_height - 2 * button_size_y) && (x > 0)
                && (x < button_size_x)) {
            selected = 2;
        } else if ((y > menu_height - 2 * button_size_y)
                && (y < menu_height - button_size_y) && (x > 0)
                && (x < button_size_x)) {
            selected = 1;
        } else if ((y > menu_height - button_size_y) && (y < menu_height)
                && (x > 0) && (x < button_size_x)) {
            selected = 0;
        } else {
            menu_hide_animation = true;
            menu_show_animation = false;
            menu_active = false;
        }

        //Save current state to a file
        save_to_file();
    }
}

static void handleScreenEvent(bps_event_t *event) {
    int screen_val, buttons;
    int pair[2];

    static bool mouse_pressed = false;

    screen_event_t screen_event = screen_event_get_event(event);

    //Query type of screen event and its location on the screen
    screen_get_event_property_iv(screen_event, SCREEN_PROPERTY_TYPE, &screen_val);
    screen_get_event_property_iv(screen_event, SCREEN_PROPERTY_SOURCE_POSITION, pair);

    //There is a difference between touch screen events and mouse events
    if (screen_val == SCREEN_EVENT_MTOUCH_RELEASE) {
        //Handle touch screen event
        handleClick(pair[0], pair[1]);

    } else if (screen_val == SCREEN_EVENT_POINTER) {
        //This is a mouse move event, it is applicable to a device with a usb mouse or simulator
        screen_get_event_property_iv(screen_event, SCREEN_PROPERTY_BUTTONS, &buttons);

        if (buttons == SCREEN_LEFT_MOUSE_BUTTON) {
            //Left mouse button is pressed
            mouse_pressed = true;
        } else {
            if (mouse_pressed) {
                //Left mouse button was released, handle left click
                handleClick(pair[0], pair[1]);
                mouse_pressed = false;
            }
        }
    }
}

static void handleNavigatorEvent(bps_event_t *event) {
    switch (bps_event_get_code(event)) {
    case NAVIGATOR_ORIENTATION_CHECK:
        //Signal navigator that we intend to resize
        navigator_orientation_check_response(event, true);
        break;
    case NAVIGATOR_ORIENTATION:
    	//XXX Rotate camera
        if (EXIT_FAILURE == resize(event)) {
            shutdown = true;
        }
        break;
    case NAVIGATOR_SWIPE_DOWN:
        menu_show_animation = true;
        menu_hide_animation = false;
        break;
    case NAVIGATOR_EXIT:
        shutdown = true;
        break;
    case NAVIGATOR_WINDOW_INACTIVE:
        //Wait for NAVIGATOR_WINDOW_ACTIVE event
        for (;;) {
            if (BPS_SUCCESS != bps_get_event(&event, -1)) {
                fprintf(stderr, "bps_get_event failed\n");
                break;
            }

            if (event && (bps_event_get_domain(event) == navigator_get_domain())) {
                int code = bps_event_get_code(event);
                if (code == NAVIGATOR_EXIT) {
                    shutdown = true;
                    break;
                } else if (code == NAVIGATOR_WINDOW_ACTIVE) {
                    break;
                }
            }
        }
        break;
    }
}

static void handle_events() {
    //Request and process available BPS events
    for(;;) {
        bps_event_t *event = NULL;
        if (BPS_SUCCESS != bps_get_event(&event, 0)) {
            fprintf(stderr, "bps_get_event failed\n");
            break;
        }

        if (event) {
            int domain = bps_event_get_domain(event);

            if (domain == screen_get_domain()) {
                handleScreenEvent(event);
            } else if (domain == navigator_get_domain()) {
                handleNavigatorEvent(event);
            }
        } else {
            break;
        }
    }
}

int resize(bps_event_t *event) {
    //Query width and height of the window surface created by utility code
    EGLint surface_width, surface_height;

    if (event) {
        int angle = navigator_event_get_orientation_angle(event);

        //Let bbutil rotate current screen surface to this angle
        if (EXIT_FAILURE == bbutil_rotate_screen_surface(angle)) {
            fprintf(stderr, "Unable to handle orientation change\n");
            return EXIT_FAILURE;
        }
    }

    eglQuerySurface(egl_disp, egl_surf, EGL_WIDTH, &surface_width);
    eglQuerySurface(egl_disp, egl_surf, EGL_HEIGHT, &surface_height);

    EGLint err = eglGetError();
    if (err != 0x3000) {
        fprintf(stderr, "Unable to query EGL surface dimensions\n");
        return EXIT_FAILURE;
    }

    width = (float) surface_width;
    height = (float) surface_height;

    if(matrix_menu_projection)
    {
    	matrix_free(matrix_menu_projection);
    	matrix_menu_projection = NULL;
    }
    if(matrix_menu_modelView)
	{
		matrix_free(matrix_menu_modelView);
		matrix_menu_modelView = NULL;
	}
    if(matrix_cube_projection)
	{
		matrix_free(matrix_cube_projection);
		matrix_cube_projection = NULL;
	}
	if(matrix_cube_modelView)
	{
		matrix_free(matrix_cube_modelView);
		matrix_cube_modelView = NULL;
	}

    if (width > height) {
        cube_pos_x = 2.9f;
        cube_pos_y = 0.1f;
        cube_pos_z = -20.0f;

        background = background_landscape;
        background_vertices = background_landscape_vertices;
        background_tex_coord = background_landscape_tex_coord;

    } else {
        cube_pos_x = 0.5f;
        cube_pos_y = -1.0f;
        cube_pos_z = -30.0f;

        background = background_portrait;
        background_vertices = background_portrait_vertices;
        background_tex_coord = background_portrait_tex_coord;
    }

    glViewport(0, 0, (int) width, (int) height);

    update();

    if (event) {
        render();

        navigator_done_orientation(event);
    }

    return EXIT_SUCCESS;
}

GLuint loadShader(const char* v_source, const char* f_source)
{
	GLint status;

	// Compile the vertex shader
	GLuint vs = glCreateShader(GL_VERTEX_SHADER);

	if (!vs) {
		fprintf(stderr, "Failed to create vertex shader: %d\n", glGetError());
		return 0;
	} else {
		glShaderSource(vs, 1, &v_source, 0);
		glCompileShader(vs);
		glGetShaderiv(vs, GL_COMPILE_STATUS, &status);
		if (GL_FALSE == status) {
			GLchar log[256];
			glGetShaderInfoLog(vs, 256, NULL, log);

			fprintf(stderr, "Failed to compile vertex shader: %s\n", log);

			glDeleteShader(vs);

			return 0;
		}
	}

	// Compile the fragment shader
	GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);

	if (!fs) {
		fprintf(stderr, "Failed to create fragment shader: %d\n", glGetError());
		return 0;
	} else {
		glShaderSource(fs, 1, &f_source, 0);
		glCompileShader(fs);
		glGetShaderiv(fs, GL_COMPILE_STATUS, &status);
		if (GL_FALSE == status) {
			GLchar log[256];
			glGetShaderInfoLog(fs, 256, NULL, log);

			fprintf(stderr, "Failed to compile fragment shader: %s\n", log);

			glDeleteShader(vs);
			glDeleteShader(fs);

			return 0;
		}
	}

	// Create and link the program
	GLuint program = glCreateProgram();
	if (program)
	{
		glAttachShader(program, vs);
		glAttachShader(program, fs);
		glLinkProgram(program);

		glGetProgramiv(program, GL_LINK_STATUS, &status);
		if (status == GL_FALSE)    {
			GLchar log[256];
			glGetProgramInfoLog(fs, 256, NULL, log);

			fprintf(stderr, "Failed to link text rendering shader program: %s\n", log);

			glDeleteProgram(program);
			program = 0;
		}
	} else {
		fprintf(stderr, "Failed to create a shader program\n");

		glDeleteShader(vs);
		glDeleteShader(fs);
		return 0;
	}

	// We don't need the shaders anymore - the program is enough
	glDeleteShader(fs);
	glDeleteShader(vs);

	return program;
}

int initialize() {
    EGLint surface_width, surface_height;

    //Load background and button textures
    float tex_x = 1.0f, tex_y = 1.0f;

    //Load textures for radio buttons
    int size_x = 64, size_y = 64;

    if (EXIT_SUCCESS
            != bbutil_load_texture("app/native/radio_btn_unselected.png",
                    NULL, NULL, &tex_x, &tex_y, &radio_btn_unselected)) {
        fprintf(stderr, "Unable to load non-selected radio button texture\n");
    }

    radio_btn_unselected_vertices[0] = 0.0f;
    radio_btn_unselected_vertices[1] = 0.0f;
    radio_btn_unselected_vertices[2] = size_x;
    radio_btn_unselected_vertices[3] = 0.0f;
    radio_btn_unselected_vertices[4] = 0.0f;
    radio_btn_unselected_vertices[5] = size_y;
    radio_btn_unselected_vertices[6] = size_x;
    radio_btn_unselected_vertices[7] = size_y;

    radio_btn_unselected_tex_coord[0] = 0.0f;
    radio_btn_unselected_tex_coord[1] = 0.0f;
    radio_btn_unselected_tex_coord[2] = tex_x;
    radio_btn_unselected_tex_coord[3] = 0.0f;
    radio_btn_unselected_tex_coord[4] = 0.0f;
    radio_btn_unselected_tex_coord[5] = tex_y;
    radio_btn_unselected_tex_coord[6] = tex_x;
    radio_btn_unselected_tex_coord[7] = tex_y;

    if (EXIT_SUCCESS
            != bbutil_load_texture("app/native/radio_btn_selected.png", NULL,
                    NULL, &tex_x, &tex_y, &radio_btn_selected)) {
        fprintf(stderr, "Unable to load selected radio button texture\n");
    }

    radio_btn_selected_vertices[0] = 0.0f;
    radio_btn_selected_vertices[1] = 0.0f;
    radio_btn_selected_vertices[2] = size_x;
    radio_btn_selected_vertices[3] = 0.0f;
    radio_btn_selected_vertices[4] = 0.0f;
    radio_btn_selected_vertices[5] = size_y;
    radio_btn_selected_vertices[6] = size_x;
    radio_btn_selected_vertices[7] = size_y;

    radio_btn_selected_tex_coord[0] = 0.0f;
    radio_btn_selected_tex_coord[1] = 0.0f;
    radio_btn_selected_tex_coord[2] = tex_x;
    radio_btn_selected_tex_coord[3] = 0.0f;
    radio_btn_selected_tex_coord[4] = 0.0f;
    radio_btn_selected_tex_coord[5] = tex_y;
    radio_btn_selected_tex_coord[6] = tex_x;
    radio_btn_selected_tex_coord[7] = tex_y;

    button_size_x = (float) size_x;
    button_size_y = (float) size_y;

    eglQuerySurface(egl_disp, egl_surf, EGL_WIDTH, &surface_width);
    eglQuerySurface(egl_disp, egl_surf, EGL_HEIGHT, &surface_height);

    EGLint err = eglGetError();
    if (err != 0x3000) {
        fprintf(stderr, "Unable to query EGL surface dimensions\n");
        return EXIT_FAILURE;
    }

    width = (float) surface_width;
    height = (float) surface_height;

    if (EXIT_SUCCESS
            != bbutil_load_texture("app/native/background-landscape.png", NULL, NULL,
                    &tex_x, &tex_y, &background_landscape)) {
        fprintf(stderr, "Unable to load landscape background texture\n");
    }

    size_x = (width > height) ? width : height;
    size_y = (width > height) ? height : width;

    background_landscape_vertices[0] = 0.0f;
    background_landscape_vertices[1] = 0.0f;
    background_landscape_vertices[2] = size_x;
    background_landscape_vertices[3] = 0.0f;
    background_landscape_vertices[4] = 0.0f;
    background_landscape_vertices[5] = size_y;
    background_landscape_vertices[6] = size_x;
    background_landscape_vertices[7] = size_y;

    background_landscape_tex_coord[0] = 0.0f;
    background_landscape_tex_coord[1] = 0.0f;
    background_landscape_tex_coord[2] = tex_x;
    background_landscape_tex_coord[3] = 0.0f;
    background_landscape_tex_coord[4] = 0.0f;
    background_landscape_tex_coord[5] = tex_y;
    background_landscape_tex_coord[6] = tex_x;
    background_landscape_tex_coord[7] = tex_y;

    if (EXIT_SUCCESS
            != bbutil_load_texture("app/native/background-portrait.png", NULL, NULL,
                    &tex_x, &tex_y, &background_portrait)) {
        fprintf(stderr, "Unable to load portrait background texture\n");
    }

    size_x = (height > width) ? width : height;
    size_y = (height > width) ? height : width;

    background_portrait_vertices[0] = 0.0f;
    background_portrait_vertices[1] = 0.0f;
    background_portrait_vertices[2] = size_x;
    background_portrait_vertices[3] = 0.0f;
    background_portrait_vertices[4] = 0.0f;
    background_portrait_vertices[5] = size_y;
    background_portrait_vertices[6] = size_x;
    background_portrait_vertices[7] = size_y;

    background_portrait_tex_coord[0] = 0.0f;
    background_portrait_tex_coord[1] = 0.0f;
    background_portrait_tex_coord[2] = tex_x;
    background_portrait_tex_coord[3] = 0.0f;
    background_portrait_tex_coord[4] = 0.0f;
    background_portrait_tex_coord[5] = tex_y;
    background_portrait_tex_coord[6] = tex_x;
    background_portrait_tex_coord[7] = tex_y;

    angle = 0.0f;
    pos_x = 0.0f;
    pos_y = 0.0f;

    //Load a typical arial font to use for our color menu
    int dpi = bbutil_calculate_dpi(screen_cxt);

    if (dpi == EXIT_FAILURE) {
        fprintf(stderr, "Unable to calculate dpi\n");
        return EXIT_FAILURE;
    }

    //As bbutil renders text using device-specifc dpi, we need to compute a point size
    //for the font, so that the text string fits into the bubble. Note that Playbook is used
    //as a reference point in this equation as we know that at dpi of 170, font with point size of
    //15 fits into the bubble texture.

    int point_size = (int)(15.0f / ((float)dpi / 170.0f ));

    font = bbutil_load_font("/usr/fonts/font_repository/monotype/arial.ttf", point_size, dpi);

    if (!font) {
       return EXIT_FAILURE;
    }

    float text_width, text_height;
    bbutil_measure_text(font, "Color Menu", &text_width, &text_height);
    menu_height = text_height + 10.0f + button_size_y * 4;

    //See if a savefile exists. If not, initialize to a hidden menu and a red cube.
    if (!read_from_file()) {
        selected = 3;

        menu_animation = 0.0f;
        menu_active = false;
        menu_show_animation = false;
        menu_hide_animation = false;
    }

    //Initialize positions of graphics assets on the screen, but don't resize the surface
    if (EXIT_FAILURE == resize(NULL)) {
        fprintf(stderr, "Initialize surface\n");
        return EXIT_FAILURE;
    }

    // CUBE CAMERA TEXTURE SETUP?
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);

    //Common gl setup
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

    glEnable(GL_CULL_FACE);

    const char* vSource_menu =
   			"attribute vec2 vertexPosition;"
   			"attribute vec2 uvPosition;"
   			"uniform mat4 modelViewMatrix;"
   			"uniform mat4 perspectiveMatrix;"
   			"varying vec2 uv;"
   			"void main()"
   			"{"
   			"    gl_Position = perspectiveMatrix * modelViewMatrix * vec4(vertexPosition, 0.0, 1.0);"
   			"    uv = uvPosition;"
   			"}";

   	const char* fSource_menu =
   			"#ifdef GL_ES\r\n"
   			"    #ifdef GL_FRAGMENT_PRECISION_HIGH\r\n"
   			"        precision highp float;\r\n"
   			"    #else\r\n"
   			"        precision mediump float;\r\n"
   			"    #endif\r\n"
   			"#endif\r\n"
   			"varying vec2 uv;"
   			"uniform sampler2D tex;"
   			"void main()"
   			"{"
   			"    gl_FragColor = texture2D(tex, uv);"
   			"}";

   	const char* vSource_cube =
   			"attribute vec3 vertexPosition;"
			"attribute vec2 uvPosition;"
			"uniform mat4 modelViewMatrix;"
			"uniform mat4 perspectiveMatrix;"
			"varying vec2 uv;"
			"void main()"
			"{"
			"    gl_Position = perspectiveMatrix * modelViewMatrix * vec4(vertexPosition, 1.0);"
   			"    uv = uvPosition;"
			"}";

   	const char* fSource_cube =
			"#ifdef GL_ES\r\n"
			"    #ifdef GL_FRAGMENT_PRECISION_HIGH\r\n"
			"        precision highp float;\r\n"
			"    #else\r\n"
			"        precision mediump float;\r\n"
			"    #endif\r\n"
			"#endif\r\n"
			"varying vec2 uv;"
			"uniform sampler2D tex;"
			"void main()"
			"{"
			"    gl_FragColor = texture2D(tex, uv);"
			"}";

   	//Based off http://coding-experiments.blogspot.com/2010/06/edge-detection.html
   	const char* fSource_cube_edge =
			"#ifdef GL_ES\r\n"
			"    #ifdef GL_FRAGMENT_PRECISION_HIGH\r\n"
			"        precision highp float;\r\n"
			"    #else\r\n"
			"        precision mediump float;\r\n"
			"    #endif\r\n"
			"#endif\r\n"
			"varying vec2 uv;"
			"uniform sampler2D tex;"
   			"uniform vec2 imageSize;"
   			"const float EDGE_DELTA_EMPHASIS = 2.3;"
   			"float threshold(in float thr1, in float thr2 , in float val) {"
			"    if (val < thr1) {return 0.0;}"
			"    if (val > thr2) {return 1.0;}"
			"    return val;"
   			"}"
   			"float get_pixel(in vec2 coords, in float dx, in float dy) {"
   			"    vec4 color = texture2D(tex, coords + vec2(dx, dy));"
   			"    // averaged pixel intensity from 3 color channels\r\n"
   			"    return (color.r + color.g + color.b) / 3.0;"
   			"}"
   			"// returns pixel color\r\n"
   			"float IsEdge(in vec2 coords, in float dxtex, in float dytex) {"
			"    float pix[8];"
			"    float delta;"
			"    // read neighboring pixel intensities (unwrapped loop to speed up)\r\n"
   			"    pix[0] = get_pixel(coords,-dxtex,-dytex);"
   			"    pix[1] = get_pixel(coords,-dxtex,0.0);"
   			"    pix[2] = get_pixel(coords,-dxtex,dytex);"
   			"    pix[3] = get_pixel(coords,0.0,-dytex);"
			"    pix[4] = get_pixel(coords,0.0,dytex);"
   			"    pix[5] = get_pixel(coords,dxtex,-dytex);"
			"    pix[6] = get_pixel(coords,dxtex,0.0);"
			"    pix[7] = get_pixel(coords,dxtex,dytex);"
			"    // average color differences around neighboring pixels\r\n"
			"    delta = (abs(pix[1]-pix[6])+"
			"            abs(pix[4]-pix[3]) +"
			"            abs(pix[0]-pix[7])+"
			"            abs(pix[2]-pix[5])"
			"            )/4.0;"
			"    return threshold(0.25,0.4,clamp(EDGE_DELTA_EMPHASIS*delta,0.0,1.0));"
   			"}"
			"void main() {"
			"    float dxtex = 1.0 / imageSize.x; //image width\r\n"
			"    float dytex = 1.0 / imageSize.y; //image height\r\n"
   			"    vec4 color = vec4(0.0,0.0,0.0,1.0);"
   			"    color.g = IsEdge(uv, dxtex, dytex);"
   			"    gl_FragColor = color;"
			"}";

   	const char* fSource_cube_grey =
			"#ifdef GL_ES\r\n"
			"    #ifdef GL_FRAGMENT_PRECISION_HIGH\r\n"
			"        precision highp float;\r\n"
			"    #else\r\n"
			"        precision mediump float;\r\n"
			"    #endif\r\n"
			"#endif\r\n"
			"varying vec2 uv;"
			"uniform sampler2D tex;"
			"void main()"
			"{"
   			"    vec4 color = texture2D(tex, uv);"
   			"    float avg = (color.r + color.g + color.b) / 3.0;"
			"    gl_FragColor = vec4(vec3(avg), 1.0);"
			"}";

   	//From GPUImageSwirlFilter from https://github.com/BradLarson/GPUImage
   	const char* fSource_cube_swirl =
			"#ifdef GL_ES\r\n"
			"    #ifdef GL_FRAGMENT_PRECISION_HIGH\r\n"
			"        precision highp float;\r\n"
			"    #else\r\n"
			"        precision mediump float;\r\n"
			"    #endif\r\n"
			"#endif\r\n"
			"varying vec2 uv;"
			"uniform sampler2D tex;"
   			"const vec2 center = vec2(0.5, 0.5);"
   			"const float radius = 0.5;"
   			"const float angle = 1.0;"
			"void main()"
			"{"
			"    vec2 textureCoordinateToUse = uv;"
			"    float dist = distance(center, uv);"
   			"    textureCoordinateToUse -= center;"
   			"    if (dist < radius)"
   			"    {"
   			"        float percent = (radius - dist) / radius;"
   			"        float theta = percent * percent * angle * 8.0;"
   			"        float s = sin(theta);"
   			"        float c = cos(theta);"
   			"        textureCoordinateToUse = vec2(dot(textureCoordinateToUse, vec2(c, -s)), dot(textureCoordinateToUse, vec2(s, c)));"
   			"    }"
   			"    textureCoordinateToUse += center;"
   			"    gl_FragColor = texture2D(tex, textureCoordinateToUse);"
			"}";

    program_menu = loadShader(vSource_menu, fSource_menu);
    if(program_menu == 0) {
    	fprintf(stderr, "Initialize menu program\n");
		return EXIT_FAILURE;
    }

    programs_cube[3] = loadShader(vSource_cube, fSource_cube);
	if(programs_cube[3] == 0) {
		fprintf(stderr, "Initialize cube program\n");
		return EXIT_FAILURE;
	}

	programs_cube[2] = loadShader(vSource_cube, fSource_cube_edge);
	if(programs_cube[2] == 0) {
		fprintf(stderr, "Initialize cube program (edge)\n");
		return EXIT_FAILURE;
	}

	programs_cube[1] = loadShader(vSource_cube, fSource_cube_grey);
	if(programs_cube[1] == 0) {
		fprintf(stderr, "Initialize cube program (greyscale)\n");
		return EXIT_FAILURE;
	}

	programs_cube[0] = loadShader(vSource_cube, fSource_cube_swirl);
	if(programs_cube[0] == 0) {
		fprintf(stderr, "Initialize cube program (swirl)\n");
		return EXIT_FAILURE;
	}

    matrix_menu_projection = NULL;
    matrix_menu_modelView = NULL;
    matrix_cube_projection = NULL;
    matrix_cube_modelView = NULL;

    menu_show_animation = true;
    return EXIT_SUCCESS;
}

matrix4f createMatrix_menu_projection()
{
	return matrix_ortho(0.0f, width / height, 0.0f, 1.0f, -1.0f, 1.0f);
}

matrix4f createMatrix_menu_modelView()
{
	return matrix_scale(1.0f / height, 1.0f / height, 1.0f);
}

matrix4f createMatrix_3d_projection()
{
	GLfloat aspect_ratio = width / height;

	GLfloat fovy = 20.0f;
	GLfloat zNear = 1.0f;
	GLfloat zFar = 1000.0f;

	GLfloat top = tan(fovy * 0.0087266462599716478846184538424431f) * zNear;
	GLfloat bottom = -top;

	return matrix_frustum(aspect_ratio * bottom, aspect_ratio * top, bottom, top, zNear, zFar);
}

void update() {
    angle = fmod((angle + 1.0f), 360.0 );

    if (menu_show_animation) {
        if (menu_animation < menu_height) {
            menu_animation += 7.0f;
        } else {
            menu_show_animation = false;
            menu_active = true;

            //Save current state to a file
            save_to_file();
        }
    } else if (menu_hide_animation) {
        if (menu_animation > 0.0f) {
            menu_animation -= 7.0f;
        } else {
            menu_hide_animation = false;
        }
    }

    pos_y = height - menu_animation;
}

unsigned long lower_power_of_two(unsigned long v)
{
    bool p2 = (v & (v - 1)) == 0;
    v--;
    v |= v >> 1;
    v |= v >> 2;
    v |= v >> 4;
    v |= v >> 8;
    v |= v >> 16;
    v++;
    if (!p2) v >>= 1;
    return v;
}

void render() {
    int i;

    //Typical render pass
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    //Setup matrices for menu
    if(!matrix_menu_projection)
	{
		matrix_menu_projection = createMatrix_menu_projection();
	}
    if(!matrix_menu_modelView)
	{
		matrix_menu_modelView = createMatrix_menu_modelView();
	}

    //First render background and menu if it is enabled
    glUseProgram(program_menu);

    GLint pAtt = glGetUniformLocation(program_menu, "perspectiveMatrix");
	glUniformMatrix4fv(pAtt, 1, GL_FALSE, matrix_menu_projection);

    GLint mvAtt = glGetUniformLocation(program_menu, "modelViewMatrix");
	glUniformMatrix4fv(mvAtt, 1, GL_FALSE, matrix_menu_modelView);

    GLint tAtt = glGetUniformLocation(program_menu, "tex");
    glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, background);
	glUniform1i(tAtt, 0);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    GLint vertAtt = glGetAttribLocation(program_menu, "vertexPosition");
	glEnableVertexAttribArray(vertAtt);
	GLint uvAtt = glGetAttribLocation(program_menu, "uvPosition");
	glEnableVertexAttribArray(uvAtt);

	glVertexAttribPointer(vertAtt, 2, GL_FLOAT, GL_FALSE, 0, background_vertices);
	glVertexAttribPointer(uvAtt, 2, GL_FLOAT, GL_FALSE, 0, background_tex_coord);

    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

    if (menu_active || menu_show_animation || menu_hide_animation) {
    	GLfloat x = pos_x, y = pos_y;
        matrix4f menuMat = matrix_multiply_delete(matrix_menu_modelView, FALSE, matrix_translate(pos_x, pos_y, 0.0f), TRUE);
        glUniformMatrix4fv(mvAtt, 1, GL_FALSE, menuMat);

        for (i = 0; i < 4; i++) {
            if (i == selected) {
            	glVertexAttribPointer(vertAtt, 2, GL_FLOAT, GL_FALSE, 0, radio_btn_selected_vertices);
				glVertexAttribPointer(uvAtt, 2, GL_FLOAT, GL_FALSE, 0, radio_btn_selected_tex_coord);
				glBindTexture(GL_TEXTURE_2D, radio_btn_selected);
            } else {
            	glVertexAttribPointer(vertAtt, 2, GL_FLOAT, GL_FALSE, 0, radio_btn_unselected_vertices);
				glVertexAttribPointer(uvAtt, 2, GL_FLOAT, GL_FALSE, 0, radio_btn_unselected_tex_coord);
				glBindTexture(GL_TEXTURE_2D, radio_btn_unselected);
            }

            glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
            menuMat = matrix_multiply_delete(menuMat, TRUE, matrix_translate(0.0f, 60.0f, 0.0f), TRUE);
            glUniformMatrix4fv(mvAtt, 1, GL_FALSE, menuMat);
        }

        matrix_get_translation(menuMat, &x, &y, NULL);
        x *= width;
        y *= height;

        bbutil_render_text(font, "Effect Menu",	10.0f + x, 10.0f + y,	0.35f, 0.35f, 0.35f, 1.0f);
        bbutil_render_text(font, "No effect",	70.0f + x, -40.0f + y,	0.35f, 0.35f, 0.35f, 1.0f);
        bbutil_render_text(font, "Edge detect",	70.0f + x, -100.0f + y,	0.35f, 0.35f, 0.35f, 1.0f);
        bbutil_render_text(font, "Greyscale",	70.0f + x, -160.0f + y,	0.35f, 0.35f, 0.35f, 1.0f);
        bbutil_render_text(font, "Swirl",		70.0f + x, -220.0f + y,	0.35f, 0.35f, 0.35f, 1.0f);

        matrix_free(menuMat);
    }

    glDisableVertexAttribArray(uvAtt);
    glDisableVertexAttribArray(vertAtt);

    glBindTexture(GL_TEXTURE_2D, 0);

    //Setup the matrices for the cube
    if(!matrix_cube_projection)
	{
		matrix_cube_projection = createMatrix_3d_projection();
	}
	if(!matrix_cube_modelView)
	{
		matrix_cube_modelView = matrix_translate(cube_pos_x, cube_pos_y, cube_pos_z);
		matrix_cube_modelView = matrix_multiply_delete(matrix_cube_modelView, TRUE, matrix_rotate(30.0f, 1.0f, 0.0f, 0.0f), TRUE);
		matrix_cube_modelView = matrix_multiply_delete(matrix_cube_modelView, TRUE, matrix_rotate(15.0f, 0.0f, 0.0f, 1.0f), TRUE);
	}

	matrix4f mvCube = matrix_multiply_delete(matrix_cube_modelView, FALSE, matrix_rotate(angle, 0.0f, 1.0f, 0.0f), TRUE);

	GLuint selectedCubeShader = programs_cube[selected];
	glUseProgram(selectedCubeShader);

	pAtt = glGetUniformLocation(selectedCubeShader, "perspectiveMatrix");
	glUniformMatrix4fv(pAtt, 1, GL_FALSE, matrix_cube_projection);

	mvAtt = glGetUniformLocation(selectedCubeShader, "modelViewMatrix");
	glUniformMatrix4fv(mvAtt, 1, GL_FALSE, mvCube);

	//Setup lighting
	GLint lightAtt = glGetUniformLocation(selectedCubeShader, "lightAmbient");
	if(lightAtt >= 0)
	{
		glUniform4fv(lightAtt, 1, light_ambient); //XXX GL_AMBIENT
	}
	lightAtt = glGetUniformLocation(selectedCubeShader, "lightDiffuse");
	if(lightAtt >= 0)
	{
		glUniform4fv(lightAtt, 1, light_diffuse); //XXX GL_DIFFUSE
	}
	lightAtt = glGetUniformLocation(selectedCubeShader, "lightPosition");
	if(lightAtt >= 0)
	{
		glUniform4fv(lightAtt, 1, light_pos); //XXX GL_POSITION
	}
	lightAtt = glGetUniformLocation(selectedCubeShader, "lightSpotDirection");
	if(lightAtt >= 0)
	{
		glUniform4fv(lightAtt, 1, light_direction); //XXX GL_SPOT_DIRECTION
	}

    // regenerate texture
    pthread_mutex_lock(&bufMutex);
    if (!bufQueue && !cameraBuf) {
        uint64_t w1,w2;
        ClockTime(CLOCK_MONOTONIC, NULL, &w1);
        pthread_cond_wait(&bufCond, &bufMutex);
        ClockTime(CLOCK_MONOTONIC, NULL, &w2);
#ifdef DEBUG_INFO_PRINTOUT
        fprintf(stderr, "waited %lld uS\n", (w2-w1)/1000);
#endif
        if (!bufQueue && !cameraBuf) {
#ifdef DEBUG_INFO_PRINTOUT
            fprintf(stderr, "no buffers\n");
#endif
            pthread_mutex_unlock(&bufMutex);
            return;
        }
    }
    bool reuse = false;
    if (bufQueue) {
        if (cameraBuf) {
            camera_return_buffer(handle, cameraBuf);
            free(cameraBuf);
        }
        cameraBuf = bufQueue;
        bufQueue = NULL;
    } else {
        // no new frame has arrive.. re-use last
#ifdef DEBUG_INFO_PRINTOUT
        fprintf(stderr, "reuse\n");
#endif
        reuse = true;
    }
    pthread_mutex_unlock(&bufMutex);

    glBindTexture(GL_TEXTURE_2D, textureID);

    int w = cameraBuf->framedesc.rgb8888.stride/4;
    int h = cameraBuf->framedesc.rgb8888.height;
    if (!reuse) {
		static int textureInit = 0;
		if(textureInit == 0) {
			//Setup the texture
			textureInit = 1;
			glTexImage2D(GL_TEXTURE_2D, 0, GL_BGRA, w, h, 0, GL_BGRA, GL_UNSIGNED_BYTE, cameraBuf->framebuf);
		} else {
			//Does the same as glTexImage2D, but doesn't allocate the space for the image (that's what the first call is for)
			glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, w, h, GL_BGRA, GL_UNSIGNED_BYTE, cameraBuf->framebuf);
		}
    }

    // TODO: I am told that we can use the EGLImage extension to bypass the texture uploads, but we need a pixmap
    // in order to do so.  It's presently probably not possible to remap the camera_buffer_t into a pixmap without
    // some additional work from the graphics team.

    GLint texSize = glGetUniformLocation(selectedCubeShader, "imageSize");
	if(texSize >= 0) {
		GLfloat imgSize[] = {cameraBuf->framedesc.rgb8888.width, cameraBuf->framedesc.rgb8888.height};
		glUniform2fv(texSize, 1, imgSize);
	}

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); // set up settings
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); // set up some more settings
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE); // set up some more settings
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE); // set up some more settings

    vertAtt = glGetAttribLocation(selectedCubeShader, "vertexPosition");
	glEnableVertexAttribArray(vertAtt);
	GLint normAtt = glGetAttribLocation(selectedCubeShader, "vertexNormal");
	if(normAtt >= 0)
	{
		glEnableVertexAttribArray(normAtt);
	}
	uvAtt = glGetAttribLocation(selectedCubeShader, "uvPosition");
	glEnableVertexAttribArray(uvAtt);

    glVertexAttribPointer(vertAtt, 3, GL_FLOAT, GL_FALSE, 0, cube_vertices);

    //scale texture co-ordinates properly to deal with aspect ratio and stride
    for(i=0; i<sizeof(cube_tex_coords)/sizeof(*cube_tex_coords); i++) {
        if (cube_tex_coords[i] != 0) {
            cube_tex_coords[i] = (float)cameraBuf->framedesc.rgb8888.width / (float)w;
        }
    }

    if(normAtt >= 0)
    {
    	glVertexAttribPointer(normAtt, 3, GL_FLOAT, GL_FALSE, 0, cube_normals);
    }
	glVertexAttribPointer(uvAtt, 2, GL_FLOAT, GL_FALSE, 0, cube_tex_coords);
    tAtt = glGetUniformLocation(selectedCubeShader, "tex");
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, textureID);
	glUniform1i(tAtt, 0);

	//Then render the cube
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	glDrawArrays(GL_TRIANGLE_STRIP, 4, 4);
	glDrawArrays(GL_TRIANGLE_STRIP, 8, 4);
	glDrawArrays(GL_TRIANGLE_STRIP, 12, 4);
	glDrawArrays(GL_TRIANGLE_STRIP, 16, 4);
	glDrawArrays(GL_TRIANGLE_STRIP, 20, 4);

	glDisableVertexAttribArray(uvAtt);
	if(normAtt >= 0)
	{
		glDisableVertexAttribArray(normAtt);
	}
	glDisableVertexAttribArray(vertAtt);

	glBindTexture(GL_TEXTURE_2D, 0);

    matrix_free(mvCube);

    glUseProgram(0);

    //Use utility code to update the screen
    bbutil_swap();

    // performance:
    static uint64_t t1 = 0;
    static uint64_t t2 = 0;
    static unsigned short count = 0;
    if (++count == 30) {
        if (!t1) {
            ClockTime(CLOCK_MONOTONIC, NULL, &t1);
        } else {
            ClockTime(CLOCK_MONOTONIC, NULL, &t2);
            double fps = (t2-t1);
            fps = fps / 1000000000;
            fps = count / fps;
#if defined(DEBUG_INFO_PRINTOUT) || defined(DEBUG_FPS)
            fprintf(stderr, "RENDER @ %f fps\n", fps);
#endif
            t1 = t2;
        }
        count = 0;
    }
}

int read_from_file() {
    //open file as binary
    FILE *fp = fopen("data/save.dat", "rb");

    if (!fp) {
        return false;
    }

    int rc = fscanf(fp, "%i %i", &selected, &menu_active);

    if (rc == -1) {
        return false;
    } else {
    	if (selected < 0 || selected > 3) {
    		return false;
    	}
    }

    if (menu_active) {
        menu_animation = menu_height;
    }

    fclose(fp);
    return true;
}

void save_to_file() {
    //open file as binary
    FILE *fp = fopen("data/save.dat", "wb");

    if (!fp) {
        return;
    }

    fprintf(fp, "%i %i", selected, menu_active);

    fclose(fp);
}


static void* vf_thread(void* arg)
{
    int rcvid;
    struct _pulse pulse;
    camera_eventkey_t key;
    camera_buffer_t inbuf;

    // hook in to the viewfinder buffer stream in read-only mode.
    // when a frame becomes available, the event will be delivered to us.
    // NOTE: we should really be checking the CAMERA_FEATURE_PREVIEWISVIDEO feature
    // to make sure we are connecting to the correct image stream, but since all
    // devices that we currently build use the same frames for the viewfinder and
    // for video recording, I am not bothering to do so for the sake of clarity.
    // If the feature is reported as not available, then we would call camera_enable_video_event()
    // instead.
    if (camera_enable_viewfinder_event(handle,
                                       CAMERA_EVENTMODE_READONLY,
                                       &key,
                                       &vf_sigev) != CAMERA_EOK) {
        fprintf(stderr, "failed to attach viewfinder read/write event\n");
        return NULL;
    }
    // NOTE: this is NEW!
    // if we are likely to be handling any camera buffer data, we must be sure
    // to inform the camera service that we are using resources.
    camera_register_resource(handle);
    // now that we have registered our intent to use resources, we will be notified
    // if the camera service is going to be unmapping our buffers if they are needed
    // by a higher priority system process.  if we do not register in this way,
    // the buffers can be unmapped AT ANY TIME, causing us to crash if we try to
    // reference the buffer data.  We must be sure to de-register the resource when
    // we are finished.  If the CAMERA_STATUS_RESOURCENOTAVAIL notification is
    // received at any time, we must be sure to immediately release all buffers and
    // call camera_deregister_resource().  There is a time limit on doing this, so do it fast!

    while(!vf_stop) {
        camera_buffer_t *release = NULL;
        rcvid = MsgReceivePulse(vf_chid, &pulse, sizeof pulse, NULL);
        if (rcvid != 0) continue;   // not a pulse
        if (pulse.code != VF_PULSE_CODE) continue;  // not a pulse we can handle
        if (vf_stop) break;     // stop if we're being told to stop

        // okay, at this point, we can be sure the pulse code is meant to indicate that
        // a new frame is available from the camera to process.
        // retrieve the input buffer for processing
        camera_get_viewfinder_buffers(handle,
                                      key,
                                      &inbuf,
                                      NULL);
        pthread_mutex_lock(&bufMutex);
        // stale frame in queue.. overwrite
        if (bufQueue) {
            release = bufQueue;
        }
        bufQueue = calloc(1, inbuf.framesize);
        memcpy(bufQueue, &inbuf, inbuf.framesize);
        pthread_cond_signal(&bufCond);
        pthread_mutex_unlock(&bufMutex);
        if (release) {
#ifdef DEBUG_INFO_PRINTOUT
            fprintf(stderr,"releasing %p\n", release->framebuf);
#endif
            camera_return_buffer(handle,
                                 release);
            free(release);
        }
    }

    // de-register the viewfinder read/write event
    camera_disable_event(handle, key);
    // de-register our use of resources.  if the thread were being shut down due to the
    // CAMERA_STATUS_RESOURCENOTAVAIL status being received, this call will also signal
    // the camera service that it may proceed with revoking resources.
    camera_deregister_resource(handle);
    return NULL;
}

void rotateUVs(float* uvs, int quads, int angle)
{
	angle %= 360;
	if(angle < 0)
	{
		angle += 360;
	}
	int q;
	int a;
	for(q = 0; q < quads; q++)
	{
		for(a = 0; a < angle; a += 90)
		{
			//x.0123 -- start

			//0.1123
			float x = uvs[q * 8 + 0];
			float y = uvs[q * 8 + 1];
			uvs[q * 8 + 0] = uvs[q * 8 + 2];
			uvs[q * 8 + 1] = uvs[q * 8 + 3];

			//0.1223
			uvs[q * 8 + 2] = uvs[q * 8 + 4];
			uvs[q * 8 + 3] = uvs[q * 8 + 5];

			//0.1203
			uvs[q * 8 + 4] = x;
			uvs[q * 8 + 5] = y;

			//3.1203
			x = uvs[q * 8 + 6];
			y = uvs[q * 8 + 7];

			//3.1202
			uvs[q * 8 + 6] = uvs[q * 8 + 2];
			uvs[q * 8 + 7] = uvs[q * 8 + 3];

			//3.1302
			uvs[q * 8 + 2] = x;
			uvs[q * 8 + 3] = y;
		}
	}
}

void flipUVs(float* uvs, int quads, int vert)
{
	float x, y;
	int q;
	for(q = 0; q < quads; q++)
	{
		if(vert)
		{
			//Vert
			x = uvs[q * 8 + 0];
			y = uvs[q * 8 + 1];
			uvs[q * 8 + 0] = uvs[q * 8 + 4];
			uvs[q * 8 + 1] = uvs[q * 8 + 5];
			uvs[q * 8 + 4] = x;
			uvs[q * 8 + 5] = y;

			x = uvs[q * 8 + 2];
			y = uvs[q * 8 + 3];
			uvs[q * 8 + 2] = uvs[q * 8 + 6];
			uvs[q * 8 + 3] = uvs[q * 8 + 7];
			uvs[q * 8 + 6] = x;
			uvs[q * 8 + 7] = y;
		}
		else
		{
			//Horz
			x = uvs[q * 8 + 0];
			y = uvs[q * 8 + 1];
			uvs[q * 8 + 0] = uvs[q * 8 + 2];
			uvs[q * 8 + 1] = uvs[q * 8 + 3];
			uvs[q * 8 + 2] = x;
			uvs[q * 8 + 3] = y;

			x = uvs[q * 8 + 4];
			y = uvs[q * 8 + 5];
			uvs[q * 8 + 4] = uvs[q * 8 + 6];
			uvs[q * 8 + 5] = uvs[q * 8 + 7];
			uvs[q * 8 + 6] = x;
			uvs[q * 8 + 7] = y;
		}
	}
}

int main(int argc, char *argv[]) {
    //Create a screen context that will be used to create an EGL surface to to receive libscreen events
    screen_create_context(&screen_cxt, 0);

    //Initialize BPS library
    bps_initialize();

    //Use utility code to initialize EGL for rendering with GL ES 2.0
    if (EXIT_SUCCESS != bbutil_init_egl(screen_cxt)) {
        fprintf(stderr, "bbutil_init_egl failed\n");
        bbutil_terminate();
        screen_destroy_context(screen_cxt);
        return 0;
    }

    //Initialize application logic
    if (EXIT_SUCCESS != initialize()) {
        fprintf(stderr, "initialize failed\n");
        bbutil_terminate();
        screen_destroy_context(screen_cxt);
        return 0;
    }

    //Signal BPS library that navigator and screen events will be requested
    if (BPS_SUCCESS != screen_request_events(screen_cxt)) {
        fprintf(stderr, "screen_request_events failed\n");
        bbutil_terminate();
        screen_destroy_context(screen_cxt);
        return 0;
    }

    if (BPS_SUCCESS != navigator_request_events(0)) {
        fprintf(stderr, "navigator_request_events failed\n");
        bbutil_terminate();
        screen_destroy_context(screen_cxt);
        return 0;
    }

    //Signal BPS library that navigator orientation is not to be locked
    if (BPS_SUCCESS != navigator_rotation_lock(false)) {
        fprintf(stderr, "navigator_rotation_lock failed\n");
        bbutil_terminate();
        screen_destroy_context(screen_cxt);
        return 0;
    }

    //Open camera
    if (camera_open(CAMERA_UNIT_FRONT, CAMERA_MODE_RW, &handle)) return 0;
    unsigned int orientation = 0;
    camera_get_native_orientation(handle, &orientation);

    //Modify UVs based on camera
    rotateUVs(cube_tex_coords, CUBE_TEX_COORDS_QUAD_COUNT, orientation);
    //TODO: flip UVs depending on if rear or front camera is being used

    // going to run a query here to find the closest resolution to 640x480
    // (error checking is omitted for clarity)
    unsigned int num;
    camera_get_video_vf_resolutions(handle, 0, &num, NULL);
    camera_res_t res[num];
    camera_get_video_vf_resolutions(handle, num, &num, res);
    unsigned int best = 0;
    int i;
    for (i=0; i<num; i++) {
        fprintf(stderr, "Supported resolution: %d x %d\n", res[i].width, res[i].height);
        if (res[i].width > res[i].height) {
            if ((fabs(res[i].height - 480) <= fabs(res[best].height - 480)) &&
                (fabs(res[i].width - 640) <= fabs(res[best].width - 640))) {
                best = i;
            }
        } else {
            if ((fabs(res[i].width - 480) <= fabs(res[best].width - 480)) &&
                (fabs(res[i].height - 640) <= fabs(res[best].height - 640))) {
                best = i;
            }
        }
    }

    fprintf(stderr, "Selecting resolution %d x %d\n", res[best].width, res[best].height);

    // get camera running
    if (camera_set_videovf_property(handle,
                                    CAMERA_IMGPROP_CREATEWINDOW, 0,
                                    CAMERA_IMGPROP_FORMAT, CAMERA_FRAMETYPE_RGB8888,
                                    CAMERA_IMGPROP_FRAMERATE, 30.0,
                                    // note: native orientation gives best performance
                                    CAMERA_IMGPROP_ROTATION, orientation,
                                    // note: can use camera_get_video_vf_resolutions() to find your favorite
                                    CAMERA_IMGPROP_WIDTH, res[best].width,
                                    CAMERA_IMGPROP_HEIGHT, res[best].height)) return 0;
    if (camera_start_video_viewfinder(handle, NULL, NULL, NULL)) return 0;

    // now we're going to do something new.  we're going to spawn a thread which will
    // implement our face-blurring filter.
    // first though, let's set up the channel/connection/sigevent stuff we're going to
    // be using to communicate with the camera and with the main thread (to tell it to stop)
    vf_chid = ChannelCreate(0);
    vf_coid = ConnectAttach(0, 0, vf_chid, _NTO_SIDE_CHANNEL, 0);
    SIGEV_PULSE_INIT(&vf_sigev,
                     vf_coid,
                     SIGEV_PULSE_PRIO_INHERIT,
                     VF_PULSE_CODE,
                     0);
    pthread_create(&vf_tid, NULL, vf_thread, NULL);

    while (!shutdown) {
        // Handle user input and accelerometer
        handle_events();
        // Update scene contents
        update();
        // Draw Scene
        render();
    }

    vf_stop = true;
    MsgSendPulse(vf_coid, -1, VF_PULSE_CODE, 0);
    pthread_join(vf_tid, NULL);

    //Stop requesting events from libscreen
    screen_stop_events(screen_cxt);

    //Use utility code to terminate EGL setup
    bbutil_terminate();

    //Shut down BPS library for this process
    bps_shutdown();

    //Destroy libscreen context
    screen_destroy_context(screen_cxt);
    return 0;
}
