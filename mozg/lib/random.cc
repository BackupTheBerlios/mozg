// $Id: random.cc,v 1.1 2006/03/18 21:53:18 lightdruid Exp $
// 
// random.cc
// some functions for generating random distributions


#include "mozg/random.hh"


namespace mozg {

// Random number generator of Park and Miller
#define IA 16807
#define IM 2147483647
#define AM (1.0/IM)
#define IQ 127773
#define IR 2836
#define NTAB 32
#define NDIV (1+(IM-1)/NTAB)
#define EPS 1.2e-7
#define RNMX (1.0-EPS)

  mozgflt ran1(long* idum)
/* Call with negative integer to initialize ;thereafter do not alter \
idum between successive deviates in a sequence. */
    {
      int j;
      long k;
      static long iy=0;
      static long iv[NTAB];
      mozgflt temp;
      
      if ( *idum <= 0 || !iy ) {
	if ( -(*idum) < 1 ) *idum=1;
	else *idum = -(*idum);
	for ( j=NTAB+7 ; j >= 0 ; j-- ) {
	  k=(*idum)/IQ;
	  *idum=IA*(*idum-k*IQ)-IR*k;
	  if ( *idum < 0 ) *idum += IM;
	  if ( j < NTAB ) iv[j] = *idum;
	}
	iy=iv[0];
      }
      k=(*idum)/IQ;
      *idum=IA*(*idum-k*IQ)-IR*k;
      if ( *idum < 0 ) *idum += IM;
      j=iy/NDIV;
      iy=iv[j];
      iv[j] = *idum;
      if ( (temp=AM*iy) > RNMX ) return RNMX;
      else return temp;
    }
  
// It returns random regulary distributed in [argmin,argmax] number
  mozgflt random_number(const mozgflt argmin,
			const mozgflt argmax)
    {
      mozgflt r;
      static long idum = -1;
      
      r = ran1(&idum);
      return r*fabs(argmax-argmin) + argmin;
    }


// It returns a normally distributed deviate with zero mean and unit variance
  mozgflt gasdev()
    {
      static short iset=0;
      static mozgflt gset;
      mozgflt fac,rsq,v1,v2;
      
      if ( iset==0 )
	{
	  do
	    {
	      v1=2.0*random_number(0.0,1.0)-1.0;
	      v2=2.0*random_number(0.0,1.0)-1.0;
	      rsq=v1*v1+v2*v2;
	    }
	  while ( rsq >=1.0 || rsq == 0.0 );
	  fac=sqrt(-2.0*log(rsq)/rsq);
	  gset=v1*fac;
	  iset=1;
	  return v2*fac;
	}
      else
	{
	  iset=0;
	  return gset;
	}
    }

// It returns random normally distributed values with zero mean and 
//sigma dispersion      
  mozgflt gauss(const mozgflt sigma)
    {
      return  gasdev() * sigma;
    }
  
// It returns random normally distributed values with sigma dispersion 
// and mean value mean
  mozgflt gauss(const mozgflt sigma,
		const mozgflt mean)
    {
      return  gasdev() * sigma + mean;
    }
  


// Return random regular distributed signum
  mozgflt rand_signum()
    {
      mozgflt sign=1;
      
      if ( random_number(0.,2.) > 1 ) sign=1;
      else sign=-1;
      return sign;
    }

}
