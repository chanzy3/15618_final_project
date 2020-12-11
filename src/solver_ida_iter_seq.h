//
// Created by Tiane Zhu on 12/10/20.
//

#ifndef INC_15618_FINAL_PROJECT_SOLVER_IDA_SEQ_ITER_H
#define INC_15618_FINAL_PROJECT_SOLVER_IDA_SEQ_ITER_H

#include "solver_ida_iter_base.h"

class SolverIdaIterSeq : public SolverIdaIterBase {

public:

  SolverIdaIterSeq() : SolverIdaIterBase() {

  }

  bool solve(cube_t *cube, int solution[MAX_DEPTH], int *num_steps); // override;

private:

  bool ida_solve_iter_seq(cube_t *cube, int solution[MAX_DEPTH], int *num_steps);

  int search_iter_seq(paracube::CornerPatternDatabase *corner_db, node_iter_t *path[MAX_DEPTH], int bound);
};

#endif //INC_15618_FINAL_PROJECT_SOLVER_IDA_SEQ_ITER_H
