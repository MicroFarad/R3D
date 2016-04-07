/*
Header file for matrix and vector operations
Intended for use in 3D graphics with homogeneous coordinates

Copyright (C) 2015 Kyle Gagner
All rights reserved
*/

// include guard
#ifndef M3D_H
#define M3D_H

// type for homogeneous vector
// except where specified, vectors are expected to be in normalized form
typedef double M3D_Vector[4];

// type for matrix
// row major matrices
typedef double M3D_Matrix[4][4];

// transform a vector (may not be normalized) by a matrix (result may not be normalized)
// takes pointers to the left operand matrix, right operand vector, and result vector (may not be same as right operand)
void M3D_Transform(M3D_Matrix *left, M3D_Vector *right, M3D_Vector *result);

// adds two vectors
// takes pointers to the left and right operand vectors and a result vector
void M3D_Add(M3D_Vector *left, M3D_Vector *right, M3D_Vector *result);

// subtracts two vectors
// takes pointers to the left and right operand vectors and a result vector
void M3D_Subtract(M3D_Vector *left, M3D_Vector *right, M3D_Vector *result);

// takes the dot product of two vectors
// takes pointers to the left and right operand vectors
// returns the dot product
double M3D_Dot(M3D_Vector *left, M3D_Vector *right);

// takes the cross product of two vectors
// takes pointers to the left and right operand vectors and a result vector (may not be same as either operand)
void M3D_Cross(M3D_Vector *left, M3D_Vector *right, M3D_Vector *result);

// scales a vector
// takes the scale factor and pointers to the operand and result vectors
void M3D_ScaleVector(double scale, M3D_Vector *operand, M3D_Vector *result);

// given a vector, gets a unit vector
// takes pointers to the operand and result vectors
void M3D_Unit(M3D_Vector *operand, M3D_Vector *result);

// given a vector (not normalized), gets a normalized vector
// takes pointers to the operand and result vectors
void M3D_Normalized(M3D_Vector *operand, M3D_Vector *result);

// constructs an identity matrix
// takes a pointer to the result matrix
void M3D_Identity(M3D_Matrix *result);

// constructs a rotation matrix (right hand rule) about the given axis
// takes the angle and a pointer to the result matrix
void M3D_RotateX(double angle, M3D_Matrix *result);
void M3D_RotateY(double angle, M3D_Matrix *result);
void M3D_RotateZ(double angle, M3D_Matrix *result);

// constructs a translation matrix from an offset vector
// takes a pointer to the offset vector and a pointer to the result matrix
void M3D_Translate(M3D_Vector *offset, M3D_Matrix *result);

// constructs a scaling matrix
// takes a pointer to a vector describing the scaling along each axis and a pointer to the result matrix
void M3D_Scale(M3D_Vector *scale, M3D_Matrix *result);

// constructs a generalized world matrix
// takes pointers to vectors for transformations on each axis, translation vector, and result matrix
void M3D_World(M3D_Vector *x_axis, M3D_Vector *y_axis, M3D_Vector *z_axis, M3D_Vector *origin, M3D_Matrix *result);

// constructs a generalized view matrix, the inverse of a world matrix
// takes pointers to vectors for transformations on each axis, translation vector, and result matrix
void M3D_View(M3D_Vector *x_axis, M3D_Vector *y_axis, M3D_Vector *z_axis, M3D_Vector *origin, M3D_Matrix *result);

// finds the determinant of a matrix
// takes a pointer to the matrix
// returns the determinant
double M3D_Determinant(M3D_Matrix *operand);

// finds the inverse of a matrix
// takes pointers to the operand and result matrices (may not be the same)
void M3D_Inverse(M3D_Matrix *operand, M3D_Matrix *result);

// finds the transpose of a matrix
// takes pointers to the operand and result matrices (may not be the same)
void M3D_Transpose(M3D_Matrix *operand, M3D_Matrix *result);

// finds the inverse transpose of a matrix (used for transforming normals)
// takes pointers to the operand and result matrices (may not be the same)
void M3D_InverseTranspose(M3D_Matrix *operand, M3D_Matrix *result);

// finds the product of two matrices
// takes pointers to the left and right operands and result matrix (may not be same to either operand)
void M3D_Multiply(M3D_Matrix *left, M3D_Matrix *right, M3D_Matrix *result);

// constructs a perspective projection matrix
// takes bounds for the top, bottom, left, right, near, and far planes, and a pointer to the result matrix
void M3D_Perspective(double top, double bottom, double left, double right, double near, double far, M3D_Matrix *result);

// constructs an orthographic projection matrix
// takes bounds for the top, bottom, left, right, near, and far planes, and a pointer to the result matrix
void M3D_Orthographic(double top, double bottom, double left, double right, double near, double far, M3D_Matrix *result);

#endif
