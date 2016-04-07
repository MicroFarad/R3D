/*
Header file for matrix operations
Not intended for performance critical applications

Copyright (C) 2015 Kyle Gagner
All rights reserved
*/


// include guard
#ifndef MATRIX_H
#define MATRIX

/*
A matrix is simply a pointer to a flat array of double values
A matrix is interpreted as being in row major order
*/

// multiplies an n by m matrix by an m by p matrix to get an n by p result
// takes n, m, p, the left and right operand matrices, and the result matrix
void MATRIX_Multiply(int n, int m, int p, double *left, double *right, double *result);

// inverts and n by n matrix
// takes n, the operand matrix, and the result matrix
void MATRIX_Inverse(int n, double *operand, double *result);

#endif
