/*
Source file for 3D graphics core

Copyright (C) 2015 Kyle Gagner
All rights reserved
*/

#include <stdio.h>
#include <string.h>
#include <math.h>
#include "gcore.h"

int GCORE_ClipTriangles(double *buffin, int attributes, int statics, int count, double *buffout, int capacity)
{
	// pointer to current triangle in input buffer
	double *ctriangle = buffin;
	// pointer to location in output buffer for next triangle
	double *wtriangle = buffout;
	// number of triangles written out
	int written = 0;
	// offset to statics in triangle
	int statoff = 12+3*attributes;
	// number of doubles in a triangle
	int pitch = statoff+statics;
	// loop across all input triangles
	for(int n = 0; n < count; n++)
	{
		// region for each vertex (describes on which side of each clipping plane it lies)
		unsigned char regions[3];
		// mark the regions of each vertex
		for(int i = 0; i < 3; i++)
		{
			double *cvertex = ctriangle + 4*i;
			regions[i] = 0;
			if(cvertex[0] < -cvertex[3]) regions[i] |= 0x01; // mark regions outside w+x=0 plane
			else if(cvertex[0] > cvertex[3]) regions[i] |= 0x02; // mark regions outside w-x=0 plane
			if(cvertex[1] < -cvertex[3]) regions[i] |= 0x04; // mark regions outside w+y=0 plane
			else if(cvertex[1] > cvertex[3]) regions[i] |= 0x08; // mark regions outside w-y=0 plane
			if(cvertex[2] < -cvertex[3]) regions[i] |= 0x10; // mark regions outside w+z=0 plane
			else if(cvertex[2] > cvertex[3]) regions[i] |= 0x20; // mark regions outside w-z=0 plane
		}
		// trivial accept, entire triangle is within the bounding cube
		if((regions[0] | regions[1] | regions[2]) == 0)
		{
			memcpy(wtriangle, ctriangle, pitch*sizeof(double));
			written++;
			wtriangle+=pitch;
		}
		// trivial reject, entire triangle is on the wrong side of at least one of the clipping planes
		else if((~((regions[0]^regions[1]) | (regions[1]^regions[2]) | (regions[2]^regions[0]))) & regions[0]);
		// non-trivial, perform intersections with clipping planes
		else
		{
			// temporary buffer for barycentric coordinates
			double barycentric[6][3];
			// temporary buffer for conventional coordinates
			double coordinates[6][4];
			// index into the buffers
			int index = 0;
			// loop through the edges of the triangle
			for(int i = 0; i < 3; i++)
			{
				// pointer to current vertex
				double *cvertex = ctriangle + 4*i;
				// pointer to next vertex 
				double *nvertex = ctriangle + 4*((i+1)%3);
				unsigned char cregion = regions[i];
				unsigned char nregion = regions[(i+1)%3];
				// accepting the start vertex is still trivial
				if(!cregion)
				{
					memcpy(coordinates[index], cvertex, 4*sizeof(double));
					for(int k = 0; k < 3; k++) barycentric[index][k] = k==i ? 1.0 : 0.0;
					index++;
				}
				// and rejecting an entire edge is also trivial
				else if((~(cregion^nregion)) & cregion) continue; // some feel continue is just fancy goto
				// t values (interpolation from current to next vertex)
				double tvalues[2];
				// index into t values
				int tindex = 0;
				// find planes cut by edge
				unsigned char planes = cregion^nregion;
				// loop across all clipping planes
				for(int j = 0; j < 6; j++)
				{
					// only consider those planes cut by the edge
					if((planes >> j) & 1)
					{
						// two dimensional barycentric coordinate across edge
						double b1, b2;
						// x, y, or z cutting plane
						int comp = j/2;
						if(j%2) // w-comp=0 planes
						{
							b1=(cvertex[3]-cvertex[comp])/(nvertex[comp]-cvertex[comp]-nvertex[3]+cvertex[3]);//(cvertex[comp]-cvertex[3])/(cvertex[comp]-nvertex[comp]+nvertex[3]-cvertex[3]);
						}
						else // w+comp=0 planes
						{
							b1=(-cvertex[3]-cvertex[comp])/(nvertex[comp]-cvertex[comp]+nvertex[3]-cvertex[3]);//(cvertex[comp]-cvertex[3])/(cvertex[comp]-nvertex[comp]+nvertex[3]-cvertex[3]);
						}
						b2 = 1-b1;
						// calculate the actual point
						double x = cvertex[0]*b2+nvertex[0]*b1;
						double y = cvertex[1]*b2+nvertex[1]*b1;
						double z = cvertex[2]*b2+nvertex[2]*b1;
						double w = cvertex[3]*b2+nvertex[3]*b1;
						// test to see if point is clipped (ORs are due to floating point not to be trusted)
						if(x < -w && j!=0) continue;
						else if(x > w && j!=1) continue;
						if(y < -w && j!=2) continue;
						else if(y > w && j!=3) continue;
						if(z < -w && j!=4) continue;
						else if(z > w && j!=5) continue;
						// the point is valid, not clipped, to be put in buffer
						coordinates[index][0] = x;
						coordinates[index][1] = y;
						coordinates[index][2] = z;
						coordinates[index][3] = w;
						barycentric[index][i] = b2;
						barycentric[index][(i+1)%3] = b1;
						barycentric[index][(i+2)%3] = 0;
						index++;
						tvalues[tindex] == b1;
						tindex++;
						// prevent degeneracy from causing tindex out of bounds issue
						if(tindex == 2) break;
					}
					// optimization
					else if(!(planes >> j)) break;
				}
				// it's possible the vertices could have been put in out of order if edge passes through two faces of cube
				if(tindex == 2 && tvalues[0] > tvalues[1])
				{
					double temp[4];
					memcpy(temp, coordinates[index-1], 4*sizeof(double));
					memcpy(coordinates[index-1], coordinates[index-2], 4*sizeof(double));
					memcpy(coordinates[index-2], temp, 4*sizeof(double));
					memcpy(temp, barycentric[index-1], 3*sizeof(double));
					memcpy(barycentric[index-1], barycentric[index-2], 4*sizeof(double));
					memcpy(barycentric[index-2], temp, 4*sizeof(double));
				}
			}
			// now the buffer has all the information needed to construct the final triangle mesh
			// easier to do this by splitting the loop up into a first case then subsequent cases
			memcpy(wtriangle, coordinates[0], 12*sizeof(double));
			wtriangle += 12;
			// pointer to attributes for first vertex (for reuse)
			double *attrib0 = wtriangle;
			for(int i = 0; i < 3; i++)
			{
				/*double winterp =
					barycentric[i][0] / ctriangle[3] +
					barycentric[i][1] / ctriangle[7] +
					barycentric[i][2] / ctriangle[11];*/
				for(int j = 0; j < attributes; j++)
				{
					/**wtriangle = (
						ctriangle[12+j] * barycentric[i][0] / ctriangle[3] +
						ctriangle[12+j+attributes] * barycentric[i][1] / ctriangle[7] +
						ctriangle[12+j+2*attributes]  * barycentric[i][2] / ctriangle[11]) / winterp;*/
					*wtriangle =
						ctriangle[12+j] * barycentric[i][0] +
						ctriangle[12+j+attributes] * barycentric[i][1] +
						ctriangle[12+j+2*attributes] * barycentric[i][2];
					wtriangle++;
				}
			}
			memcpy(wtriangle, ctriangle+statoff, statics*sizeof(double));
			wtriangle += statics;
			written++;
			if(written == capacity) return capacity;
			for(int i = 2; i < index-1; i++)
			{
				memcpy(wtriangle, coordinates[0], 4*sizeof(double));
				wtriangle += 4;
				memcpy(wtriangle, coordinates[i], 8*sizeof(double));
				wtriangle += 8;
				memcpy(wtriangle, attrib0, attributes*sizeof(double));
				wtriangle += attributes;
				memcpy(wtriangle, wtriangle-pitch+attributes, attributes*sizeof(double));
				wtriangle += attributes;
				/*double winterp =
					barycentric[i+1][0] / ctriangle[3] +
					barycentric[i+1][1] / ctriangle[7] +
					barycentric[i+1][2] / ctriangle[11];*/
				for(int j = 0; j < attributes; j++)
				{
					/**wtriangle = (
						ctriangle[12+j] * barycentric[i+1][0] / ctriangle[3] +
						ctriangle[12+j+attributes] * barycentric[i+1][1] / ctriangle[7] +
						ctriangle[12+j+2*attributes]  * barycentric[i+1][2] / ctriangle[11]) / winterp;*/
					*wtriangle =
						ctriangle[12+j] * barycentric[i+1][0] +
						ctriangle[12+j+attributes] * barycentric[i+1][1] +
						ctriangle[12+j+2*attributes]  * barycentric[i+1][2];
					wtriangle++;
				}
				memcpy(wtriangle, ctriangle+statoff, statics*sizeof(double));
				wtriangle += statics;
				written++;
				if(written == capacity) return capacity;
			}
		}
		ctriangle+=pitch;
	}
	return written;
}

