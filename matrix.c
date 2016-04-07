/*
Header file for matrix operations
Not intended for performance critical applications

Copyright (C) 2015 Kyle Gagner
All rights reserved
*/

#include <string.h>
#include "matrix.h"

void MATRIX_Multiply(int n, int m, int p, double *left, double *right, double *result)
{
	for(int i = 0; i < n; i++)
	{
		for(int j = 0; j < p; j++)
		{
			double sum = 0;
			for(int k = 0; k < m; k++) sum += left[m*i+k] * right[p*k+j];
			result[p*i+j] = sum;
		}
	}
}

void MATRIX_Inverse(int n, double *operand, double *result)
{
	int i, j, k;
	double tmp;
	double work[n*n];
	memcpy(work, operand, n*n*sizeof(double));
	for(i = 0; i < n; i++)
	{
		for(int j = 0; j < n; j++)
		{
			result[n*i+j] = i==j ? 1.0 : 0.0;
		}
	}
	for(i = 0; i < n; i++)
	{
		for(k = i; work[n*k+i]==0 && k<n; k++);
		if(k!=i)
		{
			for(int j = i; j < n; j++)
			{
				tmp = work[n*i+j];
				work[n*i+j] = work[n*k+j];
				work[n*k+j] = tmp;
				tmp = result[n*i+j];
				result[n*i+j] = result[n*k+j];
				result[n*k+j] = tmp;
			}
		}
		tmp = 1.0 / work[n*i+i];
		work[n*i+i] = 1.0;
		for(j = i + 1; j < n; j++)
		{
			work[n*i+j] *= tmp;
		}
		for(j = 0; j < n; j++)
		{
			result[n*i+j] *= tmp;
		}
		for(k = i + 1; k < n; k++)
		{
			tmp = work[n*k+i];
			work[n*k+i] = 0.0;
			for(j = i + 1; j < n; j++)
			{
				work[n*k+j] -= tmp * work[n*i+j];
			}
			for(j = 0; j < n; j++)
			{
				result[n*k+j] -= tmp * result[n*i+j];
			}
		}
	}
	for(i = n - 1; i >= 0; i--)
	{
		for(k = 0; k < i; k++)
		{
			tmp = work[n*k+i];
			work[n*k+i] = 0.0;
			for(j = 0; j < n; j++)
			{
				result[n*k+j] -= tmp * result[n*i+j];
			}
		}
	}
}
