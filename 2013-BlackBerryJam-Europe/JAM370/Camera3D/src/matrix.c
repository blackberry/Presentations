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

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <math.h>

#include "matrix.h"

//Nothing in here is optimized, so it would be easier to understand.

matrix4f matrix_ortho(GLfloat left, GLfloat right, GLfloat bottom, GLfloat top, GLfloat near, GLfloat far)
{
	GLfloat* ret = calloc(4 * 4, sizeof(GLfloat));
	if(ret)
	{
		if(!matrix_ortho_set(left, right, bottom, top, near, far, ret))
		{
			free(ret);
			return NULL;
		}
	}
	return ret;
}

matrix4f matrix_frustum(GLfloat left, GLfloat right, GLfloat bottom, GLfloat top, GLfloat near, GLfloat far)
{
	GLfloat* ret = calloc(4 * 4, sizeof(GLfloat));
	if(ret)
	{
		if(!matrix_frustum_set(left, right, bottom, top, near, far, ret))
		{
			free(ret);
			return NULL;
		}
	}
	return ret;
}

BOOL matrix_ortho_set(GLfloat left, GLfloat right, GLfloat bottom, GLfloat top, GLfloat near, GLfloat far, matrix4f matrix)
{
	if(!matrix || left == right || bottom == top || near == far)
	{
		return FALSE;
	}
	GLfloat tx = -(right + left) / (right - left);
	GLfloat ty = -(top + bottom) / (top - bottom);
	GLfloat tz = -(far + near) / (far - near);

	memset(matrix, 0, sizeof(GLfloat) * 4 * 4);

	matrix[0] = 2.0f / (right - left);
	matrix[5] = 2.0f / (top - bottom);
	matrix[10] = -2.0f / (far - near);

	matrix[12] = tx;
	matrix[13] = ty;
	matrix[14] = tz;

	matrix[15] = 1.0f;

	return TRUE;
}

BOOL matrix_frustum_set(GLfloat left, GLfloat right, GLfloat bottom, GLfloat top, GLfloat near, GLfloat far, matrix4f matrix)
{
	if(!matrix || left == right || bottom == top || near == far || near < 0.0f || far < 0.0f)
	{
		return FALSE;
	}

	GLfloat A = (right + left) / (right - left);
	GLfloat B = (top + bottom) / (top - bottom);
	GLfloat C = -(far + near) / (far - near);
	GLfloat D = -(2.0f * far * near) / (far - near);

	memset(matrix, 0, sizeof(GLfloat) * 4 * 4);

	matrix[0] = (2.0f * near) / (right - left);
	matrix[5] = (2.0f * near) / (top - bottom);

	matrix[8] = A;
	matrix[9] = B;
	matrix[10] = C;
	matrix[11] = -1.0f;

	matrix[14] = D;

	return TRUE;
}

matrix4f matrix_identiy()
{
	GLfloat* ret = calloc(4 * 4, sizeof(GLfloat));
	matrix_identiy_set(ret);
	return ret;
}

void matrix_identiy_set(matrix4f matrix)
{
	if(matrix)
	{
		memset(matrix, 0, sizeof(GLfloat) * 4 * 4);

		matrix[0] = 1.0f;
		matrix[5] = 1.0f;
		matrix[10] = 1.0f;
		matrix[15] = 1.0f;
	}
}

matrix4f matrix_create(	GLfloat m11, GLfloat m21, GLfloat m31, GLfloat m41,
						GLfloat m12, GLfloat m22, GLfloat m32, GLfloat m42,
						GLfloat m13, GLfloat m23, GLfloat m33, GLfloat m43,
						GLfloat m14, GLfloat m24, GLfloat m34, GLfloat m44)
{
	GLfloat* ret = calloc(4 * 4, sizeof(GLfloat));
	if(ret)
	{
		ret[0] = m11;
		ret[1] = m12;
		ret[2] = m13;
		ret[3] = m14;

		ret[4] = m21;
		ret[5] = m22;
		ret[6] = m23;
		ret[7] = m24;

		ret[8] = m31;
		ret[9] = m32;
		ret[10] = m33;
		ret[11] = m34;

		ret[12] = m41;
		ret[13] = m42;
		ret[14] = m43;
		ret[15] = m44;
	}
	return ret;
}

void matrix_free(const matrix4f matrix)
{
	free(matrix);
}

