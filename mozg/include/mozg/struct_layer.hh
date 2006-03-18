// $Id: struct_layer.hh,v 1.1 2006/03/18 21:53:18 lightdruid Exp $
// struct_layer.hh
// mozgMLP layer struct
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

#ifndef _MOZG_MLP_LAYER_HH_
#  define _MOZG_MLP_LAYER_HH_

#include "mozg/vector_matrix.hh"
#include "mozg/matrix.hh"
#include "mozg/vector.hh"
#include "mozg/types.hh"


namespace mozg {

// network layer
  struct struct_layer
  {
    vector<mozgflt,mozgint>*         outputs;// neuron outputs
    vector<mozgflt,mozgint>*         nets;   // weighted inputs sum for non-RBF
    vector<mozgflt,mozgint>*         errors; // layer errors
    checked_vector<mozgflt,mozgint>* sigmas; // sigmas - Radial Basis Function
    vector<mozgflt,mozgint>*         sigmas_in_chunk;// sigmas in chunk
    
    checked_matrix<mozgflt,mozgint>* weights;// neuron weights
    matrix<mozgflt,mozgint>*         RBFinputs;// need for the backpropagate
                                             // in the case of the RBF output
                                             // function
    checked_matrix<mozgflt,mozgint>* weight_etas;// learning rates for Qprop
    checked_matrix<mozgflt,mozgint>* last_deltas;// neuron weights last deltas
    matrix<mozgflt,mozgint>*         deltas_in_chunk;// neuron weights last
                                             //  deltas for chunk
    matrix<mozgflt,mozgint>*         pderivs;// partial derivative of cost
                                             // function
    matrix<mozgflt,mozgint>*         prev_pderivs;// last partial derivative of
                                             // cost function
    matrix<mozgflt,mozgint>*         pderivs_in_chunk;// partial derivative of
                                             // cost function for chunk
    vector<mozgflt,mozgint>*         sigma_pderivs;// partial derivative of
                                             // cost function
    vector<mozgflt,mozgint>*         sigma_prev_pderivs;// last partial
                                             // derivative of cost function
    vector<mozgflt,mozgint>*         sigma_pderivs_in_chunk;// partial
                                             // derivative for chunk
    checked_vector<mozgflt,mozgint>* last_sigma_deltas;// sigmas last deltas
    checked_vector<mozgflt,mozgint>* sigma_etas;// learning rates for Qprop
    
    mozgint outputs_num;      // number of units in layer
    mozgint func_number;      // number of output function in layer
    bool    flag_RBF;         // flag of RBF layer
    
    OUT_FUNC lay_output_func; // output functions in layer

    OUT_FUNC_DERIV lay_output_func_deriv; // its derivative
    
    mozgflt eta;  // learning rate in layer
    mozgflt gain; // inverse temperature in layer's output function
    
  };

}


#endif // _MOZG_MLP_LAYER_HH_
