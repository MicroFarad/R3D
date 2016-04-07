/*
Header file for matrix and vector operations
Intended for use in 3D graphics with homogeneous coordinates
Assumes a right handed coordinate system and columnar vectors

Copyright (C) 2015 Kyle Gagner
All rights reserved
*/

#include "m3d.h"
#include <math.h>

void M3D_Transform(M3D_Matrix *left, M3D_Vector* right, M3D_Vector* result)
{
	(*result)[0] = (*left)[0][0] * (*right)[0] + (*left)[0][1] * (*right)[1] + (*left)[0][2] * (*right)[2] + (*left)[0][3] * (*right)[3];
	(*result)[1] = (*left)[1][0] * (*right)[0] + (*left)[1][1] * (*right)[1] + (*left)[1][2] * (*right)[2] + (*left)[1][3] * (*right)[3];
	(*result)[2] = (*left)[2][0] * (*right)[0] + (*left)[2][1] * (*right)[1] + (*left)[2][2] * (*right)[2] + (*left)[2][3] * (*right)[3];
	(*result)[3] = (*left)[3][0] * (*right)[0] + (*left)[3][1] * (*right)[1] + (*left)[3][2] * (*right)[2] + (*left)[3][3] * (*right)[3];
}

void M3D_Add(M3D_Vector *left, M3D_Vector *right, M3D_Vector *result)
{
	(*result)[0] = (*left)[0] + (*right)[0];
	(*result)[1] = (*left)[1] + (*right)[1];
	(*result)[2] = (*left)[2] + (*right)[2];
	(*result)[3] = 1;
}

void M3D_Subtract(M3D_Vector *left, M3D_Vector *right, M3D_Vector *result)
{
	(*result)[0] = (*left)[0] - (*right)[0];
	(*result)[1] = (*left)[1] - (*right)[1];
	(*result)[2] = (*left)[2] - (*right)[2];
	(*result)[3] = 1;
}

double M3D_Dot(M3D_Vector *left, M3D_Vector *right)
{
	return (*left)[0] * (*right)[0] + (*left)[1] * (*right)[1] + (*left)[2] * (*right)[2];
}

void M3D_Cross(M3D_Vector *left, M3D_Vector *right, M3D_Vector *result)
{
	(*result)[0] = (*left)[1] * (*right)[2] - (*left)[2] * (*right)[1];
	(*result)[1] = (*left)[2] * (*right)[0] - (*left)[0] * (*right)[2];
	(*result)[2] = (*left)[0] * (*right)[1] - (*left)[1] * (*right)[0];
	(*result)[3] = 1;
}

void M3D_ScaleVector(double scale, M3D_Vector *operand, M3D_Vector *result)
{
	(*result)[0] = scale * (*operand)[0];
	(*result)[1] = scale * (*operand)[1];
	(*result)[2] = scale * (*operand)[2];
	(*result)[3] = 1;
}

void M3D_Unit(M3D_Vector *operand, M3D_Vector *result)
{
	double scale = 1.0 / sqrt((*operand)[0] * (*operand)[0] + (*operand)[1] * (*operand)[1] + (*operand)[2] * (*operand)[2]);
	(*result)[0] = scale * (*operand)[0];
	(*result)[1] = scale * (*operand)[1];
	(*result)[2] = scale * (*operand)[2];
	(*result)[3] = 1;
}

void M3D_Normalized(M3D_Vector *operand, M3D_Vector *result)
{
	double scale = 1.0 / (*operand)[3];
	(*result)[0] = scale * (*operand)[0];
	(*result)[1] = scale * (*operand)[1];
	(*result)[2] = scale * (*operand)[2];
	(*result)[3] = 1;
}

void M3D_Identity(M3D_Matrix *result)
{
	(*result)[0][0] = 1;
	(*result)[0][1] = 0;
	(*result)[0][2] = 0;
	(*result)[0][3] = 0;
	(*result)[1][0] = 0;
	(*result)[1][1] = 1;
	(*result)[1][2] = 0;
	(*result)[1][3] = 0;
	(*result)[2][0] = 0;
	(*result)[2][1] = 0;
	(*result)[2][2] = 1;
	(*result)[2][3] = 0;
	(*result)[3][0] = 0;
	(*result)[3][1] = 0;
	(*result)[3][2] = 0;
	(*result)[3][3] = 1;
}