void matrix_printout(const matrix4f matrix)
{
	if(matrix)
	{
		fprintf(stderr, "%f, %f, %f, %f\n", matrix[0], matrix[4], matrix[8], matrix[12]);
		fprintf(stderr, "%f, %f, %f, %f\n", matrix[1], matrix[5], matrix[9], matrix[13]);
		fprintf(stderr, "%f, %f, %f, %f\n", matrix[2], matrix[6], matrix[10], matrix[14]);
		fprintf(stderr, "%f, %f, %f, %f\n", matrix[3], matrix[7], matrix[11], matrix[15]);
		fprintf(stderr, "\n");
	}
}

void matrix_get_translation(const matrix4f matrix, GLfloat* x, GLfloat* y, GLfloat* z)
{
	if(matrix)
	{
		if(x)
		{
			*x = matrix[12];
		}
		if(y)
		{
			*y = matrix[13];
		}
		if(z)
		{
			*z = matrix[14];
		}
	}
}

void matrix_transform_internal(const GLfloat* matrix, int sideLen, int dim, int count, BOOL translate, const GLfloat* source, GLfloat* dest)
{
	int i;
	int c;
	int m;
	for(i = 0; i < count; i++)
	{
		for(c = 0; c < dim; c++)
		{
			int destPos = i * dim + c;
			dest[destPos] = 0.0f;
			for(m = 0; m < dim; m++)
			{
				dest[destPos] += source[i * dim + c] * matrix[m * dim + c];
			}
			if(translate && dim < sideLen)
			{
				dest[destPos] += matrix[(sideLen - 1) * sideLen + c];
			}
		}
	}
}

BOOL matrix_transform_vector(const matrix4f matrix, int dim, int count, const GLfloat* source, GLfloat* dest)
{
	if(!matrix || dim < 1 || dim > 4 || !source || !dest)
	{
		return FALSE;
	}
	matrix_transform_internal(matrix, 4, dim, count, TRUE, source, dest);
	return TRUE;
}

BOOL matrix_transform_normal(const matrix4f matrix, int dim, int count, const GLfloat* source, GLfloat* dest)
{
	if(!matrix || dim < 1 || dim > 4 || !source || !dest)
	{
		return FALSE;
	}
	matrix_transform_internal(matrix, 4, dim, count, FALSE, source, dest);
	return TRUE;
}

matrix4f matrix_scale(GLfloat x, GLfloat y, GLfloat z)
{
	GLfloat* ret = calloc(4 * 4, sizeof(GLfloat));
	matrix_scale_set(x, y, z, ret);
	return ret;
}

matrix4f matrix_translate(GLfloat x, GLfloat y, GLfloat z)
{
	GLfloat* ret = calloc(4 * 4, sizeof(GLfloat));
	matrix_translate_set(x, y, z, ret);
	return ret;
}

matrix4f matrix_rotate(GLfloat angle, GLfloat x, GLfloat y, GLfloat z)
{
	GLfloat* ret = calloc(4 * 4, sizeof(GLfloat));
	matrix_rotate_set(angle, x, y, z, ret);
	return ret;
}

void matrix_scale_set(GLfloat x, GLfloat y, GLfloat z, matrix4f matrix)
{
	if(matrix)
	{
		memset(matrix, 0, sizeof(GLfloat) * 4 * 4);

		matrix[0] = x;
		matrix[5] = y;
		matrix[10] = z;
		matrix[15] = 1.0f;
	}
}

void matrix_translate_set(GLfloat x, GLfloat y, GLfloat z, matrix4f matrix)
{
	if(matrix)
	{
		memset(matrix, 0, sizeof(GLfloat) * 4 * 4);

		matrix[0] = 1.0f;
		matrix[5] = 1.0f;
		matrix[10] = 1.0f;
		matrix[15] = 1.0f;

		matrix[12] = x;
		matrix[13] = y;
		matrix[14] = z;
	}
}

