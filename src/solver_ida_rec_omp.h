//
// Created by Tiane Zhu on 11/30/20.
//

#ifndef INC_15618_FINAL_PROJECT_IDA_REC_OMP_SOLVER_H
#define INC_15618_FINAL_PROJECT_IDA_REC_OMP_SOLVER_H

#include <omp.h>

#include "solver_ida_base.h"

class SolverIdaRecOmp : public SolverIdaBase {

public:

  SolverIdaRecOmp(int num_threads) : SolverIdaBase() {
    omp_set_num_threads(num_threads);
    omp_init_lock(&printlock);
  }

  bool solve(cube_t *cube, int solution[MAX_DEPTH], int *num_steps); // override;

private:

  bool ida_solve_omp(cube_t *cube, int solution[MAX_DEPTH], int *num_steps);

  void search_omp_para(paracube::CornerPatternDatabase *corner_db, node_t *path[MAX_DEPTH], int *d, uint8_t g, uint8_t bound);
  uint8_t search_omp(paracube::CornerPatternDatabase *corner_db, node_t *path[MAX_DEPTH], int *d, uint8_t g, uint8_t bound);

  omp_lock_t printlock;
};

#endif //INC_15618_FINAL_PROJECT_IDA_REC_OMP_SOLVER_H
