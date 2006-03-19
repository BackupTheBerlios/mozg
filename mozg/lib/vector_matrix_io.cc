// $Id: vector_matrix_io.cc,v 1.2 2006/03/19 20:46:17 lightdruid Exp $
// 
// vector_matrix_io.cc
// Kernel library, i/o functions
//


#include "mozg/vector_matrix_io.hh"


namespace mozg {

template< class vec_t,class vec_size_t> std::ofstream& operator <<(std::ofstream& output,
    const vector< vec_t,vec_size_t>& vector) {
    const vec_t* temp = vector.vec();
    vec_size_t size = vector.size();

    while (size--)
        output << ' ' << (*temp++);

    return output;
}
template std::ofstream & operator <<
    (std::ofstream & output, const vector< mozgflt,mozgint>&vector);
template std::ofstream & operator <<
    (std::ofstream & output, const vector< mozgint,mozgint>&vector);


template< class vec_t,class vec_size_t> std::ifstream& operator >>(std::ifstream& input,
    vector< vec_t,vec_size_t>& vector) {
    vec_t* temp = vector.vec();
    vec_size_t size = vector.size();

    while (size--)
        input >> (*temp++);

    return input;
}
template std::ifstream & operator >>
    (std::ifstream & input, vector< mozgflt,mozgint>&vector);
template std::ifstream & operator >>
    (std::ifstream & input, vector< mozgint,mozgint>&vector);


template< class vec_t,class vec_size_t> std::ostream& operator <<(std::ostream& output,
    const vector< vec_t,vec_size_t>& vector) {
    const vec_t* temp = vector.vec();
    vec_size_t size = vector.size();

    while (size--)
        output << (*temp++) << ' ';

    return output;
}
template std::ostream & operator <<
    (std::ostream & output, const vector< mozgflt,mozgint>&vector);
template std::ostream & operator <<
    (std::ostream & output, const vector< mozgint,mozgint>&vector);


template< class matr_t,class matr_size_t> std::ofstream& operator <<(std::ofstream& output,
    const matrix< matr_t,matr_size_t>& matr) {
    const matr_t* b = matr[0];
    matr_size_t j;
    matr_size_t num_rows = matr.rows();
    const matr_size_t num_columns = matr.cols();

    while (num_rows--) {
        j = num_columns;
        while (j--)
            output << ' ' << (*b++);
        output << std::endl;
    }
    return output;
}
template std::ofstream & operator <<
    (std::ofstream & output, const matrix< mozgflt,mozgint>&matr);
template std::ofstream & operator <<
    (std::ofstream & output, const matrix< mozgint,mozgint>&matr);


template< class matr_t,class matr_size_t> std::ifstream& operator >>(std::ifstream& input,
    matrix< matr_t,matr_size_t>& matr) {
    matr_t* b = matr[0];

    matr_size_t j;
    matr_size_t num_rows = matr.rows();
    matr_size_t num_columns = matr.cols();

    while (num_rows--) {
        j = num_columns;
        while (j--)
            input >> (*b++);
    }
    return input;
}
template std::ifstream & operator >>
    (std::ifstream & output, matrix< mozgflt,mozgint>&matr);
template std::ifstream & operator >>
    (std::ifstream & output, matrix< mozgint,mozgint>&matr);


template< class matr_t,class matr_size_t> std::ostream& operator <<(std::ostream& output,
    const matrix< matr_t,matr_size_t>& matr) {
    const matr_t* b = matr[0];
    matr_size_t num_rows = matr.rows();
    const matr_size_t num_columns = matr.cols();
    matr_size_t j;

    while (num_rows--) {
        j = num_columns;
        while (j--)
            output << ' ' << (*b++);
        output << std::endl;
    }
    return output;
}
template std::ostream & operator <<
    (std::ostream & output, const matrix< mozgflt,mozgint>&matr);
template std::ostream & operator <<
    (std::ostream & output, const matrix< mozgint,mozgint>&matr);

}
