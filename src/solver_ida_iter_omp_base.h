//
// Created by Tiane Zhu on 12/11/20.
//

#ifndef INC_15618_FINAL_PROJECT_SOLVER_IDA_BASE_OMP_H
#define INC_15618_FINAL_PROJECT_SOLVER_IDA_BASE_OMP_H

#include <omp.h>

#include "solver_ida_iter_base.h"

// #define COUNT_TRANSITIONS

#define PRUNE

#define DEPTH_LIMIT_MIN 2
#define TASK_BOUND_TARGET 9

#define OMP_PRINTF(format, ...) { \
  omp_set_lock(&printlock); \
  printf(format, ##__VA_ARGS__); \
  omp_unset_lock(&printlock); \
}

#define OMP_DBG_PRINTF(format, ...) { \
  omp_set_lock(&printlock); \
  DBG_PRINTF(format, ##__VA_ARGS__); \
  omp_unset_lock(&printlock); \
}

class SolverIdaIterOmpBase : public SolverIdaIterBase {

public:

  SolverIdaIterOmpBase(int num_threads) : SolverIdaIterBase() {
    omp_set_num_threads(num_threads);
    omp_init_lock(&printlock);
  }

protected:
  omp_lock_t printlock;
};

#endif //INC_15618_FINAL_PROJECT_SOLVER_IDA_BASE_OMP_H
