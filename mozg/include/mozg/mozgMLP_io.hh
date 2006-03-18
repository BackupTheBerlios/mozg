// $Id: mozgMLP_io.hh,v 1.1 2006/03/18 21:53:18 lightdruid Exp $
// mozgMLP_io.hh
// Multi-Layer Perceptron (mozgMLP) simulator i/o function
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

    You should have received a opy of the GNU Library General Public
    License along with this library; if not, write to the Free Software
    Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA
    02111-1307, USA.
              
 ****************************************************************************/

#ifndef _MOZG_MLP_IO_HH_
#  define _MOZG_MLP_IO_HH_

#include <cstring>
#include <iostream>
#include <fstream>
#include <assert.h>

#include "mozg/vector_matrix_io.hh"
#include "mozg/mozgMLP.hh"
#include "mozg/struct_layer.hh"
#include "mozg/vector_matrix.hh"
#include "mozg/matrix.hh"
#include "mozg/vector.hh"
#include "mozg/types.hh"
#include "mozg/config.h"


namespace mozg {

  std::ofstream& operator << (std::ofstream& output,
			 const mozgMLP& net);

  std::ifstream& operator >> (std::ifstream& input,
			 mozgMLP** p_p_net);

  std::ostream& operator << (std::ostream& output,
			const mozgMLP& net);

}


#endif // _MOZG_MLP_IO_HH_
