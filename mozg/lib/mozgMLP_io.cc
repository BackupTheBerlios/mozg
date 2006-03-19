// $Id: mozgMLP_io.cc,v 1.2 2006/03/19 20:46:17 lightdruid Exp $
// 
// mozgMLP_io.cc
// Multi-Layer Perceptron (MLP) simulator i/o functions
//

#include "mozg/mozgMLP_io.hh"


namespace mozg {

static const char IdentString[] = "mozgMLP "MOZG_PACKAGE" data file "MOZG_VERSION;

static const char OldIdentString[] = MOZG_PACKAGE" data file "MOZG_VERSION;


std::ofstream& operator <<(std::ofstream& output, const mozgMLP& net) {
    mozgint i = 0;

    if (output.good()) {
        output << IdentString << "\n";

        if (net.flag_of_bias_term)
            output << "bias term\n";
        else
            output << "\n";

        output << "the number of layers                    : "
            << net.layers_num;

        output << "\nthe number of neurons in a layer        :";

        for (i = 0; i < net.layers_num; i++) {
            output << ' ' << net.layer[i].outputs_num;
        }
        output << "\nthe number of output function in a layer:";
        //    i = net.layers_num;
        for (i = 0; i < net.layers_num; i++) {
            output << ' ' << net.layer[i].func_number;
        }
        output << std::endl;

        for (i = 1; i < net.layers_num; i++) {
            output << "\nlayer [" << i << "]:\n";

            output << (*net.layer[i].weights);

            if ((net.layer[i].func_number == 4) ||
                (net.layer[i].func_number == 6))
                output << (*net.layer[i].sigmas) << std::endl;
        }
    } else {
        std::cerr << "\nI'm congratulating you."
            << "You have some trouble with net saving ;)\n";
        // ? exception handler call...
    }
    return output;
}

std::ifstream& operator >>(std::ifstream& input, mozgMLP** p_p_net) {
    mozgint i;
    mozgint layers_num;
    mozgint* num_neurons;
    mozgint* function_num;

    bool bias_flag = false;

    char temp[50];

    if (!input.good()) {
        std::cerr << "\nFailed to open \"";
        // ? exception handler call...
    } else {
        input.getline(temp, 50, '\n');
        // compare first 12 symbols of the temp and IdentString
        // && temp and OldIdentString
        if (strncmp(temp, IdentString, 27) &&
            strncmp(temp, OldIdentString, 12)) {
            std::cerr << "Unsupported file version, or garbage in file.\n";
            // ? exception handler call...
        } else {
            //      input.ignore(1);
            input.getline(temp, 50, '\n');
            if (strcmp(temp, "bias term"))
                bias_flag = false;
            else if (strcmp(temp, ""))
                bias_flag = true;
            else
                std::cerr << "Reading error: no \"\""
                    << " (or \"bias term\") line.\n";

            input.getline(temp, 50, ':');
            input >> layers_num;
            if (strcmp(temp, "the number of layers                    ")) {
                std::cerr <<
                    "Reading error: no "
                    "\"the number of layers                    :\" line.\n";
                // ? exception handler call...
            } else {
                num_neurons = new mozgint[layers_num];
                function_num = new mozgint[layers_num];

                input.getline(temp, 50, ':');
                for (i = 0; i < layers_num; i++) {
                    input >> num_neurons[i] ;
                }
                input.getline(temp, 50, ':');
                for (i = 0; i < layers_num; i++) {
                    input >> function_num[i];
                }
                *p_p_net = new mozgMLP(layers_num, num_neurons, function_num,
                                (mozgflt) 0., bias_flag);

                for (i = 1; i < layers_num; i++) {
                    input.ignore(2);
                    input.getline(temp, 50, '\n');
                    input >> (*(**p_p_net).layer[i].weights);
                    if (!input.good()) {
                        std::cerr << "Premature EOF on data file\n";
                        // ? exception handler call...
                    }
                    if ((function_num[i] == 4) || (function_num[i] == 6)) {
                        input >> (*(**p_p_net).layer[i].sigmas);
                    }
                }
                delete[] num_neurons;
                delete[] function_num;
            }
        }
    }
    return input;
}

std::ostream& operator <<(std::ostream& output, const mozgMLP& net) {
    mozgint i = 0;

    if (output.good()) {
        output << "\nthe number of layers                    : "
            << net.layers_num;

        output << "\nthe number of neurons in a layer        :";
        i = net.layers_num;
        for (i = 0; i < net.layers_num; i++) {
            output << ' ' << net.layer[i].outputs_num;
        }

        output << "\nthe number of output function in a layer:";
        i = net.layers_num;
        for (i = 0; i < net.layers_num; i++) {
            output << ' ' << net.layer[i].func_number;
        }
        output << std::endl;

        if (net.layer[1].eta) {
            output << "eta   = " << net.layer[1].eta << std::endl;
            output << "alpha = " << net.alpha << std::endl;
            output << "Lang  = " << net.sigma_Langevin << std::endl;
            output << "decay = " << net.decay << std::endl;
        }
        for (i = 1; i < net.layers_num; i++) {
            output << "\nlayer [" << i << "]:\n";

            output << (*net.layer[i].weights);

            if (net.layer[i].func_number == 4)
                output << (*net.layer[i].sigmas) << std::endl;
        }
    } else {
        std::cerr << "\nI'm congratulating you."
            << "You have some trouble with net showing ;)\n";
        // ? exception handler call...
    }
    return output;
}

}
