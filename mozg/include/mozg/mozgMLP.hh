// $Id: mozgMLP.hh,v 1.4 2006/03/25 20:12:53 lightdruid Exp $
// mozgMLP.hh
// Multi-Layer Perceptron (mozgMLP) simulator class
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

#ifndef _MOZG_MLP_HH_
#  define _MOZG_MLP_HH_

#include <math.h>
#include <assert.h>

#include "mozg/struct_layer.hh"
#include "mozg/vector_matrix.hh"
#include "mozg/matrix.hh"
#include "mozg/vector.hh"
#include "mozg/math.hh"
#include "mozg/limits.hh"
#include "mozg/types.hh"
#include "mozg/errorMessage.hh"


namespace mozg {

class mozgMLP {
public:

    // PLEASE DEFINE THE MEMBER-FUNCTION(if you use learnNet b) or c)).
    // The function (called reverse call function) is called by learnNet b),c)
    // which send it overall number of epoch since learning begin, learning error
    // and test error if it is calculated; if test error isn't calculated (for
    // learnNet b)) zero is put in second signature parameter of the function.
    // The function MUST RETURN true if network learning must be ended (error is
    // small enough) then return from learnNet() happens and false in the other
    // case.
    virtual bool answerMessage(mozgflt, mozgflt) {
        return true;
    }
 
    struct_layer* layer;          // pointer to the massive of network layers

    mozgint identificator;        // identificator of network
    mozgint switch_of_learn_rule; // number of learning rule by which network
    //   is ready to be learned

    mozgint switch_of_energy_function; // network's energy function
    // (called error function)
    mozgint layers_num;         // the number of layers (including input layer)
    mozgint overall_epnum;      // Overall number of epoch since learning begin
    mozgint nonstand_func_type; // number of nonstandart function range
    mozgint steps_num;          // number of steps for an example in second
    // order algorithm

    mozgint chunk;              // number of examples for chunk learning

    vector< mozgflt,mozgint>* diff;  // difference between desiring and
    // real output vectors, NOT FOR
    //  Cross-Entropy and log-square error

    mozgflt alpha;                   // momentum term coefficient
    mozgflt sigma_Langevin;          // Langevin Gauss begin sigma
    mozgflt sigma_Langevin_decrease; // Langevin Gauss sigma coef decrease
    mozgflt decay;                   // weight decay coefficient
    mozgflt rate_coeff;              // coeff of changing of rate (D=1.)
    mozgflt gain_coeff;              // coeff of changing of gain (D=1.)
    mozgflt gammaplus;               // constants in Qprop learning
    mozgflt gammaminus;              //   rule
    mozgflt begin_weight_etas;       // weight etas for Rprop in learning
    // start 

    mozgflt error_over_outputs;     // square of a output error vector module
    mozgflt error_threshold;        // learning or test error which can be
    // allowed to finish learning

    mozgflt nonstandfunc_min;   // maximum of nonstandart output function
    mozgflt nonstandfunc_max;   // its minimum, bouth value is need for
    // cross-entropy error function

    bool flag_of_bias_term;     // flag of bias term, public! (for << and >> )

    // create network
    mozgMLP(mozgint, //  number of layers (including input layer)
         mozgint*, //  number of units in layers (massive)
         mozgint*, //  output function numbers at layers (massive)
         mozgflt, //  sweep of weight randomization
         bool);          //  flag of bias term


    // delete network
    virtual ~mozgMLP();

    // put learning parameters for "vanilla" backpropagation rule
    void putBackprop(mozgflt, // (D=.2) common learning rate
              mozgint, // (D=0) mode of layers' rates calculation
              mozgflt, // (D=1.) temperature 
              mozgint, // (D=1) switch of energy function
              mozgint, // (D=1) the number of examples in chunk
              mozgint); // (D=1) number of iterations per learning
    // example

    // put parameters for rule with extensions 
    void putExtensions(mozgflt, // (D=.3) momentum term coefficient
            mozgflt, // (D=0.) begin Langevin dispersion
            mozgflt, // (D=0.) Langevin dispersion decreasing coef
            mozgflt); // (D=0.) weight decay coefficient

    // put learning parameter for Qprop
    void putQuickprop(mozgflt, // (D=.2) common learning rate
               mozgint, // (D=0) mode of layers' rates 
               mozgflt, // (D=1.) temperature
               mozgint, // (D=1) energy function switch
               mozgint, // (D=1) the number of examples in chunk
               mozgint); // (D=10) number of iterations
    //  per learning example

    // put learning parameters for Rprop
    void putRprop(mozgflt, // (D=1.2) constant in learning rule
           mozgflt, // (D=.5)  (the same)
           mozgflt, // (D=.2)  common learning rate
           mozgint, // (D=0)   mode of layers' rates calculation
           mozgflt, // (D=1.)  temperature
           mozgint); // (D=1)   switch of energy function

    // learn network
    void learnNet(mozgflt*, // learning input vector
           mozgflt*); // learning output vector

