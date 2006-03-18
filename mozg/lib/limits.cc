// $Id: limits.cc,v 1.1 2006/03/18 21:53:18 lightdruid Exp $
// 
// limits.cc
// limits for weights, sigmas (RBF),...
//

#include  "mozg/limits.hh"


namespace mozg {

  mozgflt MAX_ABS_WEIGHT           =  10.;
  mozgflt MIN_ABS_WEIGHT           =  0.;
  mozgflt MAX_ABS_SIGMA            =  10.;
  mozgflt MIN_ABS_SIGMA            =  0.;
  mozgflt MAX_ABS_LAST_DELTA       =  1e-3;
  mozgflt MIN_ABS_LAST_DELTA       =  0.;
  mozgflt MAX_ABS_SIGMA_LAST_DELTA =  1e-3;
  mozgflt MIN_ABS_SIGMA_LAST_DELTA =  0.;
  mozgflt MAX_ABS_WEIGHT_ETA       =  1e-1;
  mozgflt MIN_ABS_WEIGHT_ETA       =  0.;
  mozgflt MAX_ABS_SIGMA_ETA        =  1e-1;
  mozgflt MIN_ABS_SIGMA_ETA        =  0.;


}
