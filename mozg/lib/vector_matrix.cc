// $Id: vector_matrix.cc,v 1.2 2006/03/19 20:46:17 lightdruid Exp $
// 
// vector_matrix.cc
// Kernel library, mix operations with vector and matrix classes objects
//

#include "mozg/vector_matrix.hh"


namespace mozg {

// Computes out product v1 and v2
template< class vec_t,class vec_size_t,class matr_t,class matr_size_t> void multiply(const vector< vec_t,vec_size_t>& v1,
    const vector< vec_t,vec_size_t>& v2, matrix< matr_t,matr_size_t>* m) {
    matr_t* temp3 = m->matr() [0];
    const vec_t* temp1 = v1.vec();
    const vec_t* temp2;

    vec_size_t size1 = v1.size();
    const vec_size_t size2 = v2.size();
    vec_size_t i;

    while (size1--) {
        temp2 = v2.vec();
        i = size2;
        while (i--)
            *temp3++ = *temp1 * *temp2++;
        temp1++;
    }
}
template void multiply(const vector< mozgflt,mozgint>&,
                const vector< mozgflt,mozgint>&, matrix< mozgflt,mozgint>*);
template void multiply(const vector< mozgint,mozgint>&,
                const vector< mozgint,mozgint>&, matrix< mozgint,mozgint>*);

// It returns product matrix row and vector
template< class vec_t,class vec_size_t,class matr_t,class matr_size_t> void multiply(const matrix< matr_t,matr_size_t>& m,
    const vector< vec_t,vec_size_t>& v, vector< vec_t,vec_size_t>* product_v) {
    const matr_t* temp1 = m[0];
    const vec_t* vec = v.vec();
    vec_t* result_vec = product_v->vec();
    const vec_t* temp2;
    vec_t sum;

    matr_size_t i;
    matr_size_t num_rows = m.rows();
    const matr_size_t num_cols = m.cols();

    while (num_rows--) {
        i = num_cols;
        sum = (matr_t) 0.;
        temp2 = vec;
        while (i--)
            sum += *temp1++ * *temp2++;
        *result_vec++ = sum;
    }
}
template void multiply(const matrix< mozgflt,mozgint>&,
                const vector< mozgflt,mozgint>&, vector< mozgflt,mozgint>*);
template void multiply(const matrix< mozgint,mozgint>&,
                const vector< mozgint,mozgint>&, vector< mozgint,mozgint>*);

// It multiply each element of matrix row by one vector element and put it
// in relevant element of product_m row
template< class vec_t,class vec_size_t,class matr_t,class matr_size_t> void multiply(const matrix< matr_t,matr_size_t>& m,
    const vector< vec_t,vec_size_t>& v, matrix< matr_t,matr_size_t>* product_m) {
    const matr_t* temp1 = m[0];
    matr_t* result_matr = product_m->matr() [0];
    const vec_t* temp2 = v.vec();
    matr_size_t i;
    matr_size_t num_rows = m.rows();
    const matr_size_t num_cols = m.cols();

    while (num_rows--) {
        i = num_cols;
        while (i--)
            *result_matr++ = *temp1++ * *temp2;
        temp2++;
    }
}
template void multiply(const matrix< mozgflt,mozgint>&,
                const vector< mozgflt,mozgint>&, matrix< mozgflt,mozgint>*);
template void multiply(const matrix< mozgint,mozgint>&,
                const vector< mozgint,mozgint>&, matrix< mozgint,mozgint>*);

// It multiply each element of matrix column by one vector element and put it
// in relevant element of product_m column
template< class vec_t,class vec_size_t,class matr_t,class matr_size_t> void multiply(const vector< vec_t,vec_size_t>& v,
    const matrix< matr_t,matr_size_t>& m,
    matrix< matr_t,matr_size_t>* product_m) {
    const matr_t* temp1 = m[0];
    matr_t* result_matr = product_m->matr() [0];
    const vec_t* vec = v.vec();
    const vec_t* temp2;
    matr_size_t i;
    matr_size_t num_rows = m.rows();
    const matr_size_t num_cols = m.cols();

    while (num_rows--) {
        i = num_cols;
        temp2 = vec;
        while (i--)
            *result_matr++ = *temp1++ * *temp2++;
    }
}
template void multiply(const vector< mozgflt,mozgint>&,
                const matrix< mozgflt,mozgint>&, matrix< mozgflt,mozgint>*);
template void multiply(const vector< mozgint,mozgint>&,
                const matrix< mozgint,mozgint>&, matrix< mozgint,mozgint>*);

// It returns product matrix row and vector for the RBF out function
template< class vec_t,class vec_size_t,class matr_t,class matr_size_t> void multiply(const matrix< matr_t,matr_size_t>& m,
    const vector< vec_t,vec_size_t>& v,
    matrix< matr_t,matr_size_t>* RBFmatrix,
    vector< vec_t,vec_size_t>* result_v) {
    const matr_t* temp1 = m[0];
    matr_t* RBFtemp = (*RBFmatrix)[0];

    const vec_t* vec = v.vec();
    vec_t* result_vec = result_v->vec();
    const vec_t* temp2;
    vec_t sum;

    matr_size_t i;
    matr_size_t num_rows = m.rows();
    matr_size_t num_cols = m.cols();

    while (num_rows--) {
        i = num_cols;
        sum = (vec_t) 0.;
        temp2 = vec;
        while (i--) {
            *RBFtemp = *temp1++ - *temp2++;
            sum += *RBFtemp * *RBFtemp;
            RBFtemp++;
        }
        *result_vec++ = sum;
    }
}
template void multiply(const matrix< mozgflt,mozgint>&,
                const vector< mozgflt,mozgint>&, matrix< mozgflt,mozgint>*,
                vector< mozgflt,mozgint>*);
template void multiply(const matrix< mozgint,mozgint>&,
                const vector< mozgint,mozgint>&, matrix< mozgint,mozgint>*,
                vector< mozgint,mozgint>*);

// It returns product matrix column and vector
template< class vec_t,class vec_size_t,class matr_t,class matr_size_t> void multiply(const vector< vec_t,vec_size_t>& v,
    const matrix< matr_t,matr_size_t>& m, vector< vec_t,vec_size_t>* product_v) {
    const matr_t* first_row = m[0];
    const matr_t* temp1;
    const vec_t* vec = v.vec();
    const vec_t* temp2;
    vec_t* result_vec = product_v->vec();
    vec_t sum = 0;

    const matr_size_t num_rows = m.rows();
    const matr_size_t num_cols = m.cols();
    matr_size_t i = num_cols;
    matr_size_t j;

    while (i--) {
        j = num_rows;
        sum = 0;
        temp2 = vec;
        temp1 = first_row++;
        while (j--) {
            sum += *temp1 * *temp2++;
            temp1 += num_cols;
        }
        *result_vec++ = sum;
    }
}
template void multiply(const vector< mozgflt,mozgint>&,
                const matrix< mozgflt,mozgint>&, vector< mozgflt,mozgint>*);
template void multiply(const vector< mozgint,mozgint>&,
                const matrix< mozgint,mozgint>&, vector< mozgint,mozgint>*);

}
