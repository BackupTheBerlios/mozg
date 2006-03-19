// $Id: mozgMLP_2.cc,v 1.2 2006/03/19 20:46:17 lightdruid Exp $
// 
// mozgMLP_2.cc
// MultiLayer Perceptron (mozgMLP) simulator protected methods


#include "mozg/mozgMLP.hh"


namespace mozg {

// neuron output functions (see math.hh)
const OUT_FUNC mozgMLP::output_func[7] = {
    mozg_weinet, mozg_exp, mozg_th, mozg_lin, mozg_rbf, mozg_atg, mozg_rbfnet
};

// neuron output functions derivatives (see math.hh)
const OUT_FUNC_DERIV mozgMLP::output_func_deriv[7] = {
    mozg_D_weinet, mozg_D_exp, mozg_D_th, mozg_D_lin, mozg_D_rbf, mozg_D_atg,
    mozg_D_rbfnet
};


// put net_inputs to the net inputs
void mozgMLP::
put_inputs(mozgflt* net_inputs) {
    mozgflt* inputs = layer[0].outputs->vec();
    mozgint num_inputs;

    if (flag_of_bias_term) {
        num_inputs = layer[0].outputs_num - 1;
        inputs[num_inputs] = (mozgint) 1.;
    } else
        num_inputs = layer[0].outputs_num;

    for (mozgint i = 0; i < num_inputs; i++)
        *inputs++ = *net_inputs++;
}

// propagate layer's inputs through the layer
void mozgMLP::
compute_lay_outputs(const struct_layer& LOWER, struct_layer* UPPER) {
    if (!UPPER->flag_RBF)
        multiply(*UPPER->weights, *LOWER.outputs, UPPER->nets);
    else
        multiply(*UPPER->weights, *LOWER.outputs, UPPER->RBFinputs,
            UPPER->nets);

    mozgflt* nets = UPPER->nets->vec();
    mozgflt* outputs = UPPER->outputs->vec();
    mozgflt* sigmas = UPPER->sigmas->vec();
    mozgflt beta = UPPER->gain;
    mozgint i = UPPER->outputs->size();

    OUT_FUNC output_func = UPPER->lay_output_func;

    if (beta == 1.)
        while (i--) {
            *outputs++ = (*output_func) (*nets, * sigmas);
            nets++;
            sigmas++;
        }
    else
        while (i--) {
            *outputs++ = (*output_func) ((beta * *nets), * sigmas);
            nets++;
            sigmas++;
        }
}

// put last layer outputs to the net_outputs
void mozgMLP::
get_outputs(mozgflt* net_outputs) const {
    mozgflt* outputs = layer[layers_num - 1].outputs->vec();

    for (mozgint i = 0; i < layer[layers_num - 1].outputs_num; i++)
        *net_outputs++ = *outputs++;
}


// compute network output errors and place square of the error vector module in
// the error_over_outputs
void mozgMLP::
compute_output_errors(mozgflt* target, bool outerrflag) {
    struct_layer& lay = layer[layers_num - 1];

    subtract(*lay.outputs, target, diff);

    error_over_outputs = 0.;

    // place square of a error vector module in the error_over_outputs
    multiply(*diff, *diff, &error_over_outputs);

    if (outerrflag) {
        switch (switch_of_energy_function) {
        case 2:
            if (lay.flag_RBF || (nonstand_func_type == 1)) {
                copy(OVER_UNIT, temp_vector);
                subtract(*temp_vector, *lay.outputs, temp_vector);
                *temp_vector *= *lay.outputs;
                // remove division by zero:
                while (!(*temp_vector != (mozgflt) 0.))
                    *temp_vector += EPS;
                *diff /= *temp_vector;
            }
            if ((lay.func_number == 5) ||
                (lay.func_number == 2) ||
                (nonstand_func_type == 2)) {
                copy(*lay.outputs, temp_vector);
                *temp_vector *= *temp_vector;
                *temp_vector *= -HALF_UNIT;
                *temp_vector += HALF_UNIT;
                // remove division by zero:
                while (!(*temp_vector != (mozgflt) 0.))
                    *temp_vector += EPS;
                *diff /= *temp_vector;
            }
            if (nonstand_func_type == 3) {
                copy(target, diff);
                copy(*lay.outputs, temp_vector);
                *diff += -nonstandfunc_min;
                *temp_vector += -nonstandfunc_min;
                // remove division by zero:
                while (!(*temp_vector != (mozgflt) 0.))
                    *temp_vector += EPS;
                *diff /= *temp_vector;

                copy(target, temp_vector);
                copy(*lay.outputs, temp_vector2);
                *temp_vector += -nonstandfunc_max;
                *temp_vector2 += -nonstandfunc_max;
                // remove division by zero:
                while (!(*temp_vector2 != (mozgflt) 0.))
                    *temp_vector2 += EPS;
                *temp_vector /= *temp_vector2;
                *temp_vector *= -UNIT;

                *diff += *temp_vector;
            }break;

        case 3:
            // remove division by zero:
            if (error_over_outputs == 1)
                error_over_outputs -= EPS;
            *diff /= TWO * (UNIT - error_over_outputs);break;

            //    case 4: copy(target,temp_vector);     // temp_vector_i=t_i

            //      *temp_vector /= *lay.outputs;break; // temp_vector_i=t_i/o_i
        }

        mozgflt* lay_diff = diff->vec();
        mozgflt* nets = lay.nets->vec();
        mozgflt* outputs = lay.outputs->vec();
        mozgflt* errors = lay.errors->vec();
        mozgflt* sigmas = lay.sigmas->vec();
        mozgflt beta = lay.gain; // I've examined access time for
        //   member-data and auto-value, the last is less.
        OUT_FUNC_DERIV output_func_deriv = lay.lay_output_func_deriv;

        mozgint i = lay.outputs_num;

        if (beta == 1.)
            while (i--) {
                *errors++ = *lay_diff++ * (*output_func_deriv)
                    (*outputs, * nets, * sigmas);
                outputs++;
                nets++;
                sigmas++;
            }
        else
            while (i--) {
                *errors++ = *lay_diff++ * beta * (*output_func_deriv)
                    (*outputs, (beta * *nets), * sigmas);
                outputs++;
                nets++;
                sigmas++;
            }
    }
}


// compute neuron errors for the current layer
void mozgMLP::
compute_lay_errors(const struct_layer& prev_layer,
    const matrix< mozgflt,mozgint>& prevlay_weights, struct_layer* cur_layer) {
    //      vec_t delta = 0.0;
    mozgflt* cur_nets = cur_layer->nets->vec();
    mozgflt* cur_outputs = cur_layer->outputs->vec();
    mozgflt* cur_errors = cur_layer->errors->vec();
    mozgflt* cur_sigmas = cur_layer->sigmas->vec();
    mozgflt beta = cur_layer->gain;
    mozgint j = cur_layer->outputs_num;

    OUT_FUNC_DERIV output_func_deriv = cur_layer->lay_output_func_deriv;

    multiply(*prev_layer.errors, prevlay_weights, cur_layer->errors);

    // It multiplies current errors on the derivative output function
    // and gets "layer's delta errors" named layer's errors in the mozg
    if (beta == 1.)
        while (j--) {
            *cur_errors++ *= (*output_func_deriv)
                (*cur_outputs, * cur_nets, * cur_sigmas);
            cur_outputs++;
            cur_nets++;
            cur_sigmas++;
        }
    else
        while (j--) {
            *cur_errors++ *= beta * (*output_func_deriv)
                (*cur_outputs, (beta * *cur_nets), * cur_sigmas);
            cur_outputs++;
            cur_nets++;
            cur_sigmas++;
        }
}

// propagate out errors back through the network
void mozgMLP::
backprop_errors() {
    struct_layer* cur_layer;
    struct_layer* prev_layer;
    matrix< mozgflt,mozgint>* prev_weights;

    for (mozgint i = layers_num - 1; i > 1; i--) {
        prev_layer = &layer[i];
        cur_layer = &layer[i - 1];

        prev_weights = (prev_layer->flag_RBF) ?
            prev_layer->RBFinputs :
            prev_layer->weights;

        compute_lay_errors(*prev_layer, *prev_weights, cur_layer);
    }
}

// adjust weights for current layer
void mozgMLP::
adjust_lay_weights(const struct_layer& prev_layer, struct_layer* cur_layer) {
    mozgflt* lay_inputs;
    mozgflt* lay_nets;
    mozgflt* lay_outputs;
    mozgflt* lay_errors = 0;
    mozgflt eta;

    mozgflt* connections = 0;
    mozgflt* cur_sigmas = 0;
    mozgflt* last_delta = 0;
    mozgflt* last_sigma_delta = 0;
    mozgflt* lay_RBF_weights = 0;
    mozgflt weight_delta;
    mozgflt sigma_delta = 0;
    mozgflt RBF_sigma_min = 0;

    mozgint func_number;

    // last_deltas are need for Quickprop, Rprop, SCG
    bool flag_of_alpha = (bool) alpha || (switch_of_learn_rule != 1);
    bool flag_of_sigma_Langevin = (bool) sigma_Langevin;
    bool flag_of_decay = (bool) decay;
    bool flag_of_NO_RBF_sigma_min = true;
    bool flag_of_RBF = false;

    if (cur_layer->flag_RBF) {
        flag_of_RBF = true;
        cur_sigmas = cur_layer->sigmas->vec();
        lay_RBF_weights = *(cur_layer->RBFinputs->matr());
        RBF_sigma_min = cur_layer->sigmas->getMinAbs();
        flag_of_NO_RBF_sigma_min = !(bool) RBF_sigma_min;
    }
    lay_nets = cur_layer->nets->vec();
    eta = cur_layer->eta;
    lay_outputs = cur_layer->outputs->vec();
    lay_errors = cur_layer->errors->vec();
    func_number = cur_layer->func_number;


    for (mozgint j = 0; j < cur_layer->outputs_num; j++) {
        connections = (cur_layer->weights->matr())[j];
        lay_inputs = prev_layer.outputs->vec();

        if (cur_layer->flag_RBF)
            last_sigma_delta = cur_layer->last_sigma_deltas->vec();

        if (flag_of_alpha) {
            last_delta = (cur_layer->last_deltas->matr())[j];
            if (flag_of_RBF)
                last_sigma_delta = cur_layer->last_sigma_deltas->vec();
        }

        for (mozgint k = 0; k < prev_layer.outputs_num; k++) {
            if (!flag_of_RBF)
                weight_delta = -eta * *lay_errors * *lay_inputs++;
            else {
                weight_delta = eta * *lay_errors * *lay_RBF_weights++;
            }
            if (flag_of_sigma_Langevin)
                weight_delta += gauss(sigma_Langevin);

            if (flag_of_decay)
                weight_delta -= decay * *connections;

            if (flag_of_alpha) {
                weight_delta += alpha * *last_delta;
                *last_delta++ = weight_delta;
            }
            *connections++ += weight_delta;
        }
        if (flag_of_RBF &&
            (flag_of_NO_RBF_sigma_min || (*cur_sigmas > RBF_sigma_min))) {
            sigma_delta = (mozgflt) 0;

            if (flag_of_sigma_Langevin)
                sigma_delta += gauss(sigma_Langevin);

            if (flag_of_decay)
                sigma_delta -= decay * *cur_sigmas;

            if (flag_of_alpha) {
                sigma_delta += alpha * *last_sigma_delta;
                *last_sigma_delta++ = sigma_delta;
            }
            *cur_sigmas++ += -eta * *lay_errors * *lay_nets++ / (*cur_sigmas) +
                sigma_delta;
        }
        lay_outputs++;
        lay_errors++;
    }
}

// adjust weights for current layer in chunk learning
void mozgMLP::
adjust_chunk_lay_weights(const struct_layer& prev_layer,
    struct_layer* cur_layer) {
    mozgflt* lay_inputs;
    mozgflt* lay_nets;
    mozgflt* lay_outputs;
    mozgflt* lay_errors = 0;
    mozgflt eta;

    mozgflt* connections = 0;
    mozgflt* cur_sigmas = 0;
    mozgflt* cur_sigmas_in_chunk = 0;
    mozgflt* last_delta = 0;
    mozgflt* last_sigma_delta = 0;
    mozgflt* delta_in_chunk = 0;
    mozgflt* lay_RBF_weights = 0;
    mozgflt weight_delta;
    mozgflt sigma_delta = 0;
    mozgflt RBF_sigma_min = 0;
    mozgflt max_abs_weight = 0;
    mozgflt min_abs_weight = 0;
    mozgflt max_abs_sigma = 0;
    mozgflt min_abs_sigma = 0;

    mozgint func_number;

    // chudo v per'iah :) (in russian)
    bool flag_of_alpha = (bool) alpha;
    bool flag_of_sigma_Langevin = (bool) sigma_Langevin;
    bool flag_of_decay = (bool) decay;
    bool flag_of_NO_RBF_sigma_min = true;
    bool flag_of_RBF = cur_layer->flag_RBF;

    if (flag_of_RBF) {
        cur_sigmas = cur_layer->sigmas->vec();
        lay_RBF_weights = *(cur_layer->RBFinputs->matr());
        cur_sigmas_in_chunk = cur_layer->sigmas_in_chunk->vec();
        RBF_sigma_min = cur_layer->sigmas->getMinAbs();
        flag_of_NO_RBF_sigma_min = !(bool) RBF_sigma_min;
    }
    lay_nets = cur_layer->nets->vec();
    eta = cur_layer->eta / chunk;
    lay_outputs = cur_layer->outputs->vec();
    lay_errors = cur_layer->errors->vec();
    func_number = cur_layer->func_number;


    for (mozgint j = 0; j < cur_layer->outputs_num; j++) {
        if (flag_of_last_chunk_example) {
            connections = (cur_layer->weights->matr())[j];

            max_abs_weight = cur_layer->weights->getMaxAbs();
            min_abs_weight = cur_layer->weights->getMinAbs();

            if (flag_of_RBF) {
                last_sigma_delta = cur_layer->last_sigma_deltas->vec();

                max_abs_sigma = cur_layer->sigmas->getMaxAbs();
                min_abs_sigma = cur_layer->sigmas->getMinAbs();
            }
            if (flag_of_alpha)
                last_delta = (cur_layer->last_deltas->matr())[j];
        }
        delta_in_chunk = (cur_layer->deltas_in_chunk->matr())[j];
        lay_inputs = prev_layer.outputs->vec();


        for (mozgint k = 0; k < prev_layer.outputs_num; k++) {
            if (!flag_of_RBF)
                weight_delta = -eta * *lay_errors * *lay_inputs++;
            else
                weight_delta = eta * *lay_errors * *lay_RBF_weights++;

            *delta_in_chunk += weight_delta;
            // for last chunk example weight are adjusted:
            if (flag_of_last_chunk_example &&
                fabs(*connections) > min_abs_weight &&
                fabs(*connections) < max_abs_weight) {
                weight_delta = *delta_in_chunk;

                if (flag_of_sigma_Langevin)
                    weight_delta += gauss(sigma_Langevin);

                if (flag_of_decay)
                    weight_delta -= decay * *connections;

                if (flag_of_alpha) {
                    weight_delta += alpha * *last_delta;
                    *last_delta++ = weight_delta;
                }
                *connections++ += weight_delta;
            }
            delta_in_chunk++;
        }
        if (flag_of_RBF
            // first flag_of_NO_RBF_sigma_min is checked, so if it has been risen then
            // comparing (*cur_sigmas > RBF_sigma_min) is not performed, so we get some
            // acceleration:
            &&
            (flag_of_NO_RBF_sigma_min || (*cur_sigmas > RBF_sigma_min))) {
            *cur_sigmas_in_chunk += -eta * *lay_errors * *lay_nets++ / (*cur_sigmas);

            if (flag_of_last_chunk_example &&
                fabs(*cur_sigmas) > min_abs_sigma &&
                fabs(*cur_sigmas) < max_abs_sigma) {
                sigma_delta = *cur_sigmas_in_chunk;

                if (flag_of_sigma_Langevin)
                    sigma_delta += gauss(sigma_Langevin);

                if (flag_of_decay)
                    sigma_delta -= decay * *cur_sigmas;

                if (flag_of_alpha) {
                    sigma_delta += alpha * *last_sigma_delta;
                    *last_sigma_delta++ = sigma_delta;
                }
                *cur_sigmas += sigma_delta;
            }
            cur_sigmas++;
            cur_sigmas_in_chunk++;
        }
        lay_outputs++;
        lay_errors++;
    }
}

// adjust weights for current layer by Backprop without extensions
// (optimesed CPU time)
void mozgMLP::
o_adjust_lay_weights(const struct_layer& prev_layer, struct_layer* cur_layer) {
    mozgflt* lay_inputs;
    mozgflt* lay_nets;
    mozgflt* lay_outputs;
    mozgflt* lay_errors;
    mozgflt eta;
    mozgflt* lay_RBF_weights = 0;

    mozgflt* connections;
    mozgflt* cur_sigmas = 0;
    mozgflt weight_delta;
    mozgflt RBF_sigma_min = 0;

    mozgint func_number;

    // chudo v per'iah :) (in russian)
    bool flag_of_NO_RBF_sigma_min = true;
    bool flag_of_RBF = cur_layer->flag_RBF;

    lay_nets = cur_layer->nets->vec();
    eta = cur_layer->eta;
    lay_outputs = cur_layer->outputs->vec();
    lay_errors = cur_layer->errors->vec();
    func_number = cur_layer->func_number;

    if (flag_of_RBF) {
        cur_sigmas = cur_layer->sigmas->vec();
        lay_RBF_weights = *(cur_layer->RBFinputs->matr());
        RBF_sigma_min = cur_layer->sigmas->getMinAbs();
        flag_of_NO_RBF_sigma_min = !(bool) RBF_sigma_min;
    }
    for (mozgint j = 0; j < cur_layer->outputs_num; j++) {
        connections = (cur_layer->weights->matr())[j];
        lay_inputs = prev_layer.outputs->vec();

        for (mozgint k = 0; k < prev_layer.outputs_num; k++) {
            if (!flag_of_RBF)
                weight_delta = -eta * *lay_errors * *lay_inputs++;
            else {
                weight_delta = eta * *lay_errors * *lay_RBF_weights++;
            }
            *connections++ += weight_delta;
        }

        if (flag_of_RBF
            // first flag_of_NO_RBF_sigma_min is checked, so if it has been risen then
            // comparing (*cur_sigmas > RBF_sigma_min) is not performed, so we get some
            // acceleration:
            &&
            (flag_of_NO_RBF_sigma_min || (*cur_sigmas > RBF_sigma_min))) {
            *cur_sigmas += -eta * *lay_errors * *lay_nets / (*cur_sigmas);
            cur_sigmas++;
            lay_nets++;
        }
        lay_outputs++;
        lay_errors++;
    }
}
// adjust weights for current layer in chunk learning
void mozgMLP::
o_adjust_chunk_lay_weights(const struct_layer& prev_layer,
    struct_layer* cur_layer) {
    mozgflt* lay_inputs;
    mozgflt* lay_nets;
    mozgflt* lay_outputs;
    mozgflt* lay_errors = 0;
    mozgflt eta;

    mozgflt* connections = 0;
    mozgflt* cur_sigmas = 0;
    mozgflt* cur_sigmas_in_chunk = 0;
    mozgflt* delta_in_chunk = 0;
    mozgflt* lay_RBF_weights = 0;
    mozgflt weight_delta;
    mozgflt RBF_sigma_min = 0;

    mozgflt max_abs_weight = 0;
    mozgflt min_abs_weight = 0;
    mozgflt max_abs_sigma = 0;
    mozgflt min_abs_sigma = 0;

    mozgint func_number;

    // chudo v per'iah :) (in russian)
    bool flag_of_NO_RBF_sigma_min = true;
    bool flag_of_RBF = cur_layer->flag_RBF;

    if (flag_of_RBF) {
        cur_sigmas = cur_layer->sigmas->vec();
        lay_RBF_weights = *(cur_layer->RBFinputs->matr());
        cur_sigmas_in_chunk = cur_layer->sigmas_in_chunk->vec();
        RBF_sigma_min = cur_layer->sigmas->getMinAbs();
        flag_of_NO_RBF_sigma_min = !(bool) RBF_sigma_min;
    }
    lay_nets = cur_layer->nets->vec();
    eta = cur_layer->eta / chunk;
    lay_outputs = cur_layer->outputs->vec();
    lay_errors = cur_layer->errors->vec();
    func_number = cur_layer->func_number;

    for (mozgint j = 0; j < cur_layer->outputs_num; j++) {
        if (flag_of_last_chunk_example) {
            connections = (cur_layer->weights->matr())[j];

            max_abs_weight = cur_layer->weights->getMaxAbs();
            min_abs_weight = cur_layer->weights->getMinAbs();

            if (flag_of_RBF) {
                max_abs_sigma = cur_layer->sigmas->getMaxAbs();
                min_abs_sigma = cur_layer->sigmas->getMinAbs();
            }
        }
        delta_in_chunk = (cur_layer->deltas_in_chunk->matr())[j];
        lay_inputs = prev_layer.outputs->vec();

        for (mozgint k = 0; k < prev_layer.outputs_num; k++) {
            if (!flag_of_RBF)
                weight_delta = -eta * *lay_errors * *lay_inputs++;
            else
                weight_delta = eta * *lay_errors * *lay_RBF_weights++;

            *delta_in_chunk += weight_delta;

            if (flag_of_last_chunk_example &&
                fabs(*connections) > min_abs_weight &&
                fabs(*connections) < max_abs_weight)
                *connections++ += *delta_in_chunk;

            delta_in_chunk++;
        }
        if (flag_of_RBF
            // first flag_of_NO_RBF_sigma_min is checked, so if it has been risen then
            // comparing (*cur_sigmas > RBF_sigma_min) is not performed, so we get some
            // acceleration:
            &&
            (flag_of_NO_RBF_sigma_min || (*cur_sigmas > RBF_sigma_min))) {
            *cur_sigmas_in_chunk += -eta * *lay_errors * *lay_nets++ / (*cur_sigmas);

            if (flag_of_last_chunk_example &&
                fabs(*cur_sigmas) > min_abs_sigma &&
                fabs(*cur_sigmas) < max_abs_sigma)
                *cur_sigmas += *cur_sigmas_in_chunk;

            cur_sigmas++;
            cur_sigmas_in_chunk++;
        }
        lay_outputs++;
        lay_errors++;
    }
}

// compute partial derivative of error function for the current layer
void mozgMLP::
compute_lay_pderivs(const struct_layer& prev_layer,
    const struct_layer& next_layer,
    const matrix< mozgflt,mozgint>& prevlay_weights, struct_layer* cur_layer) {
    bool flag_RBF = cur_layer->flag_RBF;

    //non-chunk updating:
    if (chunk == 1) {
        //   replace copying by pointers exchanging
        matrix< mozgflt,mozgint>* tmp = cur_layer->prev_pderivs;
        cur_layer->prev_pderivs = cur_layer->pderivs;
        cur_layer->pderivs = tmp;
    }
    if (chunk == 1) {
        multiply(*next_layer.outputs, *cur_layer->errors, cur_layer->pderivs);

        if (flag_RBF) {
            //   replace copying by pointers exchanging
            vector< mozgflt,mozgint>* tmp1 = cur_layer->sigma_prev_pderivs;
            cur_layer->sigma_prev_pderivs = cur_layer->sigma_pderivs;
            cur_layer->sigma_pderivs = tmp1;

            copy(*cur_layer->errors, cur_layer->sigma_pderivs);
            *cur_layer->sigma_pderivs *= *cur_layer->nets;
            //   remove devision by zero:
            while (!(*cur_layer->sigma_pderivs != (mozgflt) 0.))
                *cur_layer->sigma_pderivs += EPS;
            *cur_layer->sigma_pderivs /= *cur_layer->sigmas;
        }
    }
    //chunk updating:
    else {
        if (flag_of_first_chunk_example) {
            //   replace copying by pointers exchanging
            matrix< mozgflt,mozgint>* tmp = cur_layer->prev_pderivs;
            cur_layer->prev_pderivs = cur_layer->pderivs;
            cur_layer->pderivs = tmp;

            zero(cur_layer->pderivs);

            if (flag_RBF) {
                vector< mozgflt,mozgint>* tmp1 = cur_layer->sigma_prev_pderivs;
                cur_layer->sigma_prev_pderivs = cur_layer->sigma_pderivs;
                cur_layer->sigma_pderivs = tmp1;

                zero(cur_layer->sigma_pderivs);
            }
        }
        multiply(*next_layer.outputs, *cur_layer->errors,
            cur_layer->pderivs_in_chunk);

        *cur_layer->pderivs += *cur_layer->pderivs_in_chunk;

        if (flag_RBF) {
            copy(*cur_layer->errors, cur_layer->sigma_pderivs_in_chunk);
            *cur_layer->sigma_pderivs_in_chunk *= *cur_layer->nets;
            //   remove devision by zero:
            while (!(*cur_layer->sigma_pderivs_in_chunk != (mozgflt) 0.))
                *cur_layer->sigma_pderivs_in_chunk += EPS;

            *cur_layer->sigma_pderivs_in_chunk /= *cur_layer->sigmas;
            *cur_layer->sigma_pderivs += *cur_layer->sigma_pderivs_in_chunk;
        }
    }
}


// compute partial derivative of error function
void mozgMLP::
compute_partial_derivatives() {
    struct_layer& output_layer = layer[layers_num - 1];
    struct_layer* cur_layer;
    struct_layer* prev_layer;
    matrix< mozgflt,mozgint>* prev_weights;

    bool output_flag_RBF = output_layer.flag_RBF;

    // compute pderivs for output layer for non-chunk
    if (chunk == 1) {
        // replace copying by pointer exchanging
        matrix< mozgflt,mozgint>* tmp = output_layer.prev_pderivs;
        output_layer.prev_pderivs = output_layer.pderivs;
        output_layer.pderivs = tmp;

        if (output_flag_RBF) {
            vector< mozgflt,mozgint>* tmp1 = output_layer.sigma_prev_pderivs;
            output_layer.sigma_prev_pderivs = output_layer.sigma_pderivs;
            output_layer.sigma_pderivs = tmp1;
        }
        multiply(*layer[layers_num - 2].outputs, *output_layer.errors,
            output_layer.pderivs);

        if (output_flag_RBF) {
            copy(*output_layer.errors, output_layer.sigma_pderivs);
            *output_layer.sigma_pderivs *= *layer[layers_num - 1].nets;
            // remove devision by zero:
            while (!(*output_layer.sigmas != (mozgflt) 0.))
                *output_layer.sigmas += EPS;
            *output_layer.sigma_pderivs /= *output_layer.sigmas;
        }
    }
    // compute pderivs for output layer for chunk
    else {
        if (flag_of_first_chunk_example) {
            // replace copying by pointer exchanging
            matrix< mozgflt,mozgint>* tmp = output_layer.prev_pderivs;
            output_layer.prev_pderivs = output_layer.pderivs;
            output_layer.pderivs = tmp;

            zero(output_layer.pderivs);
        }
        multiply(*layer[layers_num - 2].outputs, *output_layer.errors,
            output_layer.pderivs_in_chunk);

        *output_layer.pderivs += *output_layer.pderivs_in_chunk;

        if (output_flag_RBF) {
            if (flag_of_first_chunk_example) {
                // replace copying by pointer exchanging
                vector< mozgflt,mozgint>* tmp1 = output_layer.sigma_prev_pderivs;
                output_layer.sigma_prev_pderivs = output_layer.sigma_pderivs;
                output_layer.sigma_pderivs = tmp1;

                zero(output_layer.sigma_pderivs);
            }
            copy(*output_layer.errors, output_layer.sigma_pderivs_in_chunk);

            *output_layer.sigma_pderivs_in_chunk *= *layer[layers_num - 1].nets;

            // remove devision by zero:
            while (!(*output_layer.sigmas != (mozgflt) 0.))
                *output_layer.sigmas += EPS;

            *output_layer.sigma_pderivs_in_chunk /= *output_layer.sigmas;

            if (flag_of_last_chunk_example)
                copy(*output_layer.sigma_pderivs_in_chunk,
                    output_layer.sigma_pderivs);
        }
    }
    for (mozgint i = layers_num - 1; i > 1; i--) {
        prev_layer = &layer[i];
        cur_layer = &layer[i - 1];

        prev_weights = (prev_layer->flag_RBF) ?
            prev_layer->RBFinputs :
            prev_layer->weights;

        compute_lay_pderivs(*prev_layer, layer[i - 2], *prev_weights,
            cur_layer);
    }
}

// adjust network weights by backprop
void mozgMLP::
adjust_weights_by_backprop() {
    struct_layer* cur_layer;
    struct_layer* prev_layer;

    for (mozgint i = 1; i < layers_num; i++) {
        prev_layer = &layer[i - 1];
        cur_layer = &layer[i];

        if (chunk > 1)
            if (flag_of_extensions)
                adjust_chunk_lay_weights(*prev_layer, cur_layer);
            else
                o_adjust_chunk_lay_weights(*prev_layer, cur_layer);
        else {
            if (flag_of_extensions)
                adjust_lay_weights(*prev_layer, cur_layer);
            else
                o_adjust_lay_weights(*prev_layer, cur_layer);
        }
    }
    if (sigma_Langevin)
        sigma_Langevin *= sigma_Langevin_decrease;
}


// adjust network's weights by Quickprop
void mozgMLP::
adjust_weights_by_quickprop(mozgint step_num) {
    struct_layer* lay;
    mozgflt* weight;
    mozgflt* sigma = 0;
    mozgflt* last_delta = 0;
    mozgflt* last_sigma_delta = 0;
    mozgflt* pderiv;
    mozgflt* prev_pderiv;
    mozgflt* sigma_pderiv = 0;
    mozgflt* sigma_prev_pderiv = 0;
    mozgflt eta;
    mozgflt weight_delta;
    mozgflt sigma_delta;
    mozgflt second_order_term = 0;
    mozgint j;
    mozgint k;
    mozgflt tmp;
    mozgflt max_abs_weight;
    mozgflt min_abs_weight;
    mozgflt max_abs_sigma = 0;
    mozgflt min_abs_sigma = 0;

    bool flag_of_alpha = (bool) alpha;
    bool flag_of_sigma_Langevin = (bool) sigma_Langevin;
    bool flag_of_decay = (bool) decay;
    bool flag_of_RBF = false;

    for (mozgint i = 1; i < layers_num; i++) {
        lay = &layer[i];
        weight = lay->weights->matr()[0];
        pderiv = lay->pderivs->matr()[0];
        prev_pderiv = lay->prev_pderivs->matr()[0];
        flag_of_RBF = false;

        if (lay->flag_RBF) {
            flag_of_RBF = true;
            sigma = lay->sigmas->vec();
            sigma_pderiv = lay->sigma_pderivs->vec();
            sigma_prev_pderiv = lay->sigma_prev_pderivs->vec();
        }
        j = lay->outputs_num * layer[i - 1].outputs_num;
        k = lay->outputs_num;
        eta = lay->eta;
        max_abs_weight = lay->weights->getMaxAbs();
        min_abs_weight = lay->weights->getMinAbs();
        if (flag_of_RBF) {
            max_abs_sigma = lay->sigmas->getMaxAbs();
            min_abs_sigma = lay->sigmas->getMinAbs();
        }
        // not use flag_of_extensions !!!:
        if ((*real_memory)[9] == 1)
            last_delta = lay->last_deltas->matr()[0];

        if (lay->flag_RBF && (*real_memory)[17] == 1)
            last_sigma_delta = lay->last_sigma_deltas->vec();

        //    optimized adjust:
        if (!flag_of_extensions)
            while (j--) {
                if ((tmp = *prev_pderiv - *pderiv) &&
                    fabs(*weight) < max_abs_weight &&
                    fabs(*weight) > min_abs_weight) {
                    second_order_term = *pderiv / tmp * *last_delta;

                    // make cut of second order term:
                    if (fabs(second_order_term) > CUT_QUICKPROP)
                        second_order_term = signum(second_order_term) * CUT_QUICKPROP;

                    *last_delta = -(eta * (((*pderiv * *prev_pderiv) > 0) ?
                        1. :
                        0.) * *pderiv) + second_order_term;

                    *weight += *last_delta;
                }
                weight++;
                last_delta++;
                pderiv++;
                prev_pderiv++;
            }
        //    adjust with extensions:
        else
            while (j--) {
                if ((tmp = *prev_pderiv - *pderiv) &&
                    fabs(*weight) < max_abs_weight &&
                    fabs(*weight) > min_abs_weight) {
                    weight_delta = (mozgflt) 0.;
                    if (flag_of_sigma_Langevin)
                        weight_delta += gauss(sigma_Langevin);

                    if (flag_of_decay)
                        weight_delta -= decay * *weight;

                    if (flag_of_alpha)
                        weight_delta += alpha * *last_delta;

                    second_order_term = *pderiv / tmp * *last_delta;

                    // make cut of second order term:
                    if (fabs(second_order_term) > CUT_QUICKPROP)
                        second_order_term = signum(second_order_term) * CUT_QUICKPROP;

                    *last_delta = -(eta * (((*pderiv * *prev_pderiv) > 0) ?
                        1. :
                        0.) * *pderiv) + second_order_term +
                        weight_delta;
                }
                weight++;
                last_delta++;
                pderiv++;
                prev_pderiv++;
            }
        //    if it's RBF layer:
        if (flag_of_RBF) {
            //         with extensions:
            if (flag_of_extensions)
                while (k--) {
                    if ((tmp = *sigma_prev_pderiv - *sigma_pderiv) &&
                        fabs(*sigma) < max_abs_sigma &&
                        fabs(*sigma) > min_abs_sigma) {
                        sigma_delta = (mozgflt) 0.;
                        if (flag_of_sigma_Langevin)
                            sigma_delta += gauss(sigma_Langevin);

                        if (flag_of_decay)
                            sigma_delta -= decay * *weight;

                        if (flag_of_alpha)
                            sigma_delta += alpha * *last_sigma_delta;

                        second_order_term = *sigma_pderiv /
                            tmp * *last_sigma_delta;

                        // make cut of second order term:
                        if (fabs(second_order_term) > CUT_QUICKPROP)
                            second_order_term = signum(second_order_term) * CUT_QUICKPROP;

                        *last_sigma_delta = -eta * *sigma_pderiv * (((*sigma_pderiv * *sigma_prev_pderiv) >
                            0) ?
                            1. :
                            0.) + second_order_term +
                            sigma_delta;

                        *sigma += *last_sigma_delta;
                    }
                    sigma++;
                    last_sigma_delta++;
                    sigma_pderiv++;
                    sigma_prev_pderiv++;
                }
            //         optimized:
            else
                while (k--) {
                    if ((tmp = *sigma_prev_pderiv - *sigma_pderiv) &&
                        fabs(*sigma <
                            max_abs_sigma) &&
                        fabs(*sigma >
                            min_abs_sigma)) {
                        second_order_term = *sigma_pderiv /
                            tmp * *last_sigma_delta;

                        // make cut of second order term:
                        if (fabs(second_order_term) > CUT_QUICKPROP)
                            second_order_term = signum(second_order_term) * CUT_QUICKPROP;

                        *last_sigma_delta = -eta * *sigma_pderiv * (((*sigma_pderiv * *sigma_prev_pderiv) >
                            0) ?
                            1. :
                            0.) + second_order_term;

                        *sigma += *last_sigma_delta;
                    }
                    sigma++;
                    last_sigma_delta++;
                    sigma_pderiv++;
                    sigma_prev_pderiv++;
                }
        }
    }
}

// adjust network's weights by Rprop
void mozgMLP::
adjust_weights_by_rprop(mozgint step_num) {
    struct_layer* lay;
    mozgflt* weight;
    mozgflt* sigma = 0;
    mozgflt* last_delta = 0;
    mozgflt* last_sigma_delta = 0;
    mozgflt* pderiv;
    mozgflt* prev_pderiv;
    mozgflt* sigma_pderiv = 0;
    mozgflt* sigma_prev_pderiv = 0;
    mozgflt eta;
    mozgflt* weight_eta = 0;
    mozgflt* sigma_eta = 0;
    mozgflt weight_delta;
    mozgflt sigma_delta;
    mozgint j;
    mozgint k;
    mozgflt max_abs_weight;
    mozgflt min_abs_weight;
    mozgflt max_abs_sigma = 0;
    mozgflt min_abs_sigma = 0;
    mozgflt max_abs_weight_etas = 0;
    mozgflt min_abs_weight_etas = 0;
    mozgflt max_abs_sigma_etas = 0;
    mozgflt min_abs_sigma_etas = 0;

    bool flag_of_alpha = (bool) alpha;
    bool flag_of_sigma_Langevin = (bool) sigma_Langevin;
    bool flag_of_decay = (bool) decay;
    bool flag_of_RBF = false;

    for (mozgint i = 1; i < layers_num; i++) {
        lay = &layer[i];
        weight = lay->weights->matr()[0];
        pderiv = lay->pderivs->matr()[0];
        prev_pderiv = lay->prev_pderivs->matr()[0];
        flag_of_RBF = false;

        if (lay->flag_RBF) {
            flag_of_RBF = true;
            sigma = lay->sigmas->vec();
            sigma_pderiv = lay->sigma_pderivs->vec();
            sigma_prev_pderiv = lay->sigma_prev_pderivs->vec();
        }
        j = lay->outputs_num * layer[i - 1].outputs_num;
        k = lay->outputs_num;
        // because the number of examples equals to chunk:
        eta = lay->eta / chunk;
        max_abs_weight = lay->weights->getMaxAbs();
        min_abs_weight = lay->weights->getMinAbs();
        if (flag_of_RBF) {
            max_abs_sigma = lay->sigmas->getMaxAbs();
            min_abs_sigma = lay->sigmas->getMinAbs();
        }
        if (switch_of_learn_rule == 3) {
            max_abs_weight_etas = lay->weight_etas->getMaxAbs();
            min_abs_weight_etas = lay->weight_etas->getMinAbs();

            if (flag_of_RBF) {
                max_abs_sigma_etas = lay->sigma_etas->getMaxAbs();
                min_abs_sigma_etas = lay->sigma_etas->getMinAbs();
            }
            weight_eta = lay->weight_etas->matr()[0];
            if (flag_of_RBF)
                sigma_eta = lay->sigma_etas->vec();
        }
        // not flag_of_extensions !!!:
        if ((*real_memory)[9] == 1)
            last_delta = lay->last_deltas->matr()[0];

        if (lay->flag_RBF && (*real_memory)[17] == 1)
            last_sigma_delta = lay->last_sigma_deltas->vec();

        //    optimized adjust:
        if (!flag_of_extensions)
            while (j--) {
                if (fabs(*weight_eta) < max_abs_weight_etas &&
                    fabs(*weight_eta) > min_abs_weight_etas) {
                    *weight_eta *= ((*pderiv * *prev_pderiv) > 0.) ?
                        gammaplus :
                        gammaminus;
                    if (fabs(*weight) < max_abs_weight &&
                        fabs(*weight) > min_abs_weight)
                        *weight += -*weight_eta * signum(*pderiv);
                }
                weight++;
                weight_eta++;
                pderiv++;
                prev_pderiv++;
            }
        //    adjust with extensions:
        else
            while (j--) {
                if (fabs(*weight_eta) < max_abs_weight_etas &&
                    fabs(*weight_eta) > min_abs_weight_etas) {
                    *weight_eta *= ((*pderiv * *prev_pderiv) > 0.) ?
                        gammaplus :
                        gammaminus;
                    weight_delta = (mozgflt) 0.;
                    if (flag_of_sigma_Langevin)
                        weight_delta += gauss(sigma_Langevin);

                    if (flag_of_decay)
                        weight_delta -= decay * *weight;

                    if (flag_of_alpha) {
                        weight_delta += alpha * *last_delta;
                    }
                    if (*weight < max_abs_weight && *weight> min_abs_weight)
                        *weight += -*weight_eta * signum(*pderiv);
                }
                weight++;
                weight_eta++;
                pderiv++;
                prev_pderiv++;
            }
        //    if it's RBF layer:
        if (flag_of_RBF)
            //       optimized adjust:
            if (!flag_of_extensions)
                while (k--) {
                    if (fabs(*sigma_eta) < max_abs_sigma_etas &&
                        fabs(*sigma_eta) > min_abs_sigma_etas) {
                        *sigma_eta *= ((*sigma_pderiv * *sigma_prev_pderiv) >
                            0.) ?
                            gammaplus :
                            gammaminus;
                        if (*sigma < max_abs_sigma && *sigma> min_abs_sigma)
                            *sigma -= *sigma_eta * signum(*sigma_pderiv);
                    }
                    sigma++;
                    sigma_eta++;
                    sigma_pderiv++;
                    sigma_prev_pderiv++;
                }
            //       adjust with extensions:
            else
                while (k--) {
                    if (fabs(*sigma_eta) < max_abs_sigma_etas &&
                        fabs(*sigma_eta) > min_abs_sigma_etas) {
                        *sigma_eta *= ((*sigma_pderiv * *sigma_prev_pderiv) >
                            0.) ?
                            gammaplus :
                            gammaminus;
                        sigma_delta = (mozgflt) 0.;
                        if (flag_of_sigma_Langevin)
                            sigma_delta += gauss(sigma_Langevin);

                        if (flag_of_decay)
                            sigma_delta -= decay * *sigma;

                        if (flag_of_alpha) {
                            sigma_delta += alpha * *last_sigma_delta;
                        }
                        if (fabs(*sigma) < max_abs_sigma &&
                            fabs(*sigma) > min_abs_sigma)
                            *sigma -= *sigma_eta * signum(*sigma_pderiv);
                    }
                    sigma++;
                    sigma_eta++;
                    sigma_pderiv++;
                    sigma_prev_pderiv++;
                }
    }
}

}
