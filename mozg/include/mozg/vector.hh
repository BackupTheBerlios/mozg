// $Id: vector.hh,v 1.1 2006/03/18 21:53:18 lightdruid Exp $
// vector.hh
// Kernel library, vector and checked_vector class templates
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

#ifndef _MOZG_VECTOR_HH_
#  define _MOZG_VECTOR_HH_

#include <math.h>
#include <assert.h>

#include "mozg/random.hh"
#include "mozg/types.hh"


namespace mozg {

  template <class vec_t, class vec_size_t>
    class vector {

    private:
      
      vec_t* p_vector;         // pointer to first vector element
      vec_size_t vector_size;  // vector size

    public:

      // return vector size
      vec_size_t size () const
      { return vector_size; }

// return vector pointer
      vec_t* vec () const
      { return p_vector; }

// return reference to vector element
      vec_t& operator [] (const vec_size_t number) const
      { return p_vector[number]; }

// add each element of second vector to relevant element of first one
      vector&operator |= (const vector& vector2);

// add the number to each element of the vector
      vector&operator += (vec_t number);

// add each element of second vector to relevant element of first one
      vector&operator += (const vector& vector2);

// add each element of second vector to relevant element of first one
      vector&operator += (const vec_t* vector2);

// subtruct each element of second vector to relevant element of first one
      vector&operator -= (const vector& vector2);

// multiply each element of vector by the number
      vector&operator *= (vec_t number);

// multiply each element of first vector by relevant element of second one
      vector&operator *= (const vector& vector2);

// multiply each element of first vector by relevant element of second one
      vector&operator *= (const vec_t* vector2);

// divide each vector element by the number
      vector& operator /= (vec_t number);

// divide each vector element by relevant denominator element
      vector& operator /= (const vector& denominator);

// divide each vector element by relevant denominator element
      vector& operator /= (const vec_t* denominator);

// create a vector
      vector(vec_size_t size)
      {
	vector_size = size;

	p_vector = new vec_t[size];
	assert(p_vector);
      }

// create the vector
      vector(vec_size_t size, mozgflt number)
      {
	mozgint stmp = size;

	vector_size = size;
	p_vector = new vec_t[size];
	assert (p_vector);

	mozgflt* tmp = p_vector;
	while (stmp--)
	  *tmp++ = number;
      }

// delete vector
      virtual ~vector()
      { delete [] p_vector; }
    };

///////////////////////////////////////////////////////////////////////////////

  template <class vec_t, class vec_size_t>
    class checked_vector : public vector<vec_t, vec_size_t> {

    private:

      vec_t max_abs;  // maximum of absolute value of vector element
      vec_t min_abs;  // minimum of absolute value of vector element

    public:

// put maximum of absolute value of vector element
      void putMaxAbs (vec_t maximum_abs)
      { max_abs = maximum_abs; }

// put minimum of absolute value of vector element
      void putMinAbs (vec_t minimum_abs)
      { min_abs = minimum_abs; }

// get maximum of absolute value of vector element
      vec_t getMaxAbs ()
      { return max_abs; }

// get minimum of absolute value of vector element
      vec_t getMinAbs ()
      { return min_abs; }

// check vector elements for going out of vector limits
// returns:
//     0 there're NO elements going out of maximum of absolute value
//     n there're n elements > max abs value
      vec_size_t isOutMaxAbs ();

//     0 there're NO elements going out of minimum of absolute value
//     n there're n elements < min abs value
      vec_size_t isOutMinAbs ();

// create checked vector
      checked_vector (const vec_size_t dim) : vector<vec_t, vec_size_t>(dim)
      {
	max_abs = 0;
	min_abs = 0;
      }

// delete checked vector
      virtual ~checked_vector () {}
    };

///////////////////////////////////////////////////////////////////////////////

// compare the number with each element of the vector
  template <class vec_t, class vec_size_t>
    bool operator != (const vector<vec_t,vec_size_t>& v,
		      const vec_t number);

// compare the number with each element of the vector
  template <class vec_t, class vec_size_t>
    bool operator == (const vector<vec_t,vec_size_t>& v,
		      const vec_t number);

// compare each element of *this with relevant one of the vector
  template <class vec_t, class vec_size_t>
    bool operator == (const vector<vec_t,vec_size_t>& v,
		      const vector<vec_t,vec_size_t>& v2);

// compare each element of *this with relevant one of the vector
  template <class vec_t, class vec_size_t>
    bool operator == (const vector<vec_t,vec_size_t>& v,
		      const vec_t* v2);

// It returns min vector's element
  template <class vec_t, class vec_size_t>
    vec_t  min (const vector<vec_t,vec_size_t>& v);

// It returns max vector's element
  template <class vec_t, class vec_size_t>
    vec_t  max (const vector<vec_t,vec_size_t>& v);

// It subtract vector2 from vector1
  template <class vec_t, class vec_size_t>
    void subtract (const vec_t* vector1,
		   const vector<vec_t,vec_size_t>& vector2,
		   vector<vec_t,vec_size_t>* result_vec);

// the same
  template <class vec_t, class vec_size_t>
    void subtract (const vector<vec_t,vec_size_t>& vector1,
		   const vec_t* vector2,
		   vector<vec_t,vec_size_t>* result_vec);

// the same
  template <class vec_t, class vec_size_t>
    void subtract (const vector<vec_t,vec_size_t>& vector1,
		   const vector<vec_t,vec_size_t>& vector2,
		   vector<vec_t,vec_size_t>* result_vec);

// It computes inner(scalar) vector product of vector1 and vector2
  template <class vec_t, class vec_size_t>
    void multiply (const vector<vec_t,vec_size_t>& vector1,
		   const vector<vec_t,vec_size_t>& vector2,
		   vec_t* product);

// It puts number to all copy elements
  template <class vec_t, class vec_size_t>
    void copy (const vec_t number,
	       vector<vec_t,vec_size_t>* copy);

// It copies orig_vector to copy
  template <class vec_t, class vec_size_t>
    void copy (const vec_t* orig_vector,
	       vector<vec_t,vec_size_t>* copy);

// It copies "orig" vector to copy
  template <class vec_t, class vec_size_t>
    void copy (const vector<vec_t,vec_size_t>& orig,
	       vector<vec_t,vec_size_t>* copy);

// It zeroes vector
  template <class vec_t, class vec_size_t>
    void zero (vector<vec_t,vec_size_t>* v);
  
// It sorts vector
  template <class vec_t, class vec_size_t>
    void sort (vector<vec_t,vec_size_t>* v);

// It puts indirrect addressing vector to rank_vector
  template <class vec_t, class vec_size_t>
    void rank (const vector<vec_t,vec_size_t>& v,
	       vector<vec_size_t,vec_size_t>* rank_vec);

// It randomizes vector by normal distribution with sigma and mean
  template <class vec_t, class vec_size_t>
    void randomize (vector<vec_t,vec_size_t>* v,
		    const vec_t min,
		    const vec_t max);

///////////////////////////////////////////////////////////////////////////////

// zero vector and limits
  template <class vec_t, class vec_size_t>
    void zero (checked_vector<vec_t,vec_size_t>*);


}

#endif // _MOZG_VECTOR_HH_