    // learn network
    void learnNet(mozgflt**, // learning input vector array
           mozgflt**, // learning output vector array
           mozgint, // learning set size
           mozgint, // order of vectors used in learning
           mozgint, // number of epochs network must be learned
           mozgint);  // answerMessage() is called for each pepnum-th 
    // epoch

    // learn and test network
    void learnNet(mozgflt**, // learning input vector array
           mozgflt**, // learning output vector array
           mozgint, // learning set size
           mozgint, // order of vectors used in learning
           mozgint, // number of epochs network must be learned
           mozgint, // answerMessage() is called for each pepnum-th
                      // epoch
           mozgflt**, // test input vector array
           mozgflt**, // test output vector array
           mozgint);  // test set size

    // test network, returns mean square error over test set
    mozgflt testNet(mozgflt**, // test input vector array
             mozgflt**, // test output vector array
             mozgint);  // test set size

    // propagate input vector forward through network to the outputs and save
    // outputs in the outvec
    void propInputs(mozgflt*, // inputs
             mozgflt*); // outputs

    // propagate vectors of the invecarr forward through network to the outputs
    // and save outputs for each vector in outvecarr
    void propInputs(mozgflt**, // array of input vectors
             mozgflt**, // array of output vectors
             mozgint);  // the number of input vector

    // initialise non-standart output function parameters (only if it on output
    // layer and cross-entropy is used). If function hasn't been called, error
    // message is printed (see before_exception.cc)
    void putNonStdFunction(mozgflt, // minimum of output function
                mozgflt); // maximum of output function


protected:

    vector< mozgflt,mozgint>* temp_vector;  // for Cross-Entropy function
    vector< mozgflt,mozgint>* temp_vector2; // the same & non-standard
    //  output function on output
    // layer

    // neuron output functions (see math.hh)
    static const OUT_FUNC output_func[7];

    // neuron output functions derivatives (see math.hh)
    static const OUT_FUNC_DERIV output_func_deriv[7];


    // links between dynamic memory consumers and memory_manager()
    vector< mozgint,mozgint>* real_memory;
    vector< mozgint,mozgint>* necessary_memory;
    vector< mozgint,mozgint>* net_memory;

    // allocates memory by request (necessary_memory) and makes garbage collection
    void memory_manager(mozgint); // 0 - usual call, 1 -  call destroying
    // network


    // put net_inputs to the net inputs
    void put_inputs(mozgflt*); // pointer to array of network inputs

    // propagate layer's inputs through the layer
    void compute_lay_outputs(const struct_layer&, // layer where signal
                                       // go from
                  struct_layer*);      // layer where signal go to

    // put last layer outputs to the net_outputs
    void get_outputs(mozgflt*) const; // pointer to array where put network
    // output to

    // compute errors for last layer
    void compute_output_errors(mozgflt*, // desirable outputs
                bool);    // compute_output_errors flag

    // compute neuron errors for the current layer
    void compute_lay_errors(const struct_layer&, // previous layer
                 const matrix< mozgflt,mozgint>&, // weights of
                                      // previous layer
                 struct_layer*);      // current layer

    // propagate errors from last layer to first one
    void backprop_errors();

    // update weights for current layer
    void adjust_lay_weights(const struct_layer&, // previous layer
                 struct_layer*);      //  current layer

    // adjust weights for current layer in chunk learning
    void adjust_chunk_lay_weights(const struct_layer&, // previous layer
                   struct_layer*);      //  current layer


    // optimised adjust_lay_weights (), without error backprop extentions
    void o_adjust_lay_weights(const struct_layer&, // previous layer
                   struct_layer*);      //  current layer

    // optimised adjust weights for current layer in chunk learning
    void o_adjust_chunk_lay_weights(const struct_layer&, // previous layer
                     struct_layer*);      //  current layer


    // compute partial derivative of error function for the current layer
    void compute_lay_pderivs(const struct_layer&, // previous layer
                  const struct_layer&, // next layer
                  const matrix< mozgflt,mozgint>&, // weights of
                                       // previous layer
                  struct_layer*);      //  current layer

    // compute partial derivative of error function
    void compute_partial_derivatives();

    // adjust netowrk's weights by backprop
    void adjust_weights_by_backprop();

    // adjust netowrk's weights by backprop
    void adjust_weights_by_quickprop(mozgint step_number);

    // adjust netowrk's weights by backprop
    void adjust_weights_by_rprop(mozgint step_number);

    // if is risen adjust_lay_weights() is called else o_adjust_lay_weights()
    bool flag_of_extensions;

    // are risen if first or last example in chunk is run
    bool flag_of_first_chunk_example;
    bool flag_of_last_chunk_example;

    // flags is risen if relevant function has been called yet
    bool is_call_answerMessage;     // answerMessage()
    bool is_call_putNonStdFunction; // putNonStdFunction()

    // inline for the sake of confidense:
    inline mozgflt signum(mozgflt number) {
        if (number > 0.)
            return 1.;
        else
            return (number) ? -1. : 0.;
    }
};

}


#endif // _MOZG_MLP_HH_




