// $Id: vector.cc,v 1.1 2006/03/18 21:53:18 lightdruid Exp $
// 
// vector.cc
// Kernel library, operations with vector and checked_vector class templates
// objects

#include "mozg/vector.hh"


namespace mozg {

// compare the number with each element of the vector
  template <class vec_t, class vec_size_t>
    bool operator != (const vector<vec_t,vec_size_t>& v,
		      const vec_t number)
    {
      vec_t* tmp      = v.vec();
      vec_size_t dim  = v.size();
      
      while (dim--) if (*tmp++ == number) return 0;
      return 1;
    }
  template bool operator != (const vector<mozgflt,mozgint>&,
			     const mozgflt);
  
  template bool operator != (const vector<mozgint,mozgint>&,
			     const mozgint);


// compare the number with each element of the vector
  template <class vec_t, class vec_size_t>
    bool operator == (const vector<vec_t,vec_size_t>& v,
		      const vec_t number)
    {
      vec_t* tmp      = v.vec();
      vec_size_t dim  = v.size();
      
      while (dim--) if (*tmp++ != number) return 0;
      return 1;
    }
  template bool operator == (const vector<mozgflt,mozgint>&,
			     const mozgflt);
  
  template bool operator == (const vector<mozgint,mozgint>&,
			     const mozgint);
  

// compare each element of *this with relevant one of the vector
  template <class vec_t, class vec_size_t>
    bool operator == (const vector<vec_t,vec_size_t>& v,
		      const vector<vec_t,vec_size_t>& vector2)
    {
      vec_t* tmp      = v.vec();
      vec_t* tmp2     = vector2.vec();
      vec_size_t dim  = v.size();
      
      while (dim--) if (*tmp++ != *tmp2++) return 0;
      return 1;
    }
  template bool operator == (const vector<mozgflt,mozgint>&,
			     const vector<mozgflt,mozgint>&);
  
  template bool operator == (const vector<mozgint,mozgint>&,
			     const vector<mozgint,mozgint>&);
  

// compare each element of *this with relevant one of the vector
  template <class vec_t, class vec_size_t>
    bool operator == (const vector<vec_t,vec_size_t>& v,
		      const vec_t* vector2)
    {
      const vec_t* tmp      = v.vec();
      const vec_t* tmp2     = vector2;
      vec_size_t dim  = v.size();
      
      while (dim--) if (*tmp++ != *tmp2++) return 0;
      return 1;
    }
  template bool operator == (const vector<mozgflt,mozgint>&,
			     const mozgflt*);
  
  template bool operator == (const vector<mozgint,mozgint>&,
			     const mozgint*);
  
// make OR each element of first vector with relevant element of second one
  template <class vec_t, class vec_size_t> vector<vec_t,vec_size_t>& 
    vector<vec_t,vec_size_t>::operator |= (const vector& vector2)
    {
      vec_t* temp1       = p_vector;
      const vec_t* temp2 = vector2.vec();
      vec_size_t tmp     = size();
      
      while (tmp--){
	*temp1 = (*temp1 || *temp2);
	temp1++;
	temp2++;
      }
      return *this;
    }
  template vector<mozgflt,mozgint>& vector<mozgflt,mozgint>::
    operator |= (const vector&);
  
  template vector<mozgint,mozgint>& vector<mozgint,mozgint>::
    operator |= (const vector&);
  


// added the number to each element of first vector
  template <class vec_t, class vec_size_t> vector<vec_t,vec_size_t>& 
    vector<vec_t,vec_size_t>::operator += (const vec_t number)
    {
      vec_t* temp    = p_vector;
      vec_size_t tmp = size();
      
      while (tmp--) *temp++ += number;
      
      return *this;
    }
  template vector<mozgflt,mozgint>&
    vector<mozgflt,mozgint>::operator += (const mozgflt);
  
