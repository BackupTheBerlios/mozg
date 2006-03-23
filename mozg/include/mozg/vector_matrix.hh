// $Id: vector_matrix.hh,v 1.3 2006/03/23 13:14:19 lightdruid Exp $
// vector_matrix.hh
// Kernel library, mix operatiions with objects of vector and matrix
// classes
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

#ifndef _MOZG_VECTOR_MATRIX_HH_
#  define _MOZG_VECTOR_MATRIX_HH_

#include <math.h>
#include <iostream>
#include <fstream>

#include "mozg/matrix.hh"
#include "mozg/vector.hh"
#include "mozg/types.hh"


namespace mozg {

// It computes out vector product of v1 and v2
template< class vec_t,class vec_size_t,class matr_t,class matr_size_t> void multiply(const vector< vec_t,vec_size_t>& v1,
    const vector< vec_t,vec_size_t>& v2, matrix< matr_t,matr_size_t>* matr);

// It computes product of matrix and vector
template< class vec_t,class vec_size_t,class matr_t,class matr_size_t> void multiply(const matrix< matr_t,matr_size_t>& matrix,
    const vector< vec_t,vec_size_t>& v,
    vector< vec_t,vec_size_t>* product_vector);

// It multiply each element of matrix row and vector and put it in relevent
// element of product_matrix row
template< class vec_t,class vec_size_t,class matr_t,class matr_size_t> void multiply(const matrix< matr_t,matr_size_t>& m,
    const vector< vec_t,vec_size_t>& v,
    matrix< matr_t,matr_size_t>* product_matrix);

// It multiply each element of matrix column by one vector element and put it
// in relevant element of product_matrix column
template< class vec_t,class vec_size_t,class matr_t,class matr_size_t> void multiply(const vector< vec_t,vec_size_t>& v,
    const matrix< matr_t,matr_size_t>& m,
    matrix< matr_t,matr_size_t>* product_matrix);

// It computes product of matrix and vector for the RBF out function
template< class vec_t,class vec_size_t,class matr_t,class matr_size_t> void multiply(const matrix< matr_t,matr_size_t>& m,
    const vector< vec_t,vec_size_t>& v,
    matrix< matr_t,matr_size_t>* RBFmatrix,
    vector< vec_t,vec_size_t>* product_vector);

// It computes product of vector and matrix
template< class vec_t,class vec_size_t,class matr_t,class matr_size_t> void multiply(const vector< vec_t,vec_size_t>& v,
    const matrix< matr_t,matr_size_t>& matrix,
    vector< vec_t,vec_size_t>* product_vector);

}


#endif // _MOZG_VECTOR_MATRIX_HH_
