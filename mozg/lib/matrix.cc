// $Id: matrix.cc,v 1.2 2006/03/19 20:46:17 lightdruid Exp $
// 
// matrix.cc
// Kernel library, operations with matrix and checked_matrix class templates
// objects

#include "mozg/matrix.hh"


namespace mozg {

// divide each element of this by the same element div_matr

template< class matr_t,class matr_size_t> matrix<matr_t, matr_size_t>&
matrix<matr_t, matr_size_t> ::
operator /=(const matrix& div_matr) {
    matr_size_t i;
    matr_t* temp1 = p_p_matrix[0];
    const matr_t* temp2 = div_matr[0];

    for (i = rows_num*columns_num; i--;)
        *temp1++ /= *temp2++;

    return *this;
}
template matrix< mozgflt,mozgint>&matrix< mozgflt,mozgint>::operator /= (const matrix &);
template matrix< mozgint,mozgint>&matrix< mozgint,mozgint>::operator /= (const matrix &);


// It addes extra_matr to matr
template< class matr_t,class matr_size_t> matrix<matr_t,matr_size_t>&
matrix<matr_t,matr_size_t> ::
operator +=(const matrix< matr_t,matr_size_t>& extra_matr) {
    matr_t* temp1 = p_p_matrix[0];
    const matr_t* temp2 = extra_matr[0];

    // WARNING !!! make sure the size don't go over the type size !!!
    matr_size_t size = rows_num* columns_num;

    while (size--)
        *temp1++ += *temp2++;

    return *this;
}
template matrix< mozgflt,mozgint>&matrix< mozgflt,mozgint>::
operator += (const matrix< mozgflt,mozgint>&);
template matrix< mozgint,mozgint>&matrix< mozgint,mozgint>::
operator += (const matrix< mozgint,mozgint>&);


// multiply the extra_matr by the number
template< class matr_t,class matr_size_t> matrix<matr_t,matr_size_t>&
matrix<matr_t,matr_size_t> ::operator *=(matr_t number) {
    matr_t* temp1 = p_p_matrix[0];

    // WARNING !!! make sure the size don't go over the type size !!!
    matr_size_t size = rows_num* columns_num;

    while (size--) {
        *temp1 = *temp1 * number;
        temp1++;
    }
    return *this;
}
template matrix< mozgflt,mozgint>&matrix< mozgflt,mozgint>::operator *= (mozgflt);
template matrix< mozgint,mozgint>&matrix< mozgint,mozgint>::operator *= (mozgint);


///////////////////////////////////////////////////////////////////////////////

// check matrix elements for going out of upper limits
#if 0
  template <class matr_t, class matr_size_t> 
    matr_size_t isOutMaxAbs ()
    {
      matr_t* tmp = *p_p_matrix;
      matr_size_t checking_result = 0;

      for (matr_size_t i=rows_num * columns_num; i--;)
    if (*tmp++ > max_abs) checking_result += 1;
      return checking_result;
    }
#endif
template< mozgint,mozgint> mozgint isOutMaxAbs();

#if 0
// check matrix elements for going out of lower limits
  template <class matr_t, class matr_size_t> 
    matr_size_t isOutMinAbs ()
    {
      matr_t* tmp = *p_p_matrix;
      matr_size_t checking_result = 0;

      for (matr_size_t i=rows_num * columns_num; i--;)
    if (*tmp++ < min_abs) checking_result += 1;
      return checking_result;
    }
#endif
template< mozgint,mozgint> mozgint isOutMinAbs();

///////////////////////////////////////////////////////////////////////////////

// It zeroes matr
template< class matr_t,class matr_size_t> void zero(matrix< matr_t,matr_size_t>* matr) {
    matr_t* b = (*matr)[0];

    // WARNING !!! make sure the size don't go over the type size !!!
    matr_size_t size = matr->rows() * matr->cols();

    while (size--)
        *b++ = (matr_t) 0.;
}
template void zero(matrix< mozgflt,mozgint>*);
template void zero(matrix< mozgint,mozgint>*);

// It copies number to copy matrix
template< class matr_t,class matr_size_t> void copy(matr_t number,
    matrix< matr_t,matr_size_t>* copy) {
    // WARNING !!! make sure the size don't go over the type size !!!
    matr_size_t size = copy->rows() * copy->cols();
    matr_t* cp = copy->matr()[0];

    while (size--)
        *cp++ = number;
}
template void copy(mozgflt number, matrix< mozgflt,mozgint>*);
template void copy(mozgint number, matrix< mozgint,mozgint>*);

// It makes matrix copy
template< class matr_t,class matr_size_t> void copy(const matrix< matr_t,matr_size_t>& orig,
    matrix< matr_t,matr_size_t>* copy) {
    // WARNING make sure the size don't go over the type size
    matr_size_t size = orig.rows() * orig.cols();

    const matr_t* orr = orig[0];
    matr_t* cp = (*copy)[0];

    while (size--)
        *cp++ = *orr++;
}
template void copy(const matrix< mozgflt,mozgint>&, matrix< mozgflt,mozgint>*);
template void copy(const matrix< mozgint,mozgint>&, matrix< mozgint,mozgint>*);

template< class matr_t,class matr_size_t> void copy(const matr_t* orig,
    matrix< matr_t,matr_size_t>* copy) {
    // WARNING !!! make sure the size don't go over the type size !!!
    matr_t* cp = (*copy)[0];
    matr_size_t size = copy->rows() * copy->cols();

    while (size--)
        *cp++ = *orig++;
}
//template void copy (mozgflt*  orig,
//            matrix<mozgflt,mozgint>*);
//template void copy (mozgflt*  orig,
//            matrix<mozgflt,mozgint>*);

// It addes matrices
template< class matr_t,class matr_size_t> void sum(const matrix< matr_t,matr_size_t>& matrix1,
    const matrix< matr_t,matr_size_t>& matrix2,
    matrix< matr_t,matr_size_t>* sum_matr) {
    const matr_t* temp1 = matrix1[0];
    const matr_t* temp2 = matrix2[0]; 
    matr_t* temp3 = (*sum_matr)[0];

    // WARNING !!! make sure the size don't go over the type size !!!
    matr_size_t size = matrix1.rows() * matrix1.cols();

    while (size--)
        *temp3++ = *temp1++ + *temp2++;
}
template void sum(const matrix< mozgflt,mozgint>&,
                const matrix< mozgflt,mozgint>&, matrix< mozgflt,mozgint>*);
template void sum(const matrix< mozgint,mozgint>&,
                const matrix< mozgint,mozgint>&, matrix< mozgint,mozgint>*);


// It addes matrices
template< class matr_t,class matr_size_t> void subtract(const matrix< matr_t,matr_size_t>& matrix1,
    const matrix< matr_t,matr_size_t>& matrix2,
    matrix< matr_t,matr_size_t>* result_matr) {
    const matr_t* temp1 = matrix1[0];
    const matr_t* temp2 = matrix2[0]; 
    matr_t* temp3 = (*result_matr)[0];

    // WARNING !!! make sure the size don't go over the type size !!!
    matr_size_t size = matrix1.rows() * matrix1.cols();

    while (size--)
        *temp3++ = *temp1++ - *temp2++;
}
template void subtract(const matrix< mozgflt,mozgint>&,
                const matrix< mozgflt,mozgint>&, matrix< mozgflt,mozgint>*);
template void subtract(const matrix< mozgint,mozgint>&,
                const matrix< mozgint,mozgint>&, matrix< mozgint,mozgint>*);


// It multiplies matrix on number
template< class matr_t,class matr_size_t> void multiply(const matr_t d,
    matrix< matr_t,matr_size_t>& matrix1) {
    matr_t* temp = matrix1[0];

    // WARNING !!! make sure the size don't go over the type size !!!
    matr_size_t size = matrix1.rows() * matrix1.cols();

    while (size--)
        *temp++ *= d;
}
template void multiply(const mozgflt, matrix< mozgflt,mozgint>&matrix1);
template void multiply(const mozgint, matrix< mozgint,mozgint>&matrix1);

// Matrice product
template< class matr_t,class matr_size_t> void multiply(const matrix< matr_t,matr_size_t>& matrix1,
    const matrix< matr_t,matr_size_t>& matrix2,
    matrix< matr_t,matr_size_t>* prod_matr) {
    matr_size_t i;
    matr_size_t j;
    matr_size_t k;
    const matr_size_t rows1 = matrix1.rows();
    const matr_size_t cols1 = matrix1.cols();
    const matr_size_t cols2 = matrix2.cols();

    matr_t** matr1 = matrix1.matr();
    matr_t** matr2 = matrix2.matr();
    matr_t* temp3 = (*prod_matr)[0];
    matr_t* temp1;
    matr_t* temp2;

    for (i = rows1; i--;) {
        for (k = 0; k < cols2; k++) {
            *temp3 = 0;
            temp2 = &(matr2[0][k]);
            temp1 = *matr1++;
            for (j = cols1; j--;) {
                *temp3 += *temp1++ * *temp2;
                temp2 += cols2;
            }
            temp3++;
        }
    }
}
template void multiply(const matrix< mozgflt,mozgint>&,
                const matrix< mozgflt,mozgint>&, matrix< mozgflt,mozgint>*);
template void multiply(const matrix< mozgint,mozgint>&,
                const matrix< mozgint,mozgint>&, matrix< mozgint,mozgint>*);

// Transpon matrix
template< class matr_t,class matr_size_t> void transpose(const matrix< matr_t,matr_size_t>& origin,
    matrix< matr_t,matr_size_t>* transpon) {
    const matr_t* temp1 = origin[0];
    matr_t* temp2 = (*transpon)[0];

    matr_size_t i;
    matr_size_t j;
    const matr_size_t rows_num = origin.rows();
    const matr_size_t num_cols = origin.cols();

    for (i = rows_num; i--;) {
        temp2 = (*transpon)[i];
        for (j = num_cols; j--;) {
            *temp2 = *temp1++;
            temp2 += num_cols;
        }
    }
}
template void transpose(const matrix< mozgflt,mozgint>&,
                matrix< mozgflt,mozgint>*);
template void transpose(const matrix< mozgint,mozgint>&,
                matrix< mozgint,mozgint>*);

// It randomizes matrix
template< class matr_t,class matr_size_t> void randomize(matrix< matr_t,matr_size_t>* matrix1,
    const matr_t min, const matr_t max) {
    matr_t* temp = (*matrix1)[0];

    // WARNING !!! make sure the size don't go over the type size !!!
    matr_size_t size = matrix1->rows() * matrix1->cols();

    while (size--)
        *temp++ = (matr_t) random_number(min, max);
}
template void randomize< mozgflt,mozgint>(matrix< mozgflt,mozgint>*,
    const mozgflt, const mozgflt);
//  template void randomize<mozgflt,mozgint> (matrix<mozgint,mozgint>*,
//                      const mozgflt,
//                      const mozgflt);

///////////////////////////////////////////////////////////////////////////////

// zero matrix and limits
template< class matr_t,class matr_size_t> void zero(checked_matrix< matr_t,matr_size_t>* m) {
    zero((matrix< matr_t,matr_size_t>*) m);
    m->putMaxAbs((matr_t) 0);
    m->putMinAbs((matr_t) 0);
}
template void zero(checked_matrix< mozgflt,mozgint>*);
template void zero(checked_matrix< mozgint,mozgint>*);

}
