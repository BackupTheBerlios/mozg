// $Id: types.hh,v 1.2 2006/03/19 20:46:17 lightdruid Exp $
// types.hh
// mozg types
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

#ifndef _MOZG_TYPES_H_
#  define _MOZG_TYPES_H_


namespace mozg {

// Basic types:
typedef float mozgflt;
typedef int mozgint;

// neuron output function and its derivative types
typedef mozgflt (*OUT_FUNC)       (mozgflt, mozgflt);
typedef mozgflt (*OUT_FUNC_DERIV) (mozgflt, mozgflt, mozgflt);

}


#endif // _MOZG_TYPES_H_
