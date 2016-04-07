/*
Header file for 3D graphics core

Copyright (C) 2015 Kyle Gagner
All rights reserved
*/

#include "m3d.h"

// include guard
#ifndef GCORE_H
#define GCORE_H

// clips triangles to a cube centered on the origin having corners (-1,-1,-1) (1,1,1) and interpolates attributes of the triangles' vertices
// takes a pointer to the input vertex buffer, the number of attributes per vertex, the number of static attributes per triangle,
// the number of triangles in the input buffer, a pointer to the output buffer, and the capacity in number of triangles of the output buffer
// for attributes a,b,c... and statics u,v,w...
// triangles are expressed in the form x1,y1,z1,w1,x2,y2,z2,w2,x3,y3,z3,w3,a1,a2,a3,b1,b2,b3,c1,c2,c3... u,v,w...
// returns the number of triangles placed in the output buffer
int GCORE_ClipTriangles(double *buffin, int attributes, int statics, int count, double *buffout, int capacity);

// transforms triangles in place
// takes a pointer to the vertex buffer, the number of attributes per vertex, the number of static attributes per triangle,
// the number of triangles in the buffer,
// an index to the x component of the normal attribute (subsequent indices y and z) or -1 if no normal,
// an index to the x component of the normal static or -1,
// and matrices for the world, view, and projection transforms
int GCORE_TransformTriangles(double *buff, int attributes, int statics, int count, int anormal, int snormal, M3D_Matrix *world, M3D_Matrix *view, M3D_Matrix *projection);

// rasterizes triangles with a halfspace algorithm
// takes a pointer to the input vertex buffer, the number of attributes per vertex, the number of static attributes per triangle,
// the number of triangles in the input buffer, a pointer to the z buffer, a pointer to the attribute buffer, and the width and height of the raster
// triangles are expressed in the same form as given for clipping
// the attributes buffer has the form a,b,c... u,v,w
void GCORE_TriangleRaster(double *buff, int attributes, int statics, int count, double *zbuff, double* abuff, int width, int height);

#endif
