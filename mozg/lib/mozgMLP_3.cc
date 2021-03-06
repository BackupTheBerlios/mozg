// $Id: mozgMLP_3.cc,v 1.2 2006/03/19 20:46:17 lightdruid Exp $
// 
// mozgMLP_3.cc
// MultiLayer Perceptron (mozgMLP) simulator manager of dynamic memory
// performes initial allocations, allocates memory by request, zeroes it (it's
// a good taste) and makes garbage collection
#include "mozg/mozgMLP.hh"


// Table of relations of link vectors  components to dynamic objects of
// network:
// ( s -- struct_layer,
//   v -- vector,
//  cv -- checked_vector,
//   m -- matrix,
//  cm -- checked_matrix )
//
// 0  s  layer[i]
// 1  v  layer[i].outputs
// 2  v  layer[i].nets
// 3  v  layer[i].errors
// 4  cv layer[i].sigmas
// 5  v  layer[i].sigmas_in_chunk
// 6  cm layer[i].weights
// 7  m  layer[i].RBFinputs
// 8  cm layer[i].weight_etas
// 9  cm layer[i].last_deltas
// 10 m  layer[i].deltas_in_chunk
// 11 m  layer[i].pderivs
// 12 m  layer[i].prev_pderivs
// 13 m  layer[i].pderivs_in_chunk
// 14 v  layer[i].sigma_pderivs
// 15 v  layer[i].sigma_prev_pderivs
// 16 v  layer[i].sigma_pderivs_in_chunk
// 17 cv layer[i].last_sigma_deltas
// 18 cv layer[i].sigma_etas
// 19    layer[i].   reserve
// 20    layer[i].   reserve
// 21 v  diff
// 22 v  temp_vector
// 23 v  temp_vector2
// 24                reserve
// 25                reserve
// 26                reserve
// 27                reserve
// 28                reserve
// 29                reserve