void matrix_rotate_set(GLfloat angle, GLfloat x, GLfloat y, GLfloat z, matrix4f matrix)
{
	if(matrix)
	{
		//Convert the angle to radians
		GLfloat rad = angle * (M_PI / 180.0f);

		//Normalize input
		GLfloat len = sqrtf((x * x) + (y * y) + (z * z));
		if(len != 0.0f)
		{
			len = 1.0f / len;
			x *= len;
			y *= len;
			z *= len;
		}

		//Calculate sin/cos
		GLfloat c = cosf(rad);
		GLfloat s = sinf(rad);

		GLfloat invC = 1.0f - c;

		//Setup the matrix
		matrix[0] = ((x * x) * invC) + c;
		matrix[1] = (x * y * invC) + (z * s);
		matrix[2] = (x * z * invC) - (y * s);
		matrix[3] = 0.0f;

		matrix[4] = (x * y * invC) - (z * s);
		matrix[5] = ((y * y) * invC) + c;
		matrix[6] = (y * z * invC) + (x * s);
		matrix[7] = 0.0f;

		matrix[8] = (x * z * invC) + (y * s);
		matrix[9] = (y * z * invC) - (x * s);
		matrix[10] = ((z * z) * invC) + c;
		matrix[11] = 0.0f;

		matrix[12] = 0.0f;
		matrix[13] = 0.0f;
		matrix[14] = 0.0f;
		matrix[15] = 1.0f;
	}
}

GLfloat matrix_determinant_internal(const GLfloat* matrix, int sideLen)
{
	GLfloat ret = 0.0f;
	int i, j;
	//Positive calculation
	for(i = 0; i < sideLen; i++)
	{
		GLfloat mul = 1.0f;
		for(j = 0; j < sideLen; j++)
		{
			mul *= matrix[((i + j) % sideLen) * sideLen + j];
		}
		ret += mul;
	}
	//Negative calculation
	for(i = sideLen - 1; i >= 0; i--)
	{
		GLfloat mul = 1.0f;
		for(j = 0; j < sideLen; j++)
		{
			//Make sure that we don't go outside the bounds of the array
			int off = (i - j) % sideLen;
			if(off < 0)
			{
				off += sideLen;
			}
			mul *= matrix[off * sideLen + j];
		}
		ret -= mul;
	}
	return ret;
}

GLfloat matrix_determinant(const matrix4f matrix)
{
	if(matrix)
	{
		return matrix_determinant_internal(matrix, 4);
	}
	return INFINITY;
}

matrix4f matrix_transpose(const matrix4f matrix)
{
	GLfloat* ret = calloc(4 * 4, sizeof(GLfloat));
	matrix_transpose_set(matrix, ret);
	return ret;
}

BOOL matrix_invert(const matrix4f matrix, matrix4f invert)
{
	if(matrix && invert && matrix != invert)
	{
		GLfloat det = matrix_determinant(matrix);
		if(det >= 1e-6)
		{
			det = 1.0f / det;
			//TODO
		}
	}
	return FALSE;
}

void matrix_transpose_internal(const GLfloat* matrix, GLfloat* transpose, int sideLen)
{
	int i, j;
	for(i = 0; i < sideLen; i++)
	{
		for(j = 0; j < sideLen; j++)
		{
			transpose[j * sideLen + i] = matrix[i * sideLen + j];
		}
	}
}

void matrix_transpose_set(const matrix4f matrix, matrix4f transpose)
{
	if(matrix && transpose && matrix != transpose)
	{
		matrix_transpose_internal(matrix, transpose, 4);
	}
}

void matrix_multiple_internal(const GLfloat* m1, const GLfloat* m2, GLfloat* product, int sideLen)
{
	int i, j, k;

	//Not efficient but simple
	for(i = 0; i < sideLen; i++)
	{
		for(j = 0; j < sideLen; j++)
		{
			for(k = 0; k < sideLen; k++)
			{
				product[i * sideLen + j] += m1[i * sideLen + k] * m2[k * sideLen + j];
			}
		}
	}
}

//m1 * m2
matrix4f matrix_multiply_delete(const matrix4f m1, BOOL freeM1, const matrix4f m2, BOOL freeM2)
{
	if(!m1 || !m2)
	{
		return NULL;
	}
	GLfloat* ret = calloc(4 * 4, sizeof(GLfloat));
	if(ret)
	{
		//Not efficient but simple
		matrix_multiple_internal(m2, m1, ret, 4);

		if(freeM1)
		{
			matrix_free(m1);
		}
		if(freeM2)
		{
			matrix_free(m2);
		}
	}
	return ret;
}

matrix4f matrix_multiply(const matrix4f m1, const matrix4f m2)
{
	return matrix_multiply_delete(m1, FALSE, m2, FALSE);
}

matrix4f matrix_multiply_disposable(const matrix4f mat, const matrix4f disposable)
{
	return matrix_multiply_delete(mat, FALSE, disposable, TRUE);
}
