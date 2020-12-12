//
// Created by Tiane Zhu on 12/11/20.
//

#include "solver_ida_iter_omp_base.h"

#ifdef COUNT_TRANSITIONS
int ida_iter_omp_num_transitions = 0;
int ida_iter_omp_num_transitions_top_level = 0;
#endif

int curr_iter_mean = INFTY; // atomic update

bool found = false; // atomic update