  template vector<mozgint,mozgint>&
    vector<mozgint,mozgint>::operator += (const mozgint);
  

// add each element of second vector to relevant element of first one
  template <class vec_t, class vec_size_t> vector<vec_t,vec_size_t>& 
    vector<vec_t,vec_size_t>::operator += (const vector& vector2)
    {
      vec_t* temp1       = p_vector;
      const vec_t* temp2 = vector2.vec();
      vec_size_t tmp     = size();
      
      while (tmp--) *temp1++ += *temp2++;
      return *this;
    }
  template vector<mozgflt,mozgint>& vector<mozgflt,mozgint>::
    operator += (const vector&);
  
  template vector<mozgint,mozgint>& vector<mozgint,mozgint>::
    operator += (const vector&);
  

// add each element of second vector to relevant element of first one
  template <class vec_t, class vec_size_t> vector<vec_t,vec_size_t>& 
    vector<vec_t,vec_size_t>::operator += (const vec_t* vector2)
    {
      vec_t* temp1       = p_vector;
      const vec_t* temp2 = vector2;
      vec_size_t tmp     = size();

      while (tmp--) *temp1++ += *temp2++;
      
      return *this;
    }
  template vector<mozgflt,mozgint>& vector<mozgflt,mozgint>::
    operator += (const mozgflt*);
  
  template vector<mozgint,mozgint>& vector<mozgint,mozgint>::
    operator += (const mozgint*);


// subtruct each element of second vector from relevant element of first one
  template <class vec_t, class vec_size_t> vector<vec_t,vec_size_t>& 
    vector<vec_t,vec_size_t>::operator -= (const vector& vector2)
    {
      vec_t* temp1       = p_vector;
      const vec_t* temp2 = vector2.vec();
      vec_size_t tmp     = size();
      
      while (tmp--) *temp1++ -= *temp2++;
      return *this;
    }
  template vector<mozgflt,mozgint>& vector<mozgflt,mozgint>::
    operator -= (const vector&);
  
  template vector<mozgint,mozgint>& vector<mozgint,mozgint>::
    operator -= (const vector&);
  
  
  
// multiply each element of first vector by  the number
  template <class vec_t, class vec_size_t> vector<vec_t,vec_size_t>& 
    vector<vec_t,vec_size_t>::operator *= (const vec_t number)
    {
      vec_t* temp    = p_vector;
      vec_size_t tmp = size();
      
      while (tmp--) *temp++ *= number;
      
      return *this;
    }
  template vector<mozgflt,mozgint>&
    vector<mozgflt,mozgint>::operator *= (const mozgflt);
  
  template vector<mozgint,mozgint>&
    vector<mozgint,mozgint>::operator *= (const mozgint);
  

// multiply each element of first vector by relevant element of second one
  template <class vec_t, class vec_size_t>
    vector<vec_t,vec_size_t>&vector<vec_t,vec_size_t>::
    operator *= (const vector& vector2)
    {
      vec_t* temp1       = p_vector;
      const vec_t* temp2 = vector2.vec();
      vec_size_t tmp     = vector_size;
      
      while (tmp--) *temp1++ *= *temp2++;
      
      return *this;
    }
  template vector<mozgflt,mozgint>&
    vector<mozgflt,mozgint>::operator *= (const vector&);
  
  template vector<mozgint,mozgint>&
    vector<mozgint,mozgint>::operator *= (const vector&);
  
  
// multiply each element of first vector by relevant element of second one
  template <class vec_t, class vec_size_t>
    vector<vec_t,vec_size_t>&vector<vec_t,vec_size_t>::
    operator *= (const vec_t* vector2)
    {
      vec_t* temp1       = p_vector;
      const vec_t* temp2 = vector2;
      vec_size_t tmp     = vector_size;
      
      while (tmp--) *temp1++ *= *temp2++;
      
      return *this;
    }
  template vector<mozgflt,mozgint>&
    vector<mozgflt,mozgint>::operator *= (const mozgflt*);
  
