
// $Id: t1.cc,v 1.4 2006/03/25 20:47:41 lightdruid Exp $


#include <mozg/mozgMLP.hh>

using namespace mozg;

class M : public mozgMLP {

public:
    M(mozgint number_of_layers, mozgint* lay_ouputs_num,
        mozgint* func_num, mozgflt sweep, bool bias_term_flag) :
            mozgMLP(number_of_layers, lay_ouputs_num,
                func_num, sweep, bias_term_flag) {
     }

    virtual ~M() {
    }

    bool answerMessage(mozgflt e1, mozgflt e2) {
        std::cerr << "e1=" << e1 << " e2=" << e2 << std::endl;
        return true;
    }
};

bool test(mozgflt e1, mozgflt e2) {
    return false;
}

int main() {

    //mozgint nlayers = 3;
    //mozgint nunits = 3;
    //mozgflt sweep = 0.04;
    //bool flag = false;

    float* learning_set_inputs;
    float* learning_set_outputs;
    float* test_set_inputs;
    float* test_set_outs;
    float   eta;
    float   weight_sweep;
    float   learning_rate;
    float   temperature;

    int  number_layers;
    int* num_of_outputs_at_layers = new int;
    int* num_of_output_f;
    int  num_for_chunk_updating;
    int  learning_set_size;
    int  test_set_size;
    int  vec_order;
    int  rate_mode;
    int  energy_function;
    int  steps_number;

    bool flag_of_bias_term;

    int epoch_num;
    int print_epoch_num;

    // init
    number_layers = 3;
    *num_of_outputs_at_layers = 3;

    num_of_output_f = new int[3];
    num_of_output_f[0] = 0;
    num_of_output_f[1] = 4;
    num_of_output_f[2] = 2;

    weight_sweep = 0.04;
    flag_of_bias_term = false;

    M* net = new M(number_layers,
                    num_of_outputs_at_layers,
                    num_of_output_f,
                    weight_sweep,
                    flag_of_bias_term);


    learning_set_inputs = new float[5];
    learning_set_outputs = new float[5];
    test_set_inputs = new float[5];
    test_set_outs = new float[5];


    for(int i = 0; i < 5; ++i) {
        learning_set_inputs[i] = (float)i;
        learning_set_outputs[i] = 0.;
        test_set_inputs[i] = i + 10.;
        test_set_outs[i] = 0.;
    }

    learning_set_size = 5;
    test_set_size = 5;
    vec_order = 1;

    epoch_num = 1;
    print_epoch_num = 1;

    net->learnNet (&learning_set_inputs,
                    &learning_set_outputs,
                    learning_set_size,
                    vec_order,
                    epoch_num,
                    print_epoch_num,
                    &test_set_inputs,
                    &test_set_outs,
                    test_set_size);

    delete net;

    return 0;
}