namespace mozg {

void mozgMLP::
memory_manager(mozgint end_call) {
    // allocation of array of struct layer for initialisation of network
    if (real_memory == 0 && necessary_memory == 0) {
        real_memory = new vector< mozgint,mozgint>((mozgint) 30);
        necessary_memory = new vector< mozgint,mozgint>((mozgint) 30);
        net_memory = new vector< mozgint,mozgint>((mozgint) 30);
        assert(real_memory);
        assert(necessary_memory);
        assert(net_memory);
        zero(real_memory);
        zero(necessary_memory);
        zero(net_memory);

        layer = new struct_layer[layers_num];
        assert(layer);

        return;
    }
    // add the network memory to necessary for a learning rule memory:
    if (!end_call)
        *necessary_memory |= *net_memory;

    // get memory for allocation (1) or delete it (-1):
    *necessary_memory -= *real_memory;

    struct_layer* lay = 0;
    // allocate (1) or delete (-1) memory bits:
    if ((*necessary_memory)[1] == 1) {
        layer[0].outputs = new vector< mozgflt,mozgint>(layer[0].outputs_num);
        assert(layer[0].outputs);
        zero(layer[0].outputs);
    }
    if ((*necessary_memory)[1] == -1) {
        delete layer[0].outputs;
        layer[0].outputs = 0;
    }

    for (mozgint i = 1; i < layers_num; i++) {
        lay = &(layer[i]);
        bool flag_RBF = lay->flag_RBF;

        if ((*necessary_memory)[1] == 1) {
            lay->outputs = new vector< mozgflt,mozgint>(lay->outputs_num);
            assert(lay->outputs);
            zero(lay->outputs);
        }
        if ((*necessary_memory)[1] == -1) {
            delete lay->outputs;
            lay->outputs = 0;
        }

        if ((*necessary_memory)[2] == 1) {
            lay->nets = new vector< mozgflt,mozgint>(lay->outputs_num);
            assert(lay->nets);
            zero(lay->nets);
        }
        if ((*necessary_memory)[2] == -1) {
            delete lay->nets;
            lay->nets = 0;
        }

        if ((*necessary_memory)[3] == 1) {
            lay->errors = new vector< mozgflt,mozgint>(lay->outputs_num);
            assert(lay->errors);
            zero(lay->errors);
        }
        if ((*necessary_memory)[3] == -1) {
            delete lay->errors;
            lay->errors = 0;
        }

        if ((*necessary_memory)[4] == 1) {
            lay->sigmas = new checked_vector< mozgflt,mozgint>(lay->outputs_num);
            assert(lay->sigmas);
            zero(lay->sigmas);
        }
        if ((*necessary_memory)[4] == -1) {
            delete lay->sigmas;
            lay->sigmas = 0;
        }

        if ((*necessary_memory)[5] == 1 && flag_RBF) {
            lay->sigmas_in_chunk = new vector< mozgflt,mozgint>(lay->outputs_num);
            assert(lay->sigmas_in_chunk);
            zero(lay->sigmas_in_chunk);
        }
        if ((*necessary_memory)[5] == -1 && flag_RBF) {
            delete lay->sigmas_in_chunk;
            lay->sigmas_in_chunk = 0;
        }

        if ((*necessary_memory)[6] == 1) {
            lay->weights = new checked_matrix< mozgflt,mozgint>(layer[i].outputs_num,
                layer[i - 1].outputs_num);
            assert(lay->weights);
            zero(lay->weights);
        }
        if ((*necessary_memory)[6] == -1) {
            delete lay->weights;
            lay->weights = 0;
        }

        if ((*necessary_memory)[7] == 1) {
            if (flag_RBF)
                lay->RBFinputs = new matrix< mozgflt,mozgint>(layer[i].outputs_num,
                    layer[i - 1].outputs_num);
            else
                lay->RBFinputs = new matrix< mozgflt,mozgint>(1, 1);
            assert(lay->RBFinputs);
            zero(lay->RBFinputs);
        }
        if ((*necessary_memory)[7] == -1 && (flag_RBF)) {
            delete lay->RBFinputs;
            lay->RBFinputs = 0;
        }

        if ((*necessary_memory)[8] == 1) {
            lay->weight_etas = new checked_matrix< mozgflt,mozgint>(layer[i].outputs_num,
                layer[i - 1].outputs_num);
            assert(lay->weight_etas);
            zero(lay->weight_etas);
        }
        if ((*necessary_memory)[8] == -1) {
            delete lay->weight_etas;
            lay->weight_etas = 0;
        }

        if ((*necessary_memory)[9] == 1) {
            lay->last_deltas = new checked_matrix< mozgflt,mozgint>(layer[i].outputs_num,
                layer[i - 1].outputs_num);
            assert(lay->last_deltas);
            zero(lay->last_deltas);
        }
        if ((*necessary_memory)[9] == -1) {
            delete lay->last_deltas;
            lay->last_deltas = 0;
        }

        if ((*necessary_memory)[10] == 1) {
            lay->deltas_in_chunk = new matrix< mozgflt,mozgint>(layer[i].outputs_num,
                layer[i - 1].outputs_num);
            assert(lay->deltas_in_chunk);
            zero(lay->deltas_in_chunk);
        }
        if ((*necessary_memory)[10] == -1) {
            delete lay->deltas_in_chunk;
            lay->deltas_in_chunk = 0;
        }

        if ((*necessary_memory)[11] == 1) {
            lay->pderivs = new matrix< mozgflt,mozgint>(layer[i].outputs_num,
                layer[i - 1].outputs_num);
            assert(lay->pderivs);
            zero(lay->pderivs);
        }
        if ((*necessary_memory)[11] == -1) {
            delete lay->pderivs ;
            lay->pderivs = 0;
        }

        if ((*necessary_memory)[12] == 1) {
            lay->prev_pderivs = new matrix< mozgflt,mozgint>(layer[i].outputs_num,
                layer[i - 1].outputs_num);
            assert(lay->prev_pderivs);
            zero(lay->prev_pderivs);
        }
        if ((*necessary_memory)[12] == -1) {
            delete lay->prev_pderivs;
            lay->prev_pderivs = 0;
        }

        if ((*necessary_memory)[13] == 1) {
            lay->pderivs_in_chunk = new matrix< mozgflt,mozgint>(layer[i].outputs_num,
                layer[i - 1].outputs_num);
            assert(lay->pderivs_in_chunk);
            zero(lay->pderivs_in_chunk);
        }
        if ((*necessary_memory)[13] == -1) {
            delete lay->pderivs_in_chunk;
            lay->pderivs_in_chunk = 0;
        }

        if ((*necessary_memory)[14] == 1 && flag_RBF) {
            lay->sigma_pderivs = new vector< mozgflt,mozgint>(lay->outputs_num);
            assert(lay->sigma_pderivs);
            zero(lay->sigma_pderivs);
        }
        if ((*necessary_memory)[14] == -1 && flag_RBF) {
            delete lay->sigma_pderivs;
            lay->sigma_pderivs = 0;
        }

        if ((*necessary_memory)[15] == 1 && flag_RBF) {
            lay->sigma_prev_pderivs = new vector< mozgflt,mozgint>(lay->outputs_num);
            assert(lay->sigma_prev_pderivs);
            zero(lay->sigma_prev_pderivs);
        }
        if ((*necessary_memory)[15] == -1 && flag_RBF) {
            delete lay->sigma_prev_pderivs;
            lay->sigma_prev_pderivs = 0;
        }

        if ((*necessary_memory)[16] == 1 && flag_RBF) {
            lay->sigma_pderivs_in_chunk = new vector< mozgflt,mozgint>(lay->outputs_num);
            assert(lay->sigma_pderivs_in_chunk);
            zero(lay->sigma_pderivs_in_chunk);
        }
        if ((*necessary_memory)[16] == -1 && flag_RBF) {
            delete lay->sigma_pderivs_in_chunk;
            lay->sigma_pderivs_in_chunk = 0;
        }

        if ((*necessary_memory)[17] == 1 && flag_RBF) {
            lay->last_sigma_deltas = new checked_vector< mozgflt,mozgint>(lay->outputs_num);
            assert(lay->last_sigma_deltas);
            zero(lay->last_sigma_deltas);
        }
        if ((*necessary_memory)[17] == -1 && flag_RBF) {
            delete lay->last_sigma_deltas;
            lay->last_sigma_deltas = 0;
        }

        if ((*necessary_memory)[18] == 1 && flag_RBF) {
            lay->sigma_etas = new checked_vector< mozgflt,mozgint>(lay->outputs_num);
            assert(lay->sigma_etas);
            zero(lay->sigma_etas);
        }
        if ((*necessary_memory)[18] == -1 && flag_RBF) {
            delete lay->sigma_etas;
            lay->sigma_etas = 0;
        }
        /* reserve
              if ((*necessary_memory)[19]==1)
                {
                  lay-> =
                new;
                  assert(lay->);
                }
              if ((*necessary_memory)[19]==-1)
                {
                  delete lay->;
                  lay-> = 0;
              if ((*necessary_memory)[20]==1)
                {
                  lay-> =
                new matrix<mozgflt,mozgint>(layer[i].outputs_num,
                                layer[i-1].outputs_num);
                  assert(lay->);
                }
              if ((*necessary_memory)[20]==-1)
                {
                  delete lay->;
                  lay-> = 0;
                }
        */
    }
    if ((*necessary_memory)[21] == 1) {
        diff = new vector< mozgflt,mozgint>(layer[layers_num - 1].outputs_num);
        assert(diff);
        zero(diff);
    }
    if ((*necessary_memory)[21] == -1) {
        delete diff;
        diff = 0;
    }

    if ((*necessary_memory)[22] == 1) {
        temp_vector = new vector< mozgflt,mozgint>(layer[layers_num - 1].outputs_num);
        assert(temp_vector);
        zero(temp_vector);
    }
    if ((*necessary_memory)[22] == -1) {
        delete temp_vector;
        temp_vector = 0;
    }

    if ((*necessary_memory)[23] == 1) {
        temp_vector2 = new vector< mozgflt,mozgint>(layer[layers_num - 1].outputs_num);
        assert(temp_vector2);
        zero(temp_vector2);
    }
    if ((*necessary_memory)[23] == -1) {
        delete temp_vector2;
        temp_vector2 = 0;
    }
    /* reserve
          if ((*necessary_memory)[24]==1)
        {
          = new ;
          assert();
          zero ();
        }
          if ((*necessary_memory)[24]==-1)
        {
          delete ;
          = 0;
        }
          if ((*necessary_memory)[25]==1)
        {
          = new ;
          assert();
          zero ();
        }
          if ((*necessary_memory)[25]==-1)
        {
          delete ;
          = 0;
          if ((*necessary_memory)[26]==1)
        {
          = new ;
          assert();
          zero ();
        }
          if ((*necessary_memory)[26]==-1)
        {
          delete ;
          = 0;
          if ((*necessary_memory)[27]==1)
        {
          = new ;
          assert();
          zero ();
        }
          if ((*necessary_memory)[27]==-1)
        {
          delete ;
          = 0;
          if ((*necessary_memory)[28]==1)
        {
          = new ;
          assert();
          zero ();
        }
          if ((*necessary_memory)[28]==-1)
        {
          delete ;
          = 0;
          if ((*necessary_memory)[29]==1)
        {
          = new ;
          assert();
          zero ();
        }
          if ((*necessary_memory)[29]==-1)
        {
          delete ;
          = 0;
        }
    */
    // new real memory after allocations:
    *real_memory |= *necessary_memory;
    // prepare necessary_memory for next call of memory_manager():
    zero(necessary_memory);

    // destroy network:
    if (end_call) {
        delete layer[0].outputs;
        delete layer;
        delete real_memory;
        delete necessary_memory;
        delete net_memory;
    }
}

}
