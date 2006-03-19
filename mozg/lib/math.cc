// $Id: math.cc,v 1.2 2006/03/19 20:46:17 lightdruid Exp $
// 
// math.cc
// mozg neuron output functions, some constants and numbers
//

#include  "mozg/math.hh"


namespace mozg {

// Coefficient for scaling of second order term in Quickprop
mozgflt CUT_QUICKPROP = (mozgflt) 1e-3;

// EPS and OVER_UNIT removes possibility of floating point exception
// (division by zero in Cross-Entropy, log-squared error)
// if EPS influences on computational resuts decrease it.
mozgflt EPS = (mozgflt) 1e-6;
const mozgflt UNIT = (mozgflt) (1.);
mozgflt OVER_UNIT = (UNIT + EPS);
const mozgflt TWO = (mozgflt) (2.);
const mozgflt HALF_UNIT = (mozgflt) (1. / 2.);
const mozgflt MOZG_2_PI = (mozgflt) M_2_PI;

// neuron output function:
mozgflt mozg_exp(mozgflt net, mozgflt sigm) {
    return UNIT / (UNIT + exp(-net));
}

mozgflt mozg_th(mozgflt net, mozgflt sigm) {
    return tanh(net);
}

mozgflt mozg_lin(mozgflt net, mozgflt sigm) {
    return net;
}

mozgflt mozg_rbf(mozgflt RBFnet, mozgflt sigm) {
    return exp(-HALF_UNIT * RBFnet / (sigm * sigm));
}

mozgflt mozg_atg(mozgflt net, mozgflt sigm) {
    return MOZG_2_PI * atan(net);
}


// user defined neuron output function:
mozgflt mozg_weinet(mozgflt net, mozgflt sigm) {
    return 0 /* some function (from net) */;
}

mozgflt mozg_rbfnet(mozgflt RBFnet, mozgflt sigm) {
    return 0 /* some function from RBFnet */;
}


// derivative for neuron output function:
mozgflt mozg_D_exp(mozgflt out, mozgflt net, mozgflt sigm) {
    return out * (UNIT - out);
}

mozgflt mozg_D_th(mozgflt out, mozgflt net, mozgflt sigm) {
    return (UNIT - out * out);
}

mozgflt mozg_D_lin(mozgflt out, mozgflt net, mozgflt sigm) {
    return UNIT;
}

mozgflt mozg_D_rbf(mozgflt out, mozgflt RBFnet, mozgflt sigm) {
    return out / (sigm * sigm);
}

mozgflt mozg_D_atg(mozgflt out, mozgflt net, mozgflt sigm) {
    return MOZG_2_PI / (UNIT + net * net);
}


// derivative for user defined output function:
mozgflt mozg_D_weinet(mozgflt out, mozgflt net, mozgflt sigm) {
    return 0 /* derivative some function (from net || out) */;
}

mozgflt mozg_D_rbfnet(mozgflt out, mozgflt RBFnet, mozgflt sigm) {
    return 0 /* derivative some function from somethings */;
}

}
