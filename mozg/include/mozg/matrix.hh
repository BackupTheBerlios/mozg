// $Id: matrix.hh,v 1.3 2006/03/23 13:14:19 lightdruid Exp $
// matrix.hh
// Kernel library, matrix and checked_matrix class templates
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

#ifndef _MOZG_MATRIX_HH_
#  define _MOZG_MATRIX_HH_

#include <math.h>
#include <assert.h>

#include "mozg/random.hh"
#include "mozg/types.hh"


namespace mozg
{

template< class matr_t,class matr_size_t> class matrix {
private:

    matr_t** p_p_matrix;   // pointer to poiter to first matrix element

    matr_size_t rows_num;   // the number of matrix rows
    matr_size_t columns_num;        // the number of matrix columns

public:

    // return the number of matrix rows
    matr_size_t rows() const {
        return rows_num;
    }

    // return the number of matrix columns
    matr_size_t cols() const {
        return columns_num;
    }

    // return pointer to poiter to first matrix element
    matr_t** matr() const {
        return p_p_matrix;
    }

    // return pointer to first matrix element
    matr_t* operator [](const matr_size_t number) const {
        return p_p_matrix[number];
    }

    // add the extra_matr to the matrix&
    matrix& operator +=(const matrix& extra_matr);

    // multiply the extra_matr by the number
    matrix& operator *=(matr_t number);

    // divide each element of this by the same element div_matr
    matrix& operator /=(const matrix& div_matr);

    // create matrix
    matrix(const matr_size_t number_rows, const matr_size_t number_columns) {
        rows_num = number_rows;
        columns_num = number_columns;

        matr_t* tmp = new matr_t[number_rows* number_columns];
        matr_t** mtmp;
        assert(tmp);

        p_p_matrix = new matr_t * [number_rows];
        assert(p_p_matrix);
        mtmp = p_p_matrix;
        matr_size_t rows_num = number_rows;

        while (rows_num--) {
            *mtmp++ = tmp;
            tmp += number_columns;
        }
    }

    // delete matrix
    virtual ~ matrix() {
        delete[] * p_p_matrix;
        delete[]p_p_matrix;
    }
};

///////////////////////////////////////////////////////////////////////////////

template< class matr_t,class matr_size_t> class checked_matrix : public matrix < matr_t,matr_size_t> {
private:
    matr_t max_abs;         // maximum of absolute value of matrix element
    matr_t min_abs;         // minimum of absolute value of matrix element

public:

    // put maximum of absolute value of matrix element
    void putMaxAbs(matr_t maximum_abs) {
        max_abs = maximum_abs;
    }

    // put minimum of absolute value of matrix element
    void putMinAbs(matr_t minimum_abs) {
        min_abs = minimum_abs;
    }

    // get maximum of absolute value of matrix element
    matr_t getMaxAbs() {
        return max_abs;
    }

    // get minimum of absolute value of matrix element
    matr_t getMinAbs() {
        return min_abs;
    }

    // check matrix elements for going out of matrix limits
    // returns:
    //     0 there're NO elements going out of max abs limit
    //     n there're n elements > max abs limit
    matr_size_t isOutMaxAbs();

    //     0 there're NO elements going out of min abs limit
    //     n there're n elements < min abs limit
    matr_size_t isOutMinAbs();

    // create checked matrix
    checked_matrix(const matr_size_t number_rows,
        const matr_size_t number_columns)
        : matrix< matr_t,matr_size_t>(number_rows, number_columns)
    {
        max_abs = 0;
        min_abs = 0;
    }

    // delete checked matrix
    virtual ~ checked_matrix() {
    }
};

///////////////////////////////////////////////////////////////////////////////

// It zeroes matrix
// WARNING !!! make sure the size(=rows_num*columns_num) don't go over
// the type size !!!
template< class matr_t,class matr_size_t> void zero(matrix < matr_t,matr_size_t>* matr);

// It copies number to copy matrix
// WARNING !!! make sure the size(=rows_num*columns_num) don't go over
// the type size !!!
template< class matr_t,class matr_size_t> void copy(matr_t number,
    matrix < matr_t,matr_size_t>* copy);

// It copies "orig" matrix to copy
// WARNING !!! make sure the size(=rows_num*columns_num) don't go over
// the type size !!!
template< class matr_t,class matr_size_t> void copy(const matrix < matr_t,matr_size_t>& orig,
    matrix < matr_t,matr_size_t>* copy);

// It copies "orig" matrix to copy
// WARNING !!! make sure the size(=rows_num*columns_num) don't go over
// the type size !!!
template< class matr_t,class matr_size_t> void copy(matr_t* orig,
    matrix < matr_t,matr_size_t>* copy);

// It computes sum of two matrices
// WARNING !!! make sure the size(=rows_num*columns_num) don't go over
// the type size !!!
template<class matr_t, class matr_size_t> 
    void sum(const matrix <matr_t,matr_size_t>& matrix1,
        const matrix <matr_t,matr_size_t>& matrix2,
        matrix <matr_t,matr_size_t>* sum_matr);

// It adds extra_matrix to matrix
// WARNING !!! make sure the size(=rows_num*columns_num) don't go over
// the type size !!!

template <class matr_t, class matr_size_t> 
    void sum(matrix <matr_t, matr_size_t>* m,
        const matrix <matr_t, matr_size_t>& extra_matrix);

// It multiplies matrix on d
// WARNING !!! make sure the size(=rows_num*columns_num) don't go over
// the type size !!!
template< class matr_t,class matr_size_t> void multiply(const matr_t d,
    matrix < matr_t,matr_size_t>& matrix);

// It computes product of two matrices
template< class matr_t,class matr_size_t> void multiply(const matrix < matr_t,matr_size_t>& matrix1,
    const matrix < matr_t,matr_size_t>& matrix2,
    matrix < matr_t,matr_size_t>* prod_matr);

// It transposes matrix
template< class matr_t,class matr_size_t> void transpose(const matrix < matr_t,matr_size_t>& origin_matrix,
    matrix < matr_t,matr_size_t>* transpon_matrix);

// It randomizes matrix by normal distribution with sigma and mean=0
// WARNING !!! make sure the size(=rows_num*columns_num) don't go over
// the type size !!!
template< class matr_t,class matr_size_t> void randomize(matrix < matr_t,matr_size_t>* matrix,
    const matr_t min, const matr_t max);

///////////////////////////////////////////////////////////////////////////////

// zero matrix and limits
template< class matr_t,class matr_size_t> void zero(checked_matrix < matr_t,matr_size_t>*);


}

#endif // _MOZG_MATRIX_HH_