void M3D_RotateX(double angle, M3D_Matrix *result)
{
	(*result)[0][0] = 1;
	(*result)[0][1] = 0;
	(*result)[0][2] = 0;
	(*result)[0][3] = 0;
	(*result)[1][0] = 0;
	(*result)[1][1] = cos(angle);
	(*result)[1][2] = -sin(angle);
	(*result)[1][3] = 0;
	(*result)[2][0] = 0;
	(*result)[2][1] = sin(angle);
	(*result)[2][2] = cos(angle);
	(*result)[2][3] = 0;
	(*result)[3][0] = 0;
	(*result)[3][1] = 0;
	(*result)[3][2] = 0;
	(*result)[3][3] = 1;
}

void M3D_RotateY(double angle, M3D_Matrix *result)
{
	(*result)[0][0] = cos(angle);
	(*result)[0][1] = 0;
	(*result)[0][2] = sin(angle);
	(*result)[0][3] = 0;
	(*result)[1][0] = 0;
	(*result)[1][1] = 1;
	(*result)[1][2] = 0;
	(*result)[1][3] = 0;
	(*result)[2][0] = -sin(angle);
	(*result)[2][1] = 0;
	(*result)[2][2] = cos(angle);
	(*result)[2][3] = 0;
	(*result)[3][0] = 0;
	(*result)[3][1] = 0;
	(*result)[3][2] = 0;
	(*result)[3][3] = 1;
}

void M3D_RotateZ(double angle, M3D_Matrix *result)
{
	(*result)[0][0] = cos(angle);
	(*result)[0][1] = -sin(angle);
	(*result)[0][2] = 0;
	(*result)[0][3] = 0;
	(*result)[1][0] = sin(angle);
	(*result)[1][1] = cos(angle);
	(*result)[1][2] = 0;
	(*result)[1][3] = 0;
	(*result)[2][0] = 0;
	(*result)[2][1] = 0;
	(*result)[2][2] = 1;
	(*result)[2][3] = 0;
	(*result)[3][0] = 0;
	(*result)[3][1] = 0;
	(*result)[3][2] = 0;
	(*result)[3][3] = 1;
}

void M3D_Translate(M3D_Vector *offset, M3D_Matrix *result)
{
	(*result)[0][0] = 1;
	(*result)[0][1] = 0;
	(*result)[0][2] = 0;
	(*result)[0][3] = (*offset)[0];
	(*result)[1][0] = 0;
	(*result)[1][1] = 1;
	(*result)[1][2] = 0;
	(*result)[1][3] = (*offset)[1];
	(*result)[2][0] = 0;
	(*result)[2][1] = 0;
	(*result)[2][2] = 1;
	(*result)[2][3] = (*offset)[2];
	(*result)[3][0] = 0;
	(*result)[3][1] = 0;
	(*result)[3][2] = 0;
	(*result)[3][3] = 1;
}

void M3D_Scale(M3D_Vector *scale, M3D_Matrix *result)
{
	(*result)[0][0] = (*scale)[0];
	(*result)[0][1] = 0;
	(*result)[0][2] = 0;
	(*result)[0][3] = 0;
	(*result)[1][0] = 0;
	(*result)[1][1] = (*scale)[1];
	(*result)[1][2] = 0;
	(*result)[1][3] = 0;
	(*result)[2][0] = 0;
	(*result)[2][1] = 0;
	(*result)[2][2] = (*scale)[2];
	(*result)[2][3] = 0;
	(*result)[3][0] = 0;
	(*result)[3][1] = 0;
	(*result)[3][2] = 0;
	(*result)[3][3] = 1;
}

