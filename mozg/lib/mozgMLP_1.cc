// $Id: mozgMLP_1.cc,v 1.5 2006/03/25 20:47:40 lightdruid Exp $
// 
// mozgMLP_1.cc
// MultiLayer Perceptron (mozgMLP) simulator public methods
// (interface functions)

#include "mozg/mozgMLP.hh"


namespace mozg {

// Create network, use "vanilla" Backprop in learning. If RBF isn't used,
// RBF_sigma_min doesn't influence on computations
mozgMLP::
mozgMLP(mozgint number_of_layers, mozgint* lay_ouputs_num,
    mozgint* func_num, mozgflt sweep, bool bias_term_flag) {

    struct_layer* lay;    

    layers_num = number_of_layers;
    if (layers_num < 2)
        sendErrorMessage("Error in mozgMLP: the number of layer<2, so the "
                         "program will be aborted.",
            1);

    is_call_answerMessage = false;
    is_call_putNonStdFunction = false;
    flag_of_bias_term = bias_term_flag;

    switch_of_learn_rule = 1;
    identificator = 0;
    overall_epnum = 1;

    // allocation array of struct layer
    real_memory = 0;
    necessary_memory = 0;
    memory_manager((mozgint) 0);

    for (int j = 1; j < layers_num; j++) {
        lay = &layer[j];
        lay->outputs = 0;
        lay->nets = 0;
        lay->errors = 0;
        lay->sigmas = 0;
        lay->sigmas_in_chunk = 0;
        lay->weights = 0;
        lay->RBFinputs = 0;
        lay->weight_etas = 0;
        lay->last_deltas = 0;
        lay->deltas_in_chunk = 0;
        lay->pderivs = 0;
        lay->prev_pderivs = 0;
        lay->pderivs_in_chunk = 0;
        lay->sigma_pderivs = 0;
        lay->sigma_prev_pderivs = 0;
        lay->sigma_pderivs_in_chunk = 0;
        lay->last_sigma_deltas = 0;
        lay->sigma_etas = 0;
        lay->flag_RBF = 0;
        lay->lay_output_func = 0;
        lay->lay_output_func_deriv = 0;
        lay->eta = 0;
        lay->gain = 0;
    }

    // Default values:
    for (mozgint i = 1; i < layers_num; i++) {
        layer[i].eta = (mozgflt) 0.3;
        layer[i].gain = (mozgflt) 1.;
    }
    switch_of_energy_function = 1;
    alpha = 0.;
    sigma_Langevin = 0.;
    sigma_Langevin_decrease = 0.;
    decay = 0.;
    flag_of_extensions = false;
    error_threshold = 0.;
    rate_coeff = 1.;
    gain_coeff = 1.;

    // make layers_num network layers, randomize weights
    for (mozgint i = 0; i < layers_num; i++) {
        lay = &(layer[i]);

        lay->outputs_num = lay_ouputs_num[i];

        lay->flag_RBF = false;

        if (i == 0)
            lay->func_number = 3;

        if (i > 0) {
            lay->func_number = func_num[i];
            if (lay->func_number == 4 || lay->func_number == 6) {
                lay->flag_RBF = true;
                (*net_memory)[7] = 1;
            }
            lay->lay_output_func = output_func[func_num[i]];
            lay->lay_output_func_deriv = output_func_deriv[func_num[i]];
        }
    }

    // self net
    (*net_memory)[0] = 1;
    (*net_memory)[1] = 1;
    (*net_memory)[2] = 1;
    (*net_memory)[3] = 1;
    (*net_memory)[4] = 1;
    (*net_memory)[6] = 1;
    (*net_memory)[21] = 1;
    // initial memory allocations of network
    memory_manager((mozgint) 0);

    for (mozgint i = 1; i < layers_num; i++) {
        randomize(layer[i].weights, -sweep, sweep);

        randomize(layer[i].sigmas, (mozgflt).9, (mozgflt) 1.1);
    }
}


// delete network
mozgMLP::
~mozgMLP() {
    memory_manager((mozgint) 1);
}

// put learning parameters for "vanilla" Backpropagation rule
void mozgMLP::
putBackprop(mozgflt learning_rate = .2, // common learning rate
        mozgint rate_mode = 0, // mode of layers' rates
        mozgflt temperature = 1., // temperature
        mozgint energy_function = 1, // switch of energy function
                mozgint num_for_chunk = 1, // num of examples in chunk
        mozgint steps_number = 1)   // number of iterations per
    // learning example
{
    switch_of_learn_rule = 1;
    steps_num = steps_number;
    chunk = num_for_chunk;
    if (chunk > 1) {
        (*necessary_memory)[10] = 1;
        if ((*real_memory)[4] == 1)
            (*necessary_memory)[5] = 1;
    }
    if (steps_num <= 0) {
        sendErrorMessage("Error in putBackprop(): steps_number<=0, "
                         "so it will be reset to 1 .",
            0);
        steps_num = 1;
    }
    if (chunk <= 0) {
        sendErrorMessage("Error in putBackprop(): num_for_chunk<=0, "
                         "so it will be reset to 1 .",
            0);
        chunk = 1;
    }

    for (mozgint i = 1; i < layers_num; i++) {
        struct_layer& lay = layer[i];

        lay.eta = rate_mode ?
            (learning_rate / layer[i].outputs_num) :
            learning_rate;
        lay.gain = (temperature) ? UNIT / temperature : 1 / EPS;

        lay.weights->putMaxAbs(MAX_ABS_WEIGHT);
        lay.weights->putMinAbs(MIN_ABS_WEIGHT);

        if (lay.func_number == 4 || lay.func_number == 6) {
            lay.sigmas->putMaxAbs(MAX_ABS_SIGMA);
            lay.sigmas->putMinAbs(MIN_ABS_SIGMA);
        }
    }

    switch_of_energy_function = energy_function;
    if (switch_of_energy_function< 1 || switch_of_energy_function>3) {
        sendErrorMessage("Error in putBackprop(): energy_function!=1,2,3, "
                         "so it will be reset to 1 .",
            0);
        switch_of_energy_function = 1;
    }
    if (energy_function == 2)
        (*necessary_memory)[22] = 1;
    alpha = 0.;
    sigma_Langevin = 0.;
    sigma_Langevin_decrease = 0.;
    decay = 0.;
    flag_of_extensions = false;

    memory_manager((mozgint) 0);
}

// put learning parameters for backpropagation with extensions rule
void mozgMLP::
putExtensions(mozgflt mom_term_coef = .3, // momentum term coefficient
          mozgflt lang_noise_begin_dispersion = 0., // begin Langevin
                                                   // Gauss dispersion
          mozgflt lang_noise_disp_decr_coef = 0., // Langevin Gauss
                                          // dispersion decreasing coef
          mozgflt weight_decay_coef = 0.) // weight decay coefficient
{
    if (mom_term_coef) {
        copy(*real_memory, necessary_memory);
        *necessary_memory -= *net_memory;
        (*necessary_memory)[9] = 1;
        (*necessary_memory)[17] = 1;
        memory_manager((mozgint) 0);
    }
    alpha = mom_term_coef;
    sigma_Langevin = lang_noise_begin_dispersion; 
    sigma_Langevin_decrease = lang_noise_disp_decr_coef;
    decay = weight_decay_coef;
    // may be there's possibility of using of o_adjust_lay_weights...
    flag_of_extensions = (alpha || decay || sigma_Langevin);
}

// put learning parameter for Qprop
void mozgMLP::
putQuickprop(mozgflt learning_rate = .2, // common learning rate
         mozgint rate_mode = 0, // mode of layers' rates
         mozgflt temperature = 1., // temperature
         mozgint energy_function = 1, // switch of energy function
                 mozgint num_for_chunk = 1, // num of examples in chunk
         mozgint steps_number = 10)  // number of iterations per
    // learning example
{
    struct_layer* lay;

    switch_of_learn_rule = 2;
    switch_of_energy_function = energy_function;
    if (switch_of_energy_function< 1 || switch_of_energy_function>3) {
        sendErrorMessage("Error in putQuickprop(): energy_function!=1,2,3, "
                         "so it will be reset to 1 .",
            0);
        switch_of_energy_function = 1;
    }

    if (energy_function == 2)
        (*necessary_memory)[22] = 1;

    steps_num = steps_number;
    chunk = num_for_chunk;
    if (chunk > 1) {
        (*necessary_memory)[10] = 1;
        (*necessary_memory)[13] = 1;
        if ((*real_memory)[4] == 1)
            (*necessary_memory)[5] = 1;
    }
    if (steps_num <= 0) {
        sendErrorMessage("Error in putQuickprop(): steps_number<=0, "
                         "so it will be reset to 10 .",
            0);
        steps_num = 10;
    }
    if (chunk <= 0) {
        sendErrorMessage("Error in putQuickprop(): num_for_chunk<=0, "
                         "so it will be reset to 1 .",
            0);
        chunk = 1;
    }

    // first iteration for Quickprop is made by adjust_lay_weights() 
    alpha = 0; 
    flag_of_extensions = 0;

    sigma_Langevin = 0.;
    sigma_Langevin_decrease = 0.;
    decay = 0.;

    for (mozgint i = 1; i < layers_num; i++) {
        lay = &layer[i];
        // for first iteration by Backprop
        lay->eta = rate_mode ?
            (learning_rate / lay->outputs_num) :
            learning_rate;
        lay->gain = (temperature) ? UNIT / temperature : 1 / EPS;

        lay->weights->putMaxAbs(MAX_ABS_WEIGHT);
        lay->weights->putMinAbs(MIN_ABS_WEIGHT);

        if (lay->flag_RBF) {
            lay->sigmas->putMaxAbs(MAX_ABS_SIGMA);
            lay->sigmas->putMinAbs(MIN_ABS_SIGMA);
            (*necessary_memory)[14] = 1;
            (*necessary_memory)[15] = 1;
            (*necessary_memory)[17] = 1;
            if (chunk > 1)
                (*necessary_memory)[16] = 1;
        }
    }
    (*necessary_memory)[9] = 1;
    (*necessary_memory)[11] = 1;
    (*necessary_memory)[12] = 1;
    (*necessary_memory)[22] = 1;

    memory_manager((mozgint) 0);
}

// put learning parameters for Rprop
void mozgMLP::
putRprop(mozgflt gamma_plus = 1.02, // constants in learning
         mozgflt gamma_minus = .5, //   rule
         mozgflt learning_rate = .2, // common learning rate
         mozgint rate_mode = 0, // mode of layers' rates
         mozgflt temperature = 1., // temperature
         mozgint energy_function = 1)   // switch of energy function
{
    struct_layer* lay = 0;

    switch_of_learn_rule = 3;
    steps_num = 1;

    gammaplus = gamma_plus;
    gammaminus = gamma_minus;
    switch_of_energy_function = energy_function;
    switch_of_energy_function = energy_function;
    if (switch_of_energy_function< 1 || switch_of_energy_function>3) {
        sendErrorMessage("Error in putQuickprop(): energy_function!=1,2,3, "
                         "so it will be reset to 1 .",
            0);
        switch_of_energy_function = 1;
    }

    if (energy_function == 2)
        (*necessary_memory)[22] = 1;

    // first iteration for Rprop is made by adjust_lay_weights()
    // to get first prev_p_deriv:
    alpha = 0;
    flag_of_extensions = 0;

    for (mozgint i = 1; i < layers_num; i++) {
        lay = &layer[i];

        lay->eta = (rate_mode) ?
            (learning_rate / lay->outputs_num) :
            learning_rate;
        lay->gain = (temperature) ? UNIT / temperature : 1. / EPS;

        lay->weights->putMaxAbs(MAX_ABS_WEIGHT);
        lay->weights->putMinAbs(MIN_ABS_WEIGHT);
        if (lay->flag_RBF) {
            lay->sigmas->putMaxAbs(MAX_ABS_SIGMA);
            lay->sigmas->putMinAbs(MIN_ABS_SIGMA);
            (*necessary_memory)[5] = 1;
            (*necessary_memory)[14] = 1;
            (*necessary_memory)[15] = 1;
            (*necessary_memory)[18] = 1;
        }
    }
    (*necessary_memory)[8] = 1;
    (*necessary_memory)[9] = 1;
    (*necessary_memory)[10] = 1;
    (*necessary_memory)[11] = 1;
    (*necessary_memory)[12] = 1;
    (*necessary_memory)[13] = 1;
    (*necessary_memory)[16] = 1;
    (*necessary_memory)[22] = 1;

    memory_manager((mozgint) 0);

    for (mozgint i = 1; i < layers_num; i++) {
        copy(layer[i].eta, layer[i].weight_etas);
        // upper and lower limit of weight etas in Rprop:
        lay->weight_etas->putMaxAbs(MAX_ABS_WEIGHT_ETA);
        lay->weight_etas->putMinAbs(MIN_ABS_WEIGHT_ETA);
        // the same for RBF
        if (lay->flag_RBF) {
            lay->sigma_etas->putMaxAbs(MAX_ABS_SIGMA_ETA);
            lay->sigma_etas->putMinAbs(MIN_ABS_SIGMA_ETA);
        }
    }
}


// learn net
void mozgMLP::
learnNet(mozgflt* linp, mozgflt* lout) {
    mozgint step_num = 0;

    if (switch_of_learn_rule == 3)
        sendErrorMessage("Error in learnNet a): it mustn't be used"
                         " with Rprop.",
            1);

    if (step_num < 0) {
        switch (switch_of_learn_rule) {
        case 1:
            step_num = 1;
            sendErrorMessage("Error in learnNet a): step_num < 0,"
                             " so it will be reset to 1 .",
                0);break;
        case 2:
            step_num = 10;
            sendErrorMessage("Error in learnNet a): step_num < 0,"
                             " so it will be reset to 10 .",
                0);
        }
    }
    if ((switch_of_energy_function == 2) &&
        ((layer[layers_num - 1].func_number == 0) ||
        (layer[layers_num - 1].func_number == 6)) &&
        (is_call_putNonStdFunction == false))
        sendErrorMessage("\nError in learnNet a): You haven't called "
                         "putNonStdFunction() before\n"
                         "learnNet() call,"
                         " so program will be aborted.\n",
            1);

    for (step_num = 1; step_num <= steps_num; step_num++) {
        put_inputs(linp);

        for (mozgint i = 1; i < layers_num; i++) {
            compute_lay_outputs(layer[i - 1], &layer[i]);
            if (flag_of_bias_term && (i != (layers_num - 1)))
                layer[i].outputs->vec()[layer[i].outputs_num - 1] = 1.;
        }
        switch (switch_of_learn_rule) {
        case 1:
            compute_output_errors(lout, true);
            backprop_errors();

            adjust_weights_by_backprop();
            break;

        case 2:
            // first step of Quickprop is made by Backprop:
            if (step_num == 1) {
                compute_output_errors(lout, true);
                backprop_errors();

                compute_partial_derivatives();

                adjust_weights_by_backprop();
            } else {
                compute_output_errors(lout, true);

                backprop_errors();

                compute_partial_derivatives();

                adjust_weights_by_quickprop(step_num);
            }
            break;

            // first step of SCG is made by Backprop:
        case 4:

                ;
        }
    }
}

// learn net
void mozgMLP::
learnNet(mozgflt** linp, mozgflt** lout, mozgint lssize, mozgint order,
    mozgint epnum, mozgint pepnum) {

    mozgint vecnum;
    mozgint random_vecnum = 0;
    mozgint vec_num = 0;
    mozgint step_num;

    if (switch_of_learn_rule == 3)
        chunk = lssize;

    if ((switch_of_energy_function == 2) &&
        ((layer[layers_num - 1].func_number == 0) ||
        (layer[layers_num - 1].func_number == 6)) &&
        (is_call_putNonStdFunction == false))
        sendErrorMessage("\nError in learnNet b): You haven't called "
                         "putNonStdFunction() before\n"
                         "learnNet() call,"
                         " so the program will be aborted.\n",
            1);

    mozgint max_num_in_chunk = (chunk <= lssize) ? (chunk) : (lssize);
    mozgint num_in_chunk;

    for (mozgint tprint = 1, i = 1;
        i <= epnum;
        tprint++, i++, overall_epnum++) {
        mozgflt lerr = (mozgflt) 0.;

        for (vecnum = 0; vecnum < lssize; vecnum++) {
            if (!order)
                random_vecnum = (mozgint) random_number((mozgflt) 0.,
                                            (mozgflt) lssize);

            vec_num = (!order) ? random_vecnum : vecnum;

            for (step_num = 1; step_num <= steps_num; step_num++) {
                mozgint local_chunk = ((lssize - vecnum) > max_num_in_chunk) ?
                    (max_num_in_chunk) :
                    (lssize - vecnum);

                flag_of_first_chunk_example = true;
                flag_of_last_chunk_example = false;

                for (num_in_chunk = local_chunk;
                    num_in_chunk > 0;
                    num_in_chunk--) {
                    // flag for weights adjustment
                    if (num_in_chunk == 1)
                        flag_of_last_chunk_example = true;

                    put_inputs(linp[vec_num]);

                    for (mozgint j = 1; j < layers_num; j++) {
                        compute_lay_outputs(layer[j - 1], &layer[j]);
                        if (flag_of_bias_term && (j != (layers_num - 1)))
                            layer[j].outputs->vec()[layer[j].outputs_num - 1] = 1.;
                    }
                    switch (switch_of_learn_rule) {
                        // Backprop:
                    case 1:
                        compute_output_errors(lout[vec_num], true);
                        backprop_errors();

                        adjust_weights_by_backprop();
                        break;

                    case 2:
                        // first step of Quickprop is made by Backprop:
                        if (step_num == 1) {
                            compute_output_errors(lout[vec_num], true);
                            backprop_errors();

                            adjust_weights_by_backprop();

                            // compute derivative for second step which will be done by Quickprop:
                            compute_partial_derivatives();
                        } else {
                            compute_output_errors(lout[vec_num], true);

                            backprop_errors();

                            compute_partial_derivatives();

                            adjust_weights_by_quickprop(step_num);
                        }
                        break;

                    case 3:
                        // first step of Rprop is made by Backprop:
                        if (step_num == 1) {
                            compute_output_errors(lout[vec_num], true);
                            backprop_errors();

                            // compute derivative for second step which will be done by Rprop:
                            compute_partial_derivatives();

                            adjust_weights_by_backprop();
                        } else {
                            compute_output_errors(lout[vec_num], true);

                            backprop_errors();

                            compute_partial_derivatives();

                            adjust_weights_by_rprop(step_num);
                        }
                        break;

                    case 4:
                        // first step of SCG is made by Backprop:
                                ;}
                    if (step_num == steps_num)
                        lerr += error_over_outputs;
                    //          if (chunk==1
                    //          || flag_of_last_chunk_example)
                    //        adjust_weights (step_num);
                    flag_of_first_chunk_example = false;
                }
            }
        }
        if (tprint == pepnum) {
            tprint = 0;

            // finish (1) or continue (0) learning...
            if (answerMessage(lerr / lssize, (mozgflt) 0))
                i = epnum + 1;
        }
    }
}

// learn and test net
void mozgMLP::
learnNet(mozgflt** linp, mozgflt** lout, mozgint lssize, mozgint order,
    mozgint epnum, mozgint pepnum, mozgflt** tinp, mozgflt** tout,
    mozgint tssize) {
    mozgint vecnum;
    mozgint random_vecnum = 0;
    mozgint vec_num = 0;
    mozgint step_num;

    if (switch_of_learn_rule == 3) {
        steps_num = 1;
        chunk = lssize;
    }

    if ((switch_of_energy_function == 2) &&
        ((layer[layers_num - 1].func_number == 0) ||
        (layer[layers_num - 1].func_number == 6)) &&
        (is_call_putNonStdFunction == false))
        sendErrorMessage("\nError in learnNet c): You haven't called "
                         "putNonStdFunction() before\n"
                         "learnNet() call,"
                         " so the program will be aborted.\n",
            1);

    mozgint max_num_in_chunk = (chunk <= lssize) ? (chunk) : (lssize);
    mozgint num_in_chunk;

    for (mozgint tprint = 1, i = 1;
        i <= epnum;
        tprint++, i++, overall_epnum++) {
        mozgflt lerr = (mozgflt) 0.;

        for (vecnum = 0; vecnum < lssize; vecnum++) {
            if (!order)
                random_vecnum = (mozgint) random_number((mozgflt) 0.,
                                            (mozgflt) lssize);

            vec_num = (!order) ? random_vecnum : vecnum;

            for (step_num = 1; step_num <= steps_num; step_num++) {
                mozgint local_chunk = ((lssize - vecnum) > max_num_in_chunk) ?
                    (max_num_in_chunk) :
                    (lssize - vecnum);

                flag_of_first_chunk_example = true;
                flag_of_last_chunk_example = false;

                for (num_in_chunk = local_chunk;
                    num_in_chunk > 0;
                    num_in_chunk--) {
                    // flag for weights adjustment
                    if (num_in_chunk == 1)
                        flag_of_last_chunk_example = true;

                    put_inputs(linp[vec_num]);

                    for (mozgint j = 1; j < layers_num; j++) {
                        compute_lay_outputs(layer[j - 1], &layer[j]);
                        if (flag_of_bias_term && (j != (layers_num - 1)))
                            layer[j].outputs->vec()[layer[j].outputs_num - 1] = 1.;
                    }
                    switch (switch_of_learn_rule) {
                        // Backprop:
                    case 1:
                        compute_output_errors(lout[vec_num], true);
                        backprop_errors();

                        adjust_weights_by_backprop();
                        break;

                    case 2:
                        // first step of Quickprop is made by Backprop:
                        if (step_num == 1) {
                            compute_output_errors(lout[vec_num], true);
                            backprop_errors();

                            adjust_weights_by_backprop();

                            // compute derivative for second step which will be done by Quickprop:
                            compute_partial_derivatives();
                        } else {
                            compute_output_errors(lout[vec_num], true);

                            backprop_errors();

                            compute_partial_derivatives();

                            adjust_weights_by_quickprop(step_num);
                        }
                        break;

                    case 3:
                        // first step of Rprop is made by Backprop:
                        if (step_num == 1) {
                            compute_output_errors(lout[vec_num], true);
                            backprop_errors();

                            // compute derivative for second step which will be done by Rprop:
                            compute_partial_derivatives();

                            adjust_weights_by_backprop();
                        } else {
                            compute_output_errors(lout[vec_num], true);

                            backprop_errors();

                            compute_partial_derivatives();

                            adjust_weights_by_rprop(step_num);
                        }
                        break;

                    case 4:
                        // first step of SCG is made by Backprop:
                                ;}
                    if (step_num == steps_num)
                        lerr += error_over_outputs;

                    flag_of_first_chunk_example = false;
                }
            }
        }
        if (tprint == pepnum) {
            tprint = 0;
            mozgflt tsqer = 0.;

            for (vecnum = 0; vecnum < tssize; vecnum++) {
                put_inputs(tinp[vecnum]);

                compute_output_errors(tout[vecnum], false);

                tsqer += error_over_outputs;
            }
            // finish (1) or continue (0) learning...
            if (answerMessage(lerr / lssize, tsqer / tssize))
                i = epnum + 1;
        }
    }
}


// test network, returns mean square error over test set
mozgflt mozgMLP::
testNet(mozgflt** tinp, mozgflt** tout, mozgint tssize) {
    mozgflt testerror = 0.;

    for (mozgint vecnum = 0; vecnum < tssize; vecnum++) {
        put_inputs(tinp[vecnum]);

        for (mozgint i = 1; i < layers_num; i++) {
            compute_lay_outputs(layer[i - 1], &layer[i]);
            if (flag_of_bias_term && (i != (layers_num - 1)))
                layer[i].outputs->vec()[layer[i].outputs_num - 1] = 1.;
        }
        compute_output_errors(tout[vecnum], false);

        testerror += error_over_outputs;
    }
    return (testerror / tssize);
}


// propagate input vector forward through network to the outputs and save
// outputs in the outvec
void mozgMLP::
propInputs(mozgflt* invec, mozgflt* outvec) {
    put_inputs(invec);

    for (mozgint i = 1; i < layers_num; i++) {
        compute_lay_outputs(layer[i - 1], &layer[i]);
        if (flag_of_bias_term && (i != (layers_num - 1)))
            layer[i].outputs->vec()[layer[i].outputs_num - 1] = 1.;
    }
    get_outputs(outvec);
}

// propagate vectors of the invecarr forward through network to the outputs
// and save outputs for each vector in outvecarr
void mozgMLP::
propInputs(mozgflt** invecarr, mozgflt** outvecarr, mozgint ssize) {
    for (mozgint vecnum = 0; vecnum < ssize; vecnum++) {
        put_inputs(invecarr[vecnum]);

        for (mozgint i = 1; i < layers_num; i++) {
            compute_lay_outputs(layer[i - 1], &layer[i]);
            if (flag_of_bias_term && (i != (layers_num - 1)))
                layer[i].outputs->vec()[layer[i].outputs_num - 1] = 1.;
        }
        get_outputs(outvecarr[vecnum]);
    }
}

// initialise non-standart output function parameters (if it on output layer
// and cross-entropy is used)
void mozgMLP::
putNonStdFunction(mozgflt min, mozgflt max) {
    nonstandfunc_min = min;
    nonstandfunc_max = max;

    if (min == max || min > max) {
        sendErrorMessage("Error in putNonStdFunction(): min==max or min>max"
                         " so the program will be aborted.",
            1);
    }
    if (switch_of_energy_function == 2)
        (*net_memory)[22] = 1;

    if ((min == 0.) && (max == 1.))
        nonstand_func_type = (mozgint) 1;
    else if ((min == -1.) && (max == 1.))
        nonstand_func_type = (mozgint) 2;
    else {
        nonstand_func_type = (mozgint) 3;
        if (switch_of_energy_function == 2)
            (*net_memory)[23] = 1;
    }
    is_call_putNonStdFunction = true;
}

}
