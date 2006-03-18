// $Id: random.hh,v 1.1 2006/03/18 21:53:18 lightdruid Exp $
// random.hh
// some functions for generating random distributions
//
/****************************************************************************

    Neural Network Library mozg
    Copyright (C) 1999 Alexy Filin

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Library General Public
    License as published by the Free Software Foundation; either
    version 2 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Library General Public License for more details.

    You should have received a copy of the GNU Library General Public
    License along with this library; if not, write to the Free Software
    Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA
    02111-1307, USA.
              
 ****************************************************************************/

#ifndef _MOZG_RANDOM_HH_
#  define _MOZG_RANDOM_HH_

#include <math.h>

#include "mozg/types.hh"


namespace mozg {

// Random number generator of Park and Miller
  extern mozgflt ran1(long* idum);

// It returns random regulary distributed in [argmin,argmax] number
  extern mozgflt random_number(const mozgflt argmin,
			       const mozgflt argmax);

// It returns a normally distributed deviate with zero mean 
// and unit variance
  extern mozgflt gasdev();

// It returns random normally distributed values with sigma dispersion
  extern mozgflt gauss(const mozgflt sigma);

// It returns random normally distributed values with sigma dispersion and 
// mean value mean
  extern mozgflt gauss(const mozgflt sigma,
		       const mozgflt mean);

// It returns random regular distributed signum
  extern mozgflt rand_signum();

// It returns signum (Heaviside step function)
  extern mozgflt mozg_heaviside(const mozgflt number);

}


#endif // _MOZG_RANDOM_HH_