  template vector<mozgint,mozgint>&
    vector<mozgint,mozgint>::operator *= (const mozgint*);
  
  
  template <class vec_t, class vec_size_t> vector<vec_t,vec_size_t>& 
    vector<vec_t,vec_size_t>::operator /= (vec_t number)
    {
      vec_t* temp    = p_vector;
      vec_size_t tmp = vector_size;
      
      while (tmp--) *temp++ /= number;
      
      return *this;
    }
  template vector<mozgflt,mozgint>&
    vector<mozgflt,mozgint>::operator /= (mozgflt);
  
  template vector<mozgint,mozgint>&
    vector<mozgint,mozgint>::operator /= (mozgint);
  
  
  template <class vec_t, class vec_size_t> vector<vec_t,vec_size_t>& 
    vector<vec_t,vec_size_t>::operator /= (const vector& denominator)
    {
      vec_t* temp1       = p_vector;
      const vec_t* temp2 = denominator.vec();
      vec_size_t tmp = vector_size;
      
      while (tmp--) *temp1++ /= *temp2++;

      return *this;
    }
  template vector<mozgflt,mozgint>&
    vector<mozgflt,mozgint>::operator /= (const vector&);
  
  template vector<mozgint,mozgint>&
    vector<mozgint,mozgint>::operator /= (const vector&);
  
  
  template <class vec_t, class vec_size_t> vector<vec_t,vec_size_t>& 
    vector<vec_t,vec_size_t>::operator /= (const vec_t* denominator)
    {
      vec_t* temp1       = p_vector;
      const vec_t* temp2 = denominator;
      vec_size_t tmp     = vector_size;
      
      while (tmp--) *temp1++ /= *temp2++;
      
      return *this;
    }
  template vector<mozgflt,mozgint>&
    vector<mozgflt,mozgint>::operator /= (const mozgflt*);
  