void M3D_World(M3D_Vector *x_axis, M3D_Vector *y_axis, M3D_Vector *z_axis, M3D_Vector *origin, M3D_Matrix *result)
{
	(*result)[0][0] = (*x_axis)[0];
	(*result)[0][1] = (*y_axis)[0];
	(*result)[0][2] = (*z_axis)[0];
	(*result)[0][3] = (*origin)[0];
	(*result)[1][0] = (*x_axis)[1];
	(*result)[1][1] = (*y_axis)[1];
	(*result)[1][2] = (*z_axis)[1];
	(*result)[1][3] = (*origin)[1];
	(*result)[2][0] = (*x_axis)[2];
	(*result)[2][1] = (*y_axis)[2];
	(*result)[2][2] = (*z_axis)[2];
	(*result)[2][3] = (*origin)[2];
	(*result)[3][0] = 0;
	(*result)[3][1] = 0;
	(*result)[3][2] = 0;
	(*result)[3][3] = 1;
}

void M3D_View(M3D_Vector *x_axis, M3D_Vector *y_axis, M3D_Vector *z_axis, M3D_Vector *origin, M3D_Matrix *result)
{
	(*result)[0][0] = (*x_axis)[0];
	(*result)[0][1] = (*x_axis)[1];
	(*result)[0][2] = (*x_axis)[2];
	(*result)[0][3] = -M3D_Dot(origin, x_axis);
	(*result)[1][0] = (*y_axis)[0];
	(*result)[1][1] = (*y_axis)[1];
	(*result)[1][2] = (*y_axis)[2];
	(*result)[1][3] = -M3D_Dot(origin, y_axis);
	(*result)[2][0] = (*z_axis)[0];
	(*result)[2][1] = (*z_axis)[1];
	(*result)[2][2] = (*z_axis)[2];
	(*result)[2][3] = -M3D_Dot(origin, z_axis);
	(*result)[3][0] = 0;
	(*result)[3][1] = 0;
	(*result)[3][2] = 0;
	(*result)[3][3] = 1;
}

double M3D_Determinant(M3D_Matrix *operand)
{
	return
		(*operand)[0][0] * (*operand)[1][1] * (*operand)[2][2] * (*operand)[3][3] + (*operand)[0][0] * (*operand)[1][2] * (*operand)[2][3] * (*operand)[3][1] + (*operand)[0][0] * (*operand)[1][3] * (*operand)[2][1] * (*operand)[3][2] +
		(*operand)[0][1] * (*operand)[1][0] * (*operand)[2][3] * (*operand)[3][2] + (*operand)[0][1] * (*operand)[1][2] * (*operand)[2][0] * (*operand)[3][3] + (*operand)[0][1] * (*operand)[1][3] * (*operand)[2][2] * (*operand)[3][0] +
		(*operand)[0][2] * (*operand)[1][0] * (*operand)[2][1] * (*operand)[3][3] + (*operand)[0][2] * (*operand)[1][1] * (*operand)[2][3] * (*operand)[3][0] + (*operand)[0][2] * (*operand)[1][3] * (*operand)[2][0] * (*operand)[3][1] +
		(*operand)[0][3] * (*operand)[1][0] * (*operand)[2][2] * (*operand)[3][1] + (*operand)[0][3] * (*operand)[1][1] * (*operand)[2][0] * (*operand)[3][2] + (*operand)[0][3] * (*operand)[1][2] * (*operand)[2][1] * (*operand)[3][0] -
		(*operand)[0][0] * (*operand)[1][1] * (*operand)[2][3] * (*operand)[3][2] - (*operand)[0][0] * (*operand)[1][2] * (*operand)[2][1] * (*operand)[3][3] - (*operand)[0][0] * (*operand)[1][3] * (*operand)[2][2] * (*operand)[3][1] -
		(*operand)[0][1] * (*operand)[1][0] * (*operand)[2][2] * (*operand)[3][3] - (*operand)[0][1] * (*operand)[1][2] * (*operand)[2][3] * (*operand)[3][0] - (*operand)[0][1] * (*operand)[1][3] * (*operand)[2][0] * (*operand)[3][2] -
		(*operand)[0][2] * (*operand)[1][0] * (*operand)[2][3] * (*operand)[3][1] - (*operand)[0][2] * (*operand)[1][1] * (*operand)[2][0] * (*operand)[3][3] - (*operand)[0][2] * (*operand)[1][3] * (*operand)[2][1] * (*operand)[3][0] -
		(*operand)[0][3] * (*operand)[1][0] * (*operand)[2][1] * (*operand)[3][2] - (*operand)[0][3] * (*operand)[1][1] * (*operand)[2][2] * (*operand)[3][0] - (*operand)[0][3] * (*operand)[1][2] * (*operand)[2][0] * (*operand)[3][1];
}

