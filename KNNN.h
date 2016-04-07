/*
This is a noise generation library, for different kinds of proceedural noise. All algorithms
have been generalized to arbitrary dimensions where possible.

Regarding Simplex noise (KNNN_Simplex):
Finds both the noise function and its derivative, based on Ken Perlin's simplex noise algorithm
http://www.csee.umbc.edu/~olano/s2002c36/ch02.pdf
Read more:
http://webstaff.itn.liu.se/~stegu/simplexnoise/simplexnoise.pdf
http://webstaff.itn.liu.se/~stegu/aqsis/DSOs/DSOnoises.html
The simplex noise portion of this code is a derivative work, based heavily on Ken Perlin's Java
code in Appendix B of the above pdf link (ch02.pdf).

Regarding Worley noise (KNNN_Worley):
Finds both the noise function and its derivative, based on, although not entirely like, Steven
Worley's Worley noise algorithm. This implementation breaks the sample space into regularly tiled
hypercubes and places only one seed point in each hypercube for performance reasons.
Read more:
http://www.gamedev.net/topic/576541-voronoi--worley-noise-knowlege-dump/
http://www.math.rutgers.edu/~erowland/polytopes.html
https://code.google.com/p/fractalterraingeneration/wiki/Cell_Noise
http://www.ylilammi.com/BerconNoise.shtml

This work by Kyle A. Gagner is licensed under a Creative Commons Attribution-ShareAlike 3.0
Unported License. http://creativecommons.org/licenses/by-sa/3.0/deed.en_US

Please note that this does not imply the entirety of this document is original.
The code was authored by Kyle Gagner, but the underlying algorithms are derivative works, based
on existing algorithms, which should be credited to their original authors.
Simplex noise algorithm credited to Ken Perlin
Worley noise algorithm credited to Steven Worley
*/

/*
Usage notes:

Simplex noise is often used to make fractal noise, in which several octaves of noise are added
together using a power law to reduce the magnitude of each increasing octave.

Curl noise can be made by taking the curl of a potential function. The potential function is
simply a scalar field for two dimensions, and a three dimensional scalar field for three
dimensions. Curl noise produces a velocity field in the desired number of dimensions, which has
implications for particle simulations. Read more...
http://www.cs.ubc.ca/~rbridson/docs/bridson-siggraph2007-curlnoise.pdf
Curl formulas are:
v(x,y)=<dp/dy,-dp/dx>
v(x,y,z)=<dp3/dy-dp2/dz,dp1/dz-dp3/dx,dp2/dx-dp1/dy>
Turbulent curl noise can be made by summing curl noise made from octaves of simplex noise, reducing
the magnitude of each octave as one would for fractal noise.
*/

double KNNN_Worley(double *point,int dim,double Minkowski,double *terms,int nterms,double *deriv,unsigned long long seed)
{
	int n,i,k;
	int A[dim];
	double d;
	double F[nterms];
	double T[nterms][dim];
	double t;
	unsigned long long hash;
	for(n=0;n<nterms;n++)
	{
		F[n]=-1.0;
	}
	for(n=0;n<(int)pow(3,dim);n++)
	{
		k=n;
		for(i=0;i<dim;i++)
		{
			A[i]=(int)floor(point[i])+(k%3)-1;
			k/=3;
		}
		hash=KFNV_Hash(A,dim*sizeof(int),seed);
		d=0;
		for(i=0;i<dim;i++)
		{
			hash=KFNV_Hash(&hash,sizeof(unsigned long long),hash);
			deriv[i]=(double)A[i]+((double)(hash&65535)/65535.0)-point[i];
			d+=pow(fabs(deriv[i]),Minkowski);
		}
		for(i=0;i<dim;i++)
		{
			deriv[i]=-pow(d,(1.0/Minkowski)-1.0)*pow(fabs(deriv[i]),Minkowski)/deriv[i];
		}
		d=pow(d,1.0/Minkowski);
		for(i=0;i<nterms;i++)
		{
			if((d<F[i])||(F[i]==-1.0))
			{
				t=F[i];
				F[i]=d;
				d=t;
				for(k=0;k<dim;k++)
				{
					t=T[i][k];
					T[i][k]=deriv[k];
					deriv[k]=t;
				}
			}
		}
	}
	d=0;
	for(n=0;n<dim;n++)
	{
		deriv[n]=0;
	}
	for(n=0;n<nterms;n++)
	{
		d+=F[n]*terms[n];
		for(i=0;i<dim;i++)
		{
			deriv[i]+=T[n][i]*terms[n];
		}
	}
	t=pow(dim,-1.0/Minkowski);
	for(n=0;n<dim;n++)
	{
		deriv[n]*=t;
	}
	return d*t;
}
double KNNN_Simplex(double *point,int dim,double radius,double amplitude,double *derivative,unsigned long long seed)
{
	int n,i;
	double s;
	int A[dim];
	double B[dim];
	double C[dim];
	int D[dim];
	int off[dim];
	int sort[dim+1];
	int ok;
	unsigned long long hash;
	double t1,t2,t4;
	double value;
	double sum;
	double tmp;
	int tmpi;
	// Skew
	s=0;
	for(n=0;n<dim;n++)
	{
		s+=point[n];
	}
	s*=(sqrt(dim+1)-1.0)/(double)dim;
	for(n=0;n<dim;n++)
	{
		A[n]=(int)floor(point[n]+s);
	}
	// Unskew
	s=0;
	for(n=0;n<dim;n++)
	{
		s+=A[n];
	}
	s*=((double)(dim+1)-sqrt(dim+1))/(double)(dim*(dim+1));
	for(n=0;n<dim;n++)
	{
		B[n]=point[n]-A[n]+s;
		off[n]=0;
		sort[n]=n;
		derivative[n]=0;
	}
	// Sort axis
	sort[dim]=0;
	do
	{
		ok=0;
		for(n=0;n<(dim-1);n++)
		{
			if(B[sort[n]]<B[sort[n+1]])
			{
				tmpi=sort[n];
				sort[n]=sort[n+1];
				sort[n+1]=tmpi;
				ok=1;
			}
		}
	} while(ok);
	// Sum contributions from each corner
	value=0;
	for(n=0;n<=dim;n++)
	{
		s=0;
		for(i=0;i<dim;i++)
		{
			s+=off[i];
		}
		s*=((double)(dim+1)-sqrt(dim+1))/(double)(dim*(dim+1));
		t1=radius;
		for(i=0;i<dim;i++)
		{
			C[i]=B[i]-off[i]+s;
			t1-=C[i]*C[i];
		}
		for(i=0;i<dim;i++)
		{
			D[i]=(A[i]+off[i])^i;
		}
		off[sort[n]]++;
		if(t1<0)
		{
			continue;
		}
		hash=KFNV_Hash(D,dim*sizeof(int),seed);
		hash=KFNV_Hash(&hash,sizeof(unsigned long long),hash);
		sum=0;
		tmp=0;
		t2=t1*t1;
		t4=t2*t2;
		for(i=0;i<dim;i++)
		{
			if(((hash>>dim)%dim)!=i)
			{
				if((hash>>i)&1)
				{
					sum+=C[i];
					tmp++;
				}
				else
				{
					sum-=C[i];
					tmp--;
				}
			}
		}
		for(i=0;i<dim;i++)
		{
			derivative[i]+=(-8*amplitude*t1*t2*C[i]*sum)+(t4*tmp);
		}
		value+=t4*sum;
	}
	return amplitude*value;
}
