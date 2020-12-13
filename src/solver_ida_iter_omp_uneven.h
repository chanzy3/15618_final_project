//
// Created by Tiane Zhu on 12/11/20.
//

#ifndef INC_15618_FINAL_PROJECT_SOLVER_IDA_ITER_OMP_UNEVEN_H
#define INC_15618_FINAL_PROJECT_SOLVER_IDA_ITER_OMP_UNEVEN_H

#include "solver_ida_iter_omp_base.h"

class SolverIdaIterOmpUneven : public SolverIdaIterOmpBase {

public:

  SolverIdaIterOmpUneven(int num_threads) : SolverIdaIterOmpBase(num_threads) {
    this->num_threads = num_threads;
    worker_states = (WorkerState **) malloc(num_threads * sizeof(WorkerState *));

    omp_init_lock(&solution_lock);
  }

  ~SolverIdaIterOmpUneven() {
    omp_destroy_lock(&solution_lock);

    for (int i=0; i<num_threads; i++) {
      omp_destroy_lock(&(worker_states[i]->lock));
      free(worker_states[i]);
    }
    free(worker_states);
  }

  bool solve(cube_t *cube, int solution[MAX_DEPTH], int *num_steps); // override;

private:

  bool ida_solve_iter_omp_uneven(cube_t *cube, int solution[MAX_DEPTH], int *num_steps);

  void search_iter_omp_uneven(paracube::CornerPatternDatabase *corner_db, node_iter_t path[MAX_DEPTH], int *solution_length, int bound);
  int search_iter_omp_uneven_helper(paracube::CornerPatternDatabase *corner_db,
                                    node_iter_t path[MAX_DEPTH], int *solution_length,
                                    node_iter_t *local_path, int *starting_depth, int *local_path_d,
                                    omp_lock_t *local_lock,
                                    node_iter_t *private_local_path, int *private_starting_depth, int bound);
  int search_iter_omp_uneven_helper_private(paracube::CornerPatternDatabase *corner_db, node_iter_t path[MAX_DEPTH], int bound, int starting_depth);

  enum WorkingState {
    Waiting = 0,
    Working = 1,
    Finished = 2
  };
  struct WorkerState {
    enum WorkingState state;
    int starting_depth;
    node_iter_t local_path[MAX_DEPTH];
    int local_path_d;
    omp_lock_t lock;
  };
  int num_threads;
  WorkerState **worker_states;

  omp_lock_t solution_lock;
};

#endif //INC_15618_FINAL_PROJECT_SOLVER_IDA_ITER_OMP_UNEVEN_H