int GCORE_TransformTriangles(double *buff, int attributes, int statics, int count, int anormal, int snormal, M3D_Matrix *world, M3D_Matrix *view, M3D_Matrix *projection)
{
	// pointer to current triangle
	double *ctriangle = buff;
	// number of doubles per triangle
	int pitch = 12+3*attributes+statics;
	// normal transform
	M3D_Matrix normtrans;
	// coordinate transform
	M3D_Matrix coordtrans;
	// temporary matrix
	M3D_Matrix temp;
	M3D_InverseTranspose(world, &normtrans);
	M3D_Multiply(view, world, &temp);
	M3D_Multiply(projection, &temp, &coordtrans);
	for(int n = 0; n < count; n++)
	{
		// temporary holding space for coordinates
		M3D_Vector temp1[4];
		memcpy(temp1, ctriangle, 12*sizeof(double));
		for(int i = 0; i < 3; i++)
		{
			M3D_Transform(&coordtrans, &temp1[i], (M3D_Vector*)(ctriangle+4*i));
		}
		// temporary holding space for normal
		M3D_Vector temp2;
		if(snormal != -1)
		{
			memcpy(temp2, ctriangle+snormal, 3*sizeof(double));
			temp2[3] = 0;
			M3D_Transform(&normtrans, &temp2, (M3D_Vector*)(ctriangle+snormal));
		}
		if(anormal != -1)
		{
			for(int i = 0; i < 3; i++)
			{
				int offset = anormal+attributes*i;
				memcpy(temp2, ctriangle+offset, 3*sizeof(double));
				temp2[3] = 0;
				M3D_Transform(&normtrans, &temp2, (M3D_Vector*)(ctriangle+offset));
			}
		}
		ctriangle += pitch;
	}
}