void M3D_Inverse(M3D_Matrix *operand, M3D_Matrix *result)
{
	(*result)[0][0] =  (*operand)[1][1] * (*operand)[2][2] * (*operand)[3][3] - (*operand)[1][1] * (*operand)[2][3] * (*operand)[3][2] - (*operand)[2][1] * (*operand)[1][2] * (*operand)[3][3] + (*operand)[2][1] * (*operand)[1][3] * (*operand)[3][2] + (*operand)[3][1] * (*operand)[1][2] * (*operand)[2][3] - (*operand)[3][1] * (*operand)[1][3] * (*operand)[2][2];
	(*result)[0][1] = -(*operand)[0][1] * (*operand)[2][2] * (*operand)[3][3] + (*operand)[0][1] * (*operand)[2][3] * (*operand)[3][2] + (*operand)[2][1] * (*operand)[0][2] * (*operand)[3][3] - (*operand)[2][1] * (*operand)[0][3] * (*operand)[3][2] - (*operand)[3][1] * (*operand)[0][2] * (*operand)[2][3] + (*operand)[3][1] * (*operand)[0][3] * (*operand)[2][2];
	(*result)[0][2] =  (*operand)[0][1] * (*operand)[1][2] * (*operand)[3][3] - (*operand)[0][1] * (*operand)[1][3] * (*operand)[3][2] - (*operand)[1][1] * (*operand)[0][2] * (*operand)[3][3] + (*operand)[1][1] * (*operand)[0][3] * (*operand)[3][2] + (*operand)[3][1] * (*operand)[0][2] * (*operand)[1][3] - (*operand)[3][1] * (*operand)[0][3] * (*operand)[1][2];
	(*result)[0][3] = -(*operand)[0][1] * (*operand)[1][2] * (*operand)[2][3] + (*operand)[0][1] * (*operand)[1][3] * (*operand)[2][2] + (*operand)[1][1] * (*operand)[0][2] * (*operand)[2][3] - (*operand)[1][1] * (*operand)[0][3] * (*operand)[2][2] - (*operand)[2][1] * (*operand)[0][2] * (*operand)[1][3] + (*operand)[2][1] * (*operand)[0][3] * (*operand)[1][2];
	double scl = 1.0 / ((*operand)[0][0] * (*result)[0][0] + (*operand)[1][0] * (*result)[0][1] + (*operand)[2][0] * (*result)[0][2] + (*operand)[3][0] * (*result)[0][3]);
	(*result)[0][0] *= scl;
	(*result)[0][1] *= scl;
	(*result)[0][2] *= scl;
	(*result)[0][3] *= scl;
	(*result)[1][0] = scl * (-(*operand)[1][0] * (*operand)[2][2] * (*operand)[3][3] + (*operand)[1][0] * (*operand)[2][3] * (*operand)[3][2] + (*operand)[2][0] * (*operand)[1][2] * (*operand)[3][3] - (*operand)[2][0] * (*operand)[1][3] * (*operand)[3][2] - (*operand)[3][0] * (*operand)[1][2] * (*operand)[2][3] + (*operand)[3][0] * (*operand)[1][3] * (*operand)[2][2]);
	(*result)[1][1] = scl * ( (*operand)[0][0] * (*operand)[2][2] * (*operand)[3][3] - (*operand)[0][0] * (*operand)[2][3] * (*operand)[3][2] - (*operand)[2][0] * (*operand)[0][2] * (*operand)[3][3] + (*operand)[2][0] * (*operand)[0][3] * (*operand)[3][2] + (*operand)[3][0] * (*operand)[0][2] * (*operand)[2][3] - (*operand)[3][0] * (*operand)[0][3] * (*operand)[2][2]);
	(*result)[1][2] = scl * (-(*operand)[0][0] * (*operand)[1][2] * (*operand)[3][3] + (*operand)[0][0] * (*operand)[1][3] * (*operand)[3][2] + (*operand)[1][0] * (*operand)[0][2] * (*operand)[3][3] - (*operand)[1][0] * (*operand)[0][3] * (*operand)[3][2] - (*operand)[3][0] * (*operand)[0][2] * (*operand)[1][3] + (*operand)[3][0] * (*operand)[0][3] * (*operand)[1][2]);
	(*result)[1][3] = scl * ( (*operand)[0][0] * (*operand)[1][2] * (*operand)[2][3] - (*operand)[0][0] * (*operand)[1][3] * (*operand)[2][2] - (*operand)[1][0] * (*operand)[0][2] * (*operand)[2][3] + (*operand)[1][0] * (*operand)[0][3] * (*operand)[2][2] + (*operand)[2][0] * (*operand)[0][2] * (*operand)[1][3] - (*operand)[2][0] * (*operand)[0][3] * (*operand)[1][2]);
	(*result)[2][0] = scl * ( (*operand)[1][0] * (*operand)[2][1] * (*operand)[3][3] - (*operand)[1][0] * (*operand)[2][3] * (*operand)[3][1] - (*operand)[2][0] * (*operand)[1][1] * (*operand)[3][3] + (*operand)[2][0] * (*operand)[1][3] * (*operand)[3][1] + (*operand)[3][0] * (*operand)[1][1] * (*operand)[2][3] - (*operand)[3][0] * (*operand)[1][3] * (*operand)[2][1]);
	(*result)[2][1] = scl * (-(*operand)[0][0] * (*operand)[2][1] * (*operand)[3][3] + (*operand)[0][0] * (*operand)[2][3] * (*operand)[3][1] + (*operand)[2][0] * (*operand)[0][1] * (*operand)[3][3] - (*operand)[2][0] * (*operand)[0][3] * (*operand)[3][1] - (*operand)[3][0] * (*operand)[0][1] * (*operand)[2][3] + (*operand)[3][0] * (*operand)[0][3] * (*operand)[2][1]);
	(*result)[2][2] = scl * ( (*operand)[0][0] * (*operand)[1][1] * (*operand)[3][3] - (*operand)[0][0] * (*operand)[1][3] * (*operand)[3][1] - (*operand)[1][0] * (*operand)[0][1] * (*operand)[3][3] + (*operand)[1][0] * (*operand)[0][3] * (*operand)[3][1] + (*operand)[3][0] * (*operand)[0][1] * (*operand)[1][3] - (*operand)[3][0] * (*operand)[0][3] * (*operand)[1][1]);
	(*result)[2][3] = scl * (-(*operand)[0][0] * (*operand)[1][1] * (*operand)[2][3] + (*operand)[0][0] * (*operand)[1][3] * (*operand)[2][1] + (*operand)[1][0] * (*operand)[0][1] * (*operand)[2][3] - (*operand)[1][0] * (*operand)[0][3] * (*operand)[2][1] - (*operand)[2][0] * (*operand)[0][1] * (*operand)[1][3] + (*operand)[2][0] * (*operand)[0][3] * (*operand)[1][1]);
	(*result)[3][0] = scl * (-(*operand)[1][0] * (*operand)[2][1] * (*operand)[3][2] + (*operand)[1][0] * (*operand)[2][2] * (*operand)[3][1] + (*operand)[2][0] * (*operand)[1][1] * (*operand)[3][2] - (*operand)[2][0] * (*operand)[1][2] * (*operand)[3][1] - (*operand)[3][0] * (*operand)[1][1] * (*operand)[2][2] + (*operand)[3][0] * (*operand)[1][2] * (*operand)[2][1]);
	(*result)[3][1] = scl * ( (*operand)[0][0] * (*operand)[2][1] * (*operand)[3][2] - (*operand)[0][0] * (*operand)[2][2] * (*operand)[3][1] - (*operand)[2][0] * (*operand)[0][1] * (*operand)[3][2] + (*operand)[2][0] * (*operand)[0][2] * (*operand)[3][1] + (*operand)[3][0] * (*operand)[0][1] * (*operand)[2][2] - (*operand)[3][0] * (*operand)[0][2] * (*operand)[2][1]);
	(*result)[3][2] = scl * (-(*operand)[0][0] * (*operand)[1][1] * (*operand)[3][2] + (*operand)[0][0] * (*operand)[1][2] * (*operand)[3][1] + (*operand)[1][0] * (*operand)[0][1] * (*operand)[3][2] - (*operand)[1][0] * (*operand)[0][2] * (*operand)[3][1] - (*operand)[3][0] * (*operand)[0][1] * (*operand)[1][2] + (*operand)[3][0] * (*operand)[0][2] * (*operand)[1][1]);
	(*result)[3][3] = scl * ( (*operand)[0][0] * (*operand)[1][1] * (*operand)[2][2] - (*operand)[0][0] * (*operand)[1][2] * (*operand)[2][1] - (*operand)[1][0] * (*operand)[0][1] * (*operand)[2][2] + (*operand)[1][0] * (*operand)[0][2] * (*operand)[2][1] + (*operand)[2][0] * (*operand)[0][1] * (*operand)[1][2] - (*operand)[2][0] * (*operand)[0][2] * (*operand)[1][1]);
}

