// $Id: vector_matrix_io.hh,v 1.2 2006/03/19 20:46:17 lightdruid Exp $
// vector_matrix_io.hh
// Kernel library, i/o functions
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

#ifndef _MOZG_VECTOR_MATRIX_IO_HH_
#  define  _MOZG_VECTOR_MATRIX_IO_HH_

#include <iostream>
#include <fstream>
#include <cstring>

#include "mozg/matrix.hh"
#include "mozg/vector.hh"
#include "mozg/types.hh"


namespace mozg {

template< class vec_t,class vec_size_t> std::ofstream& operator <<(std::ofstream& output,
    const vector< vec_t,vec_size_t>& vector);

template< class vec_t,class vec_size_t> std::ifstream& operator >>(std::ifstream& input,
    vector< vec_t,vec_size_t>& vector);

template< class vec_t,class vec_size_t> std::ostream& operator <<(std::ostream& output,
    const vector< vec_t,vec_size_t>& vector);

template< class matr_t,class matr_size_t> std::ofstream& operator <<(std::ofstream& output,
    const matrix< matr_t,matr_size_t>& matr);

template< class matr_t,class matr_size_t> std::ifstream& operator >>(std::ifstream& output,
    matrix< matr_t,matr_size_t>& matr);

template< class matr_t,class matr_size_t> std::ostream& operator <<(std::ostream& output,
    const matrix< matr_t,matr_size_t>& matr);

}

#endif // _MOZG_VECTOR_MATRIX_IO_HH_