void GCORE_TriangleRaster(double *buff, int attributes, int statics, int count, double *zbuff, double *abuff, int width, int height)
{
	// pointer to current triangle
	double *ctriangle = buff;
	// number of doubles per triangle
	int pitch = 12+3*attributes+statics;
	for(int n = 0; n < count; n++)
	{
		double ytmp[] = {0.5*(height*ctriangle[1]/ctriangle[3]+height), 0.5*(height*ctriangle[5]/ctriangle[7]+height), 0.5*(height*ctriangle[9]/ctriangle[11]+height)};
		double *verts[3];
		double *attrs[3];
		double xvals[3];
		double yvals[3];
		int ycut[3];
		for(int i = 0; i < 3; i++)
		{
			int ind = 0;
			for(int j = 0; j < 3; j++)
			{
				if(i > j && ytmp[j] <= ytmp[i]) ind++;
				else if(i < j && ytmp[j] < ytmp[i]) ind++;
			}
			verts[ind] = ctriangle + 4*i;
			attrs[ind] = ctriangle + 12 + attributes*i;
			xvals[ind] = 0.5*(width*verts[ind][0]/verts[ind][3]+width);
			ycut[ind] = round(yvals[ind] = ytmp[i]);
		}
		for(int y = ycut[0]; y < ycut[2]; y++)
		{
			double t0 = (double)(y-ycut[0])/(ycut[2]-ycut[0]);
			int cmp = ycut[2] == ycut[1] ? 0 : ycut[1] == ycut[0] ? 1 : y > ycut[1];
			double t1 = cmp ? (double)(y-ycut[1])/(ycut[2]-ycut[1]) : (double)(y-ycut[0])/(ycut[1]-ycut[0]);
			double xstart = xvals[0]*(1-t0)+xvals[2]*t0;
			double xend = cmp ? xvals[1]*(1-t1)+xvals[2]*t1 : xvals[0]*(1-t1)+xvals[1]*t1;
			double *vert0, *vert1, *attr0, *attr1;
			if(cmp)
			{
				vert0 = verts[1];
				vert1 = verts[2];
				attr0 = attrs[1];
				attr1 = attrs[2];
			}
			else
			{
				vert0 = verts[0];
				vert1 = verts[1];
				attr0 = attrs[0];
				attr1 = attrs[1];
			}
			for(int x = round(xstart<xend?xstart:xend); x < round(xstart<xend?xend:xstart); x++)
			{
				//if(y < 0 || y >= height || x < 0 || x >= width) continue;
				double t2 = (x-xstart)/(xend-xstart);
				double winterp = ((1-t0)/verts[0][3]+t0/verts[2][3])*(1-t2)+((1-t1)/vert0[3]+t1/vert1[3])*t2;
				double z = (((1-t0)*verts[0][2]/verts[0][3]+t0*verts[2][2]/verts[2][3])*(1-t2)+((1-t1)*vert0[2]/vert0[3]+t1*vert1[2]/vert1[3])*t2);
				if(z < zbuff[x+width*y]) continue;
				zbuff[x+width*y] = z;
				for(int i = 0; i < attributes; i++)
				{
					abuff[(attributes+statics)*x+(attributes+statics)*width*y+i] = ((attrs[0][i]*(1-t0)/verts[0][3]+attrs[2][i]*t0/verts[2][3])*(1-t2)+(attr0[i]*(1-t1)/vert0[3]+attr1[i]*t1/vert1[3])*t2)/winterp;
				}
				memcpy(&abuff[(attributes+statics)*x+(attributes+statics)*width*y+attributes],ctriangle+12+3*attributes,statics*sizeof(double));
			}
		}
		ctriangle += pitch;
	}
}