void M3D_Transpose(M3D_Matrix *operand, M3D_Matrix *result)
{
	(*result)[0][0] = (*operand)[0][0];
	(*result)[0][1] = (*operand)[1][0];
	(*result)[0][2] = (*operand)[2][0];
	(*result)[0][3] = (*operand)[3][0];
	(*result)[1][0] = (*operand)[0][1];
	(*result)[1][1] = (*operand)[1][1];
	(*result)[1][2] = (*operand)[2][1];
	(*result)[1][3] = (*operand)[3][1];
	(*result)[2][0] = (*operand)[0][2];
	(*result)[2][1] = (*operand)[1][2];
	(*result)[2][2] = (*operand)[2][2];
	(*result)[2][3] = (*operand)[3][2];
	(*result)[3][0] = (*operand)[0][3];
	(*result)[3][1] = (*operand)[1][3];
	(*result)[3][2] = (*operand)[2][3];
	(*result)[3][3] = (*operand)[3][3];
}

void M3D_InverseTranspose(M3D_Matrix *operand, M3D_Matrix *result)
{
	M3D_Matrix temporary;
	M3D_Inverse(operand, &temporary);
	M3D_Transpose(&temporary, result);
}

void M3D_Multiply(M3D_Matrix *left, M3D_Matrix *right, M3D_Matrix *result)
{
	(*result)[0][0] = (*left)[0][0] * (*right)[0][0] + (*left)[0][1] * (*right)[1][0] + (*left)[0][2] * (*right)[2][0] + (*left)[0][3] * (*right)[3][0];
	(*result)[0][1] = (*left)[0][0] * (*right)[0][1] + (*left)[0][1] * (*right)[1][1] + (*left)[0][2] * (*right)[2][1] + (*left)[0][3] * (*right)[3][1];
	(*result)[0][2] = (*left)[0][0] * (*right)[0][2] + (*left)[0][1] * (*right)[1][2] + (*left)[0][2] * (*right)[2][2] + (*left)[0][3] * (*right)[3][2];
	(*result)[0][3] = (*left)[0][0] * (*right)[0][3] + (*left)[0][1] * (*right)[1][3] + (*left)[0][2] * (*right)[2][3] + (*left)[0][3] * (*right)[3][3];
	(*result)[1][0] = (*left)[1][0] * (*right)[0][0] + (*left)[1][1] * (*right)[1][0] + (*left)[1][2] * (*right)[2][0] + (*left)[1][3] * (*right)[3][0];
	(*result)[1][1] = (*left)[1][0] * (*right)[0][1] + (*left)[1][1] * (*right)[1][1] + (*left)[1][2] * (*right)[2][1] + (*left)[1][3] * (*right)[3][1];
	(*result)[1][2] = (*left)[1][0] * (*right)[0][2] + (*left)[1][1] * (*right)[1][2] + (*left)[1][2] * (*right)[2][2] + (*left)[1][3] * (*right)[3][2];
	(*result)[1][3] = (*left)[1][0] * (*right)[0][3] + (*left)[1][1] * (*right)[1][3] + (*left)[1][2] * (*right)[2][3] + (*left)[1][3] * (*right)[3][3];
	(*result)[2][0] = (*left)[2][0] * (*right)[0][0] + (*left)[2][1] * (*right)[1][0] + (*left)[2][2] * (*right)[2][0] + (*left)[2][3] * (*right)[3][0];
	(*result)[2][1] = (*left)[2][0] * (*right)[0][1] + (*left)[2][1] * (*right)[1][1] + (*left)[2][2] * (*right)[2][1] + (*left)[2][3] * (*right)[3][1];
	(*result)[2][2] = (*left)[2][0] * (*right)[0][2] + (*left)[2][1] * (*right)[1][2] + (*left)[2][2] * (*right)[2][2] + (*left)[2][3] * (*right)[3][2];
	(*result)[2][3] = (*left)[2][0] * (*right)[0][3] + (*left)[2][1] * (*right)[1][3] + (*left)[2][2] * (*right)[2][3] + (*left)[2][3] * (*right)[3][3];
	(*result)[3][0] = (*left)[3][0] * (*right)[0][0] + (*left)[3][1] * (*right)[1][0] + (*left)[3][2] * (*right)[2][0] + (*left)[3][3] * (*right)[3][0];
	(*result)[3][1] = (*left)[3][0] * (*right)[0][1] + (*left)[3][1] * (*right)[1][1] + (*left)[3][2] * (*right)[2][1] + (*left)[3][3] * (*right)[3][1];
	(*result)[3][2] = (*left)[3][0] * (*right)[0][2] + (*left)[3][1] * (*right)[1][2] + (*left)[3][2] * (*right)[2][2] + (*left)[3][3] * (*right)[3][2];
	(*result)[3][3] = (*left)[3][0] * (*right)[0][3] + (*left)[3][1] * (*right)[1][3] + (*left)[3][2] * (*right)[2][3] + (*left)[3][3] * (*right)[3][3];
}

