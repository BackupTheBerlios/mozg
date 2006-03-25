
// $Id: t1.cc,v 1.3 2006/03/25 20:12:53 lightdruid Exp $


#include <mozg/mozgMLP.hh>

using namespace mozg;

class M : public mozgMLP {

public:
    M(mozgint number_of_layers, mozgint* lay_ouputs_num,
        mozgint* func_num, mozgflt sweep, bool bias_term_flag) :
            mozgMLP(number_of_layers, lay_ouputs_num,
                func_num, sweep, bias_term_flag) {
     }


    // delete network
    virtual ~M() {
    }

    bool answerMessage(mozgflt e1, mozgflt e2) {
        return false;
    }
};

bool test(mozgflt e1, mozgflt e2) {
    return false;
}

int main() {

    mozgint nlayers = 3;
    mozgint nunits = 3;
    mozgflt sweep = 0.04;
    bool flag = false;

    mozgint nfunc[] = { 0, 4, 2 };

    M* mlp = new M(nlayers, &nunits, nfunc, sweep, flag);
    //delete mlp;

    return 0;
}
