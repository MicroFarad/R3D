/*
Implements novel marching simplices algorithm
Similar to the marching tetrahedra algorithm, an improvement
on marching cubes, and arguably an improvement on marching tetrahedra.
It is up to debate whether this is better than marching tetrahedra.

Copyright 2014-2015 Kyle Gagner
All rights reserved
*/

// Note to future reader... I am sorry - Kyle 12/27/15
#define tau 6.28318530717958647692528676655900576839433879875021
int global=0;

int KMSA_Isochunk(double *corners,int *indices,double *tvals)
{
	/*
	Imagine cube from 0,0,0 to 1,1,1 in right handed coordinate system
	Each vertex is given an index, this index is just its position converted to binary
	0,0,0->0; 1,1,1->7; 1,0,1->5; etc...
	We break cube into six simplices, irregular tetrahedra.
	All simplices are defined by the four indices of their vertices, that is t[]
	t[0] is always the starting vertex, 0
	t[3] is always the ending vertex, 7
	t[1] is one of the two remaining vertices, with a hamming distance closest to t[0], having an index of 1, 2, or 4
	t[2] is the other of the two remaining vertices, hamming distance closest to t[3], having an index of 6, 5, or 3
	All simplex vertices are defined by an ordered traversal from 0 to 7 along cube edges
	This means that for a given t[1], t[2] has only two possibilities, given by the vertices connected to t[1]
		this excludes one of the three indices t[2] may have, namely the one corresponding to a vertex that
		cannot be reached from t[1] along one cube edge
	For simplicity purposes, the axis lying along the line from 0,0,0 to 1,1,1 will hereafter be called only "the axis"
	The simplices are shaped somewhat like six orange slices cut out of the cube, with the axis of the orange being the axis
	What are you doing? Go grab a cube and some of those magnet toys right now. It only gets more confusing.
	A pencil and paper won't hurt either.
	Now imagine orthogonally projecting the cube onto a plane perpendicular to the axis.
	Moving from t[0] to t[1] to t[2] to t[3] and back to t[0] on some of the projected simplices yeilds a clockwise (positive)
		motion and an anticlockwise (negative) motion on others (half are positive, half are negative).
	The positive and negative orientations correspond to the two possible t[2] vertices given a t[1]
	Given these facts, a simplex can be fully defined by t[1] and orientation, positive or negative
	The algorithm loops through all simplices using a loop for t[1] and an inner loop for orientation
	For every simplex which contains part of the isosurface, we must generate the mesh at that simplex
	It is important that the triangles in the generated mesh have a uniform orientation
	An anticlockwise vertex order has been chosen, as seen looking on from the outside of the isosurface
	Since it is known whether t[2] is clockwise or anticlockwise of t[1] along the axis, and that t[0] and t[1] are on the axis
		the problem of ensuring order is greatly simplified.
	There are three cases for constructing the portion of the isosurface that lies in a given simplex
		which of the three cases depends on the number of the simplex's vertices that have positive density.
			For 0 or 4, it is trivial. The isosurface does not intersect the simplex
			For 1 or 3, only a single triangle is contained in the simplex
			For 2, a four sided figure comprised of two triangles is contained
	*/
	int n,i,c,k,r,j;
	int t[4];
	int a1[4];
	int a2[4];
	int u[6]={0,0,1,1,1,0};
	int v[6]={0,1,1,1,0,0};
	int *p;
	int *z;
	k=0;
	t[0]=0; // t[0] is always at 0,0,0
	t[3]=7; // t[1] is always at 1,1,1
	for(t[1]=1;t[1]<8;t[1]=t[1]<<1)
	{ // loop through all t[1]
		for(n=0;n<2;n+=1)
		{ // n=0 positive orientation, n=1 negative orientation
			t[2]=t[1]+(7&(n?(t[1]<<1)+(t[1]>>2):(t[1]<<2)+(t[1]>>1))); // infer t[2] from t[1] and orienation
			c=0;
			p=a1;
			z=a2;
			for(i=0;i<4;i++)
			{ // count the number of vertices and enumerate which have positive density and which do not
				if(corners[t[i]]>0)
				{
					p[c]=i; // add vertex indices with positive density to ordered array p
					c++;
				}
				else
				{
					z[i-c]=i; // add vertex indices with zero or negative density to ordered array z
				}
			}
			r=1;
			switch(c)
			{ // switch between the three cases
				case 3:
					// for three swap p and z and set the reverse flag
					// we can treat it the same as one after that
					// we don't break, just continue to case two's code
					// it's the same single triangle case, no need to duplicate codes
					p=a2;
					z=a1;
					r=0;
				case 1:
					r=(p[0]+n+r)%2;
					for(i=0;i<3;i++)
					{
						tvals[k>>1]=1.0-(fabs(corners[t[p[0]]])/(fabs(corners[t[p[0]]])+fabs(corners[t[z[r?2-i:i]]])));
						indices[k]=t[p[0]];
						k++;
						indices[k]=t[z[r?2-i:i]];
						k++;
					}
					break;
				case 2:
					r=(n+p[0]+p[1])%2;
					for(i=0;i<6;i++)
					{
						tvals[k>>1]=1.0-(corners[t[p[u[i]]]]/(corners[t[p[u[i]]]]-corners[t[z[r?1-v[i]:v[i]]]]));
						indices[k]=t[p[u[i]]];
						k++;
						indices[k]=t[z[r?1-v[i]:v[i]]];
						k++;
					}
					break;
				default:
					break;
			}
		}
	}
	return k;
}
double meta(double x,double y,double z)
{
	return pow(x*x+y*y+z*z,0.5);//1.0/(x*x+y*y+z*z);
}
double fun(double x,double y,double z)
{
	return 0.6-meta(x,y,z)+0.1*sin(12*y)+0.05*sin(24*y)+0.1*(sin(5*atan2(z,x))+0.5*sin(15*atan2(z,x))+0.25*sin(25*atan2(z,x)))*(0.6-fabs(y));
}
double func(double x,double y,double z)
{
	int n;
	double warp[3];
	double pt[3];
	double d[3];
	double t[2]={-1.0,1.0};
	double v;//=-meta(x,y,z);
	pt[0]=2*x;
	pt[1]=2*y;
	pt[2]=2*z;
	for(n=0;n<3;n++)
	{
		warp[n]=KNNN_Simplex(pt,3,0.7,2.0,d,1790+42*n);
	}
	/*for(n=0;n<4;n++)
	{
		pt[0]=x*pow(1.9,n)+0.34*n+warp[0];
		pt[1]=y*pow(1.9,n)+0.53*n+warp[1];
		pt[2]=z*pow(1.9,n)+0.62*n+warp[2];
		v+=KNNN_Simplex(pt,3,0.7,8.0/(n+1),d,1337);
		//v+=KNNN_Worley(pt,3,2.0,t,2,d,1337);
	}*/
	return fun(x+warp[0],y+warp[1],z+warp[2]);
}
void KMSA_STLtest(void)
{
	int n,k=0;
	int o;
	double x,y,z;
	double corners[8];
	int indices[200];
	double tvals[100];
	double d=0.02;
	FILE *fp=fopen("test.stl","w");
	fprintf(fp,"solid sphere\n");
	for(x=-1;x<1;x+=d)
	{
		printf("%lf\n",x);
		for(y=-1;y<1;y+=d)
		{
			/*if((fabs(x-0.4)<0.01)&&(fabs(y-0.4)<0.01))
			{
				printf("> ");
				scanf("%d",&global);
			}*/
			for(z=-1;z<1;z+=d)
			{
				k=0;
				for(n=0;n<8;n++)
				{
					corners[n]=fun(x+d*(n&1),y+d*((n>>1)&1),z+d*((n>>2)&1));
					//printf("%lf\n",x+d*(n&1));
				}
				//printf("\n");
				k=KMSA_Isochunk(corners,indices,tvals);
				//printf("k=%d\n",k);
				for(n=0;n<k;n+=2)
				{
					if(n%3==0)
					{
						fprintf(fp,"facet normal %lf %lf %lf\nouter loop\n",func(x+0.001,y,z)-func(x,y,z),func(x,y+0.001,z)-func(x,y,z),func(x,y,z+0.001)-func(x,y,z));
					}
					fprintf(fp,"vertex %lf %lf %lf\n",x+d*(indices[n]&1)*tvals[n>>1]+d*(indices[n+1]&1)*(1.0-tvals[n>>1]),y+d*((indices[n]>>1)&1)*tvals[n>>1]+d*((indices[n+1]>>1)&1)*(1.0-tvals[n>>1]),z+d*((indices[n]>>2)&1)*tvals[n>>1]+d*((indices[n+1]>>2)&1)*(1.0-tvals[n>>1]));
					if(n%3==1)
					{
						fprintf(fp,"endloop\nendfacet\n");
					}
				}
			}
		}
	}
	fprintf(fp,"endsolid sphere\n");
	fclose(fp);
}
