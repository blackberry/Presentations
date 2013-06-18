/*
 * Copyright (c) 2013 Vinnie Simonetti
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

#ifndef MATRIX_H_
#define MATRIX_H_

#ifndef GLfloat
typedef float GLfloat;
#endif

#ifndef matrix4f
typedef GLfloat* matrix4f;
#endif

#ifndef BOOL
#define BOOL int
#endif

#ifndef TRUE
#define TRUE 1
#endif

#ifndef FALSE
#define FALSE 0
#endif

matrix4f matrix_ortho(GLfloat left, GLfloat right, GLfloat bottom, GLfloat top, GLfloat near, GLfloat far);
matrix4f matrix_frustum(GLfloat left, GLfloat right, GLfloat bottom, GLfloat top, GLfloat near, GLfloat far);

BOOL matrix_ortho_set(GLfloat left, GLfloat right, GLfloat bottom, GLfloat top, GLfloat near, GLfloat far, matrix4f matrix);
BOOL matrix_frustum_set(GLfloat left, GLfloat right, GLfloat bottom, GLfloat top, GLfloat near, GLfloat far, matrix4f matrix);

matrix4f matrix_identiy();
void matrix_identiy_set(matrix4f matrix);
matrix4f matrix_create(	GLfloat m11, GLfloat m21, GLfloat m31, GLfloat m41,
						GLfloat m12, GLfloat m22, GLfloat m32, GLfloat m42,
						GLfloat m13, GLfloat m23, GLfloat m33, GLfloat m43,
						GLfloat m14, GLfloat m24, GLfloat m34, GLfloat m44);
void matrix_free(const matrix4f matrix);
void matrix_printout(const matrix4f matrix);

void matrix_get_translation(const matrix4f matrix, GLfloat* x, GLfloat* y, GLfloat* z);

//count is in number of elements, not components ({x,y,z},{x,y,z} would be 2, not 6)
BOOL matrix_transform_vector(const matrix4f matrix, int dim, int count, const GLfloat* source, GLfloat* dest);
BOOL matrix_transform_normal(const matrix4f matrix, int dim, int count, const GLfloat* source, GLfloat* dest);

matrix4f matrix_scale(GLfloat x, GLfloat y, GLfloat z);
matrix4f matrix_translate(GLfloat x, GLfloat y, GLfloat z);
matrix4f matrix_rotate(GLfloat angle, GLfloat x, GLfloat y, GLfloat z);

void matrix_scale_set(GLfloat x, GLfloat y, GLfloat z, matrix4f matrix);
void matrix_translate_set(GLfloat x, GLfloat y, GLfloat z, matrix4f matrix);
void matrix_rotate_set(GLfloat angle, GLfloat x, GLfloat y, GLfloat z, matrix4f matrix);

GLfloat matrix_determinant(const matrix4f matrix);
matrix4f matrix_transpose(const matrix4f matrix);

//BOOL matrix_invert(const matrix4f matrix, matrix4f invert);
void matrix_transpose_set(const matrix4f matrix, matrix4f transpose);

matrix4f matrix_multiply_delete(const matrix4f m1, BOOL freeM1, const matrix4f m2, BOOL freeM2);
matrix4f matrix_multiply(const matrix4f m1, const matrix4f m2);
matrix4f matrix_multiply_disposable(const matrix4f mat, const matrix4f disposable);

#endif
