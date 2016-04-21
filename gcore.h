
/*
Header file for 3D graphics core

Copyright (C) 2015-2016 Kyle Gagner
All rights reserved
*/

// include guard
#ifndef GCORE_H
#define GCORE_H

#define GCORE_THREADED 1
#define GCORE_BLOCKING 3

#include <threads.h>
#include "m3d.h"
#include "avl.h"
#include "list.h"

struct GCORE_BufferPool;

typedef struct GCORE_Buffer
{
	int refcount;
	mtx_t lock;
	struct GCORE_BufferPool *source;
	void *content;
	struct GCORE_Buffer *next;
} GCORE_Buffer;

typedef struct GCORE_BufferPool
{
	int buffersize;
	mtx_t lock;
	cnd_t block;
	GCORE_Buffer *first;
	int flags;
} GCORE_BufferPool;

struct GCORE_ContainerPool;

typedef struct
{
	AVL_Tree map;
} GCORE_ContainerDescriptor;

typedef struct GCORE_Container
{
	int refcount;
	mtx_t lock;
	struct GCORE_ContainerPool *source;
	GCORE_Buffer *buffers;
	struct GCORE_Container *next;
} GCORE_Container;

typedef struct GCORE_ContainerPool
{
	GCORE_ContainerDescriptor *descriptor;
	mtx_t lock;
	cnd_t block;
	GCORE_Container *first;
	int flags;
} GCORE_ContainerPool;

typedef struct
{
	LIST_List queue;
	mtx_t lock;
	cnd_t block;
	int flags;
} GCORE_ContainerQueue;

typedef struct
{
	AVL_Tree set;
} GCORE_TagSubset;

typedef struct
{
	int *indices;
	int count;
} GCORE_IndexSubset;

typedef struct
{
} GCORE_TriangleClipper;

void GCORE_BufferRelease(GCORE_Buffer *buffer);

void GCORE_BufferReference(GCORE_Buffer *buffer, int increment);

void GCORE_BufferPoolInitialize(GCORE_BufferPool *pool, int buffersize, int initial, int flags);

GCORE_Buffer *GCORE_BufferPoolAcquire(GCORE_BufferPool *pool);

void GCORE_BufferPoolClean(GCORE_BufferPool *pool);

void GCORE_ContainerDescriptorInitialize(GCORE_ContainerDescriptor *descriptor);

void GCORE_ContainerDescriptorInsert(GCORE_ContainerDescriptor *descriptor, char *tag);

void GCORE_ContainerRelease(GCORE_Container *container);

void GCORE_ContainerReference(GCORE_Container *container, int increment, int flags);

void GCORE_ContainerPoolInitialize(GCORE_ContainerPool *pool, GCORE_Descriptor *descriptor, int initial);

GCORE_Container *GCORE_ContainerPoolAcquire(GCORE_ContainerPool *pool);

void GCORE_ContainerPoolClean(GCORE_ContainerPool *pool);

void GCORE_ContainerQueueEnqueue(GCORE_ContainerQueue *queue, GCORE_Container *container);

GCORE_Container *GCORE_ContainerQueueDequeue(GCORE_ContainerQueue *queue);

void GCORE_TagSubsetInitialize(GCORE_TagSubset *subset);

void GCORE_TagSubsetInsert(GCORE_TagSubset *subset, char *tag);

void GCORE_IndexSubsetConstruct(GCORE_IndexSubset *indices, GCORE_ContainerDescriptor *descriptor, GCORE_TagSubset *tags);

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
