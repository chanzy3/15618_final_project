//
// Created by Tiane Zhu on 11/30/20.
//

#ifndef INC_15618_FINAL_PROJECT_IDA_OMP_SOLVER_H
#define INC_15618_FINAL_PROJECT_IDA_OMP_SOLVER_H

#include <omp.h>

#include "solver_ida_base.h"

class SolverIdaOmp : public SolverIdaBase {

public:

  SolverIdaOmp(int num_threads) : SolverIdaBase() {
    omp_set_num_threads(num_threads);
    fprintf(stdout, "Num omp threads: %d\n", omp_get_num_threads());
  }

  bool solve(cube_t *cube, int solution[MAX_DEPTH], int *num_steps) override;

private:

  bool ida_solve_omp(cube_t *cube, int solution[MAX_DEPTH], int *num_steps);

  void search_omp_para(paracube::CornerPatternDatabase *corner_db, node_t *path[MAX_DEPTH], int *d, int g, int bound);
  int search_omp(paracube::CornerPatternDatabase *corner_db, node_t *path[MAX_DEPTH], int *d, int g, int bound);
};

#endif //INC_15618_FINAL_PROJECT_IDA_OMP_SOLVER_H
