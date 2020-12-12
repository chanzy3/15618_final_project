//
// Created by Tiane Zhu on 12/11/20.
//

#ifndef INC_15618_FINAL_PROJECT_SOLVER_IDA_ITER_OMP_MAIN_WORKER_H
#define INC_15618_FINAL_PROJECT_SOLVER_IDA_ITER_OMP_MAIN_WORKER_H

#include "solver_ida_iter_omp_base.h"

class SolverIdaIterOmpMainWorker : public SolverIdaIterOmpBase {

public:

  SolverIdaIterOmpMainWorker(int num_threads) : SolverIdaIterOmpBase(num_threads) {

  }

  bool solve(cube_t *cube, int solution[MAX_DEPTH], int *num_steps); // override;

private:

  bool ida_solve_iter_omp_main_worker(cube_t *cube, int solution[MAX_DEPTH], int *num_steps);

  void search_iter_omp_main_worker(paracube::CornerPatternDatabase *corner_db, node_iter_t path[MAX_DEPTH], int *solution_length, int bound);
  int search_iter_omp_main_worker_helper(paracube::CornerPatternDatabase *corner_db, node_iter_t path[MAX_DEPTH], int bound, int starting_depth);
};

#endif //INC_15618_FINAL_PROJECT_SOLVER_IDA_ITER_OMP_MAIN_WORKER_H
