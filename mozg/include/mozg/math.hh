// $Id: math.hh,v 1.1 2006/03/18 21:53:18 lightdruid Exp $
// math.hh
// mozg neuron output functions, them derivatives
// and some mathematic constants and numbers
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

#ifndef _MOZG_OUT_FUNCTIONS_HH_
#  define _MOZG_OUT_FUNCTIONS_HH_

# include <math.h>

# include "mozg/types.hh"


namespace mozg {

// Coefficient for scaling of second order term in Quickprop
  extern       mozgflt CUT_QUICKPROP;

// EPS and OVER_UNIT removes possibility of floating point exception
// (division by zero in Cross-Entropy, log-squared error)
// if EPS influences on computational resuts decrease it.
  extern       mozgflt EPS;
  extern const mozgflt UNIT;
  extern       mozgflt OVER_UNIT;
  extern const mozgflt TWO;
  extern const mozgflt HALF_UNIT;
  extern const mozgflt MOZG_2_PI;

// neuron output function:
  extern mozgflt mozg_exp (mozgflt , mozgflt );
  extern mozgflt mozg_th  (mozgflt , mozgflt );
  extern mozgflt mozg_lin (mozgflt , mozgflt );
  extern mozgflt mozg_rbf (mozgflt , mozgflt );
  extern mozgflt mozg_atg (mozgflt , mozgflt );

// user defined neuron output function:
  extern mozgflt mozg_weinet (mozgflt , mozgflt );
  extern mozgflt mozg_rbfnet (mozgflt , mozgflt );

// derivative for neuron output function:
  extern mozgflt mozg_D_exp (mozgflt , mozgflt, mozgflt );
  extern mozgflt mozg_D_th  (mozgflt , mozgflt, mozgflt );
  extern mozgflt mozg_D_lin (mozgflt , mozgflt, mozgflt );
  extern mozgflt mozg_D_rbf (mozgflt , mozgflt, mozgflt );
  extern mozgflt mozg_D_atg (mozgflt , mozgflt, mozgflt );

// derivative for user defined output function:
  extern mozgflt mozg_D_weinet (mozgflt , mozgflt, mozgflt );
  extern mozgflt mozg_D_rbfnet (mozgflt , mozgflt, mozgflt );
  
}


# endif // _MOZG_OUT_FUNCTIONS_HH_