  template vector<mozgint,mozgint>&
    vector<mozgint,mozgint>::operator /= (const mozgint*);

///////////////////////////////////////////////////////////////////////////////

// check matrix elements for going out of upper limits
#if 0
  template <class vec_t, class vec_size_t> 
    vec_size_t isOutMaxAbs ()
    {
      vec_t* tmp = p_vector;
      vec_size_t checking_result = 0;

      for (vec_size_t i=rows_num * columns_num; i--;)
	if (*tmp++ > max_abs) checking_result += 1;
      return checking_result;
    }
#endif
  template<mozgint,mozgint> mozgint isOutMaxAbs ();

#if 0
// check matrix elements for going out of lower limits
  template <class vec_t, class vec_size_t> 
    vec_size_t isOutMinAbs ()
    {
      vec_t* tmp = p_vector;
      vec_size_t checking_result = 0;

      for (vec_size_t i=rows_num * columns_num; i--;)
	if (*tmp++ < min_abs) checking_result += 1;
      return checking_result;
    }
#endif
  template<mozgint,mozgint> mozgint isOutMinAbs ();

///////////////////////////////////////////////////////////////////////////////
  
// It returns min element
  template <class vec_t, class vec_size_t> 
    vec_t  min(const vector<vec_t,vec_size_t>&v)
    {
      const vec_t* a  = v.vec();
      vec_size_t size = v.size();
      vec_t setmin;
      
      for (setmin = a[--size]; size;
	   setmin = setmin > a[--size] ? a[size] : setmin);
      
      return setmin;
    }
  template mozgflt min(const vector<mozgflt,mozgint>&);
  template mozgint min(const vector<mozgint,mozgint>&);
  
// It returns max element
  template <class vec_t, class vec_size_t> 
    vec_t  max(const vector<vec_t,vec_size_t>& v)
    {
      const vec_t* a  = v.vec();
      vec_size_t size = v.size();
      vec_t setmax;
      
      for (setmax = a[--size]; size;
	   setmax = setmax < a[--size] ? a[size] : setmax);
      
      return setmax;
    }
  template mozgflt max(const vector<mozgflt,mozgint>&);
  template mozgint max(const vector<mozgint,mozgint>&);
  
// It subtract vector2 from vector1
  template <class vec_t, class vec_size_t> 
    void  subtract(const vec_t* vector1,
		   const vector<vec_t,vec_size_t>& vector2,
		   vector<vec_t,vec_size_t>* result_vec)
    {
      const vec_t* temp1 = vector1;
      const vec_t* temp2 = vector2.vec();
      vec_t* temp3       = result_vec->vec();
      vec_size_t size    = vector2.size();

      while (size--) *temp3++ = *temp1++ - *temp2++;  
    }
  template void subtract(const mozgflt*,
			 const vector<mozgflt,mozgint>&,
			 vector<mozgflt,mozgint>*);
  template void subtract(const mozgint*,
			 const vector<mozgint,mozgint>&,
			 vector<mozgint,mozgint>*);
  
// It subtract vector2 from vector1
  template <class vec_t, class vec_size_t> 
    void subtract(const vector<vec_t,vec_size_t>& vector1,
		  const vec_t* vector2,
		  vector<vec_t,vec_size_t>* result_vec)
    {
      
      const vec_t* temp1 = vector1.vec();
      const vec_t* temp2 = vector2;
      vec_t* temp3       = result_vec->vec();
      vec_size_t size    = vector1.size();

      while (size--) *temp3++ = *temp1++ - *temp2++;
    }
  template void subtract(const vector<mozgflt,mozgint>&,
			 const mozgflt*,			
			 vector<mozgflt,mozgint>*);
  template void subtract(const vector<mozgint,mozgint>&,
			 const mozgint*,			
			 vector<mozgint,mozgint>*);
  
// It subtract vector2 from vector1
  template <class vec_t, class vec_size_t> 
    void subtract(const vector<vec_t,vec_size_t>& vector1,
		  const vector<vec_t,vec_size_t>& vector2,
		  vector<vec_t,vec_size_t>* result_vec)
    {
      const vec_t* temp1 = vector1.vec();
      const vec_t* temp2 = vector2.vec();
      vec_t* temp3       = result_vec->vec();
      vec_size_t size    = vector1.size();

      while (size--) *temp3++ = *temp1++ - *temp2++;
    }
  template void subtract(const vector<mozgflt,mozgint>&,
			 const vector<mozgflt,mozgint>&,
			 vector<mozgflt,mozgint>*);
  template void subtract(const vector<mozgint,mozgint>&,
			 const vector<mozgint,mozgint>&,
			 vector<mozgint,mozgint>*);
  
// It returns inner(scalar) product of two vectors
  template <class vec_t, class vec_size_t> 
    void multiply(const vector<vec_t,vec_size_t>& vector1,
		  const vector<vec_t,vec_size_t>& vector2,
		  vec_t * product)
    {
      const vec_t* v1  = vector1.vec();
      const vec_t* v2  = vector2.vec();
      vec_size_t size  = vector1.size();
      vec_t  sum       = 0;

      while (size--) sum += *v1++ * *v2++;
      
      *product = sum;
    }
  template void multiply(const vector<mozgflt,mozgint>&,
			 const vector<mozgflt,mozgint>&,
			 mozgflt* product);
  template void multiply(const vector<mozgint,mozgint>&,
			 const vector<mozgint,mozgint>&,
			 mozgint* product);

// It puts number to all copy's elements
  template <class vec_t, class vec_size_t> 
    void copy(const vec_t number,
	      vector<vec_t,vec_size_t>* copy)
    {
      vec_t* cp       = copy->vec();
      vec_size_t size = copy->size();
      
      while (size--) *cp++ = number;
    }
  template void copy(const mozgflt,
		     vector<mozgflt,mozgint>* copy);
  template void copy(const mozgint,
		     vector<mozgint,mozgint>* copy);
  
// It copies orig_vector to copy
  template <class vec_t, class vec_size_t> 
    void copy(const vec_t* orig_vector,
	      vector<vec_t,vec_size_t>* copy)
    {
      const vec_t* orr = orig_vector;
      vec_t* cp       = copy->vec();
      vec_size_t size = copy->size();
      
      while (size--) *cp++ = *orr++;
    }
  template void copy(const mozgflt*,
		     vector<mozgflt,mozgint>* copy);
  template void copy(const mozgint*,
		     vector<mozgint,mozgint>* copy);
  
// It copies vector
  template <class vec_t, class vec_size_t> 
    void copy(const vector<vec_t,vec_size_t>& orig,
	      vector<vec_t,vec_size_t>* copy)
    {
      const vec_t* orr = orig.vec();
      vec_t* cp       = copy->vec();
      vec_size_t size = orig.size();
      
      
      while (size--) *cp++ = *orr++;
    }
  template void copy(const vector<mozgflt,mozgint>& orig,
		     vector<mozgflt,mozgint>* copy);
  template void copy(const vector<mozgint,mozgint>& orig,
		     vector<mozgint,mozgint>* copy);
  
// It zeroes vector
  template <class vec_t, class vec_size_t> 
    void zero(vector<vec_t,vec_size_t>* v)
    {
      vec_t* a        = v->vec();
      vec_size_t size = v->size();
      
      while (size--) *a++ = 0;
    }
  template void zero(vector<mozgflt,mozgint>* v);
  template void zero(vector<mozgint,mozgint>* v);
  
// It sortes vector by increase
  template <class vec_t, class vec_size_t> 
    void sort(vector<vec_t,vec_size_t>* v)
    {
      vec_t* a        = v->vec();
      vec_t temp;
      vec_size_t size = v->size();
      vec_size_t j;
      bool flag = 1;
      
      while ( size-- && flag)
	{
	  flag = 0;
	  j = size;
	  while (j--)
	    {
	      if ( *a > *(a + 1) )
		{
		  temp     = *a;
		  *a       = *(a + 1);
		  *(a + 1) = temp;
		  flag = 1;
		}
	      a++;
	    }
	}
    }
  template void sort(vector<mozgflt,mozgint>*);
  template void sort(vector<mozgint,mozgint>*);
  
// It makes indirect adresing ranked by increasing massive
  template <class vec_t, class vec_size_t> 
    void rank(const vector<vec_t,vec_size_t>& v,
	      vector<vec_size_t,vec_size_t>* rank_vec)
    {
      const vec_t* vec        = v.vec();
      vec_size_t* rank_vector = rank_vec->vec();
      vec_size_t size         = v.size();
      vec_size_t i            = v.size();
      vec_size_t k;
      vec_size_t j;

      rank_vector += size - 1;
      while (--i) *rank_vector-- = i;
      
      while (--size)
	{
	  j = size;
	  while (--j)
	    if (vec[rank_vector[size]]
		< vec[rank_vector[j]])
	      {
		k                 = rank_vector[size];
		rank_vector[size] = rank_vector[j];
		rank_vector[j]    = k;
	      }
	}
    }
  template void rank(const vector<mozgflt,mozgint>&,
		     vector<mozgint,mozgint>*);
  template void rank(const vector<mozgint,mozgint>&,
		     vector<mozgint,mozgint>*);

// It randomizes vector
  template <class vec_t, class vec_size_t> 
    void randomize(vector<vec_t,vec_size_t>* v,
		   const vec_t min,
		   const vec_t max)
    {
      vec_t* temp     = v->vec();
      vec_size_t size = v->size();
      
      while (size--) *temp++ = random_number(min,max);
    }
  template void randomize(vector<mozgflt,mozgint>*,
			  const mozgflt,
			  const mozgflt);

///////////////////////////////////////////////////////////////////////////////

// zero matrix and limits
  template <class vec_t, class vec_size_t> 
    void zero (checked_vector<vec_t,vec_size_t>* v)
    {
      zero ((vector<vec_t,vec_size_t>*)v);
      v->putMaxAbs ((vec_t)0);
      v->putMinAbs ((vec_t)0);
    }
  template void zero (checked_vector<mozgflt,mozgint>*);
  template void zero (checked_vector<mozgint,mozgint>*);

}
