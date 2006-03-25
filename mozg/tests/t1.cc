
// $Id: t1.cc,v 1.2 2006/03/25 11:18:04 lightdruid Exp $


#include <mozg/mozgMLP.hh>

using namespace mozg;

bool test(mozgflt e1, mozgflt e2) {
    return false;
}

int main() {

    mozgint nlayers = 3;
    mozgint nunits = 3;
    mozgflt sweep = 0.04;
    bool flag = false;

    mozgint nfunc[] = { 0, 4, 2 };

    mozgMLP* mlp = new mozgMLP(nlayers, &nunits, nfunc, sweep, flag, &test);
    delete mlp;

    return 0;
}