void M3D_Perspective(double top, double bottom, double left, double right, double near, double far, M3D_Matrix *result)
{
	(*result)[0][0] = 2.0*near/(left-right);
	(*result)[0][1] = 0;
	(*result)[0][2] = (right+left)/(right-left);
	(*result)[0][3] = 0;
	(*result)[1][0] = 0;
	(*result)[1][1] = 2.0*near/(bottom-top);
	(*result)[1][2] = (top+bottom)/(top-bottom);
	(*result)[1][3] = 0;
	(*result)[2][0] = 0;
	(*result)[2][1] = 0;
	(*result)[2][2] = (far+near)/(far-near);
	(*result)[2][3] = 2.0*far*near/(near-far);
	(*result)[3][0] = 0;
	(*result)[3][1] = 0;
	(*result)[3][2] = -1;
	(*result)[3][3] = 0;
}

void M3D_Orthographic(double top, double bottom, double left, double right, double near, double far, M3D_Matrix *result)
{
	(*result)[0][0] = 2.0/(right-left);
	(*result)[0][1] = 0;
	(*result)[0][2] = 0;
	(*result)[0][3] = (right+left)/(left-right);
	(*result)[1][0] = 0;
	(*result)[1][1] = 2.0/(top-bottom);
	(*result)[1][2] = 0;
	(*result)[1][3] = (top+bottom)/(bottom-top);
	(*result)[2][0] = 0;
	(*result)[2][1] = 0;
	(*result)[2][2] = 2.0/(near-far);
	(*result)[2][3] = (far+near)/(near-far);
	(*result)[3][0] = 0;
	(*result)[3][1] = 0;
	(*result)[3][2] = 0;
	(*result)[3][3] = 1;
}
