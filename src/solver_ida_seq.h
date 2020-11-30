//
// Created by Tiane Zhu on 11/30/20.
//

#ifndef INC_15618_FINAL_PROJECT_SOLVER_IDA_SEQ_H
#define INC_15618_FINAL_PROJECT_SOLVER_IDA_SEQ_H

#include "solver_ida_base.h"

class SolverIdaSeq : public SolverIdaBase {

public:

  SolverIdaSeq() : SolverIdaBase() {

  }

  bool solve(cube_t *cube, int solution[MAX_DEPTH], int *num_steps) override;

private:

  bool ida_solve_seq(cube_t *cube, int solution[MAX_DEPTH], int *num_steps);

  int search_seq(paracube::CornerPatternDatabase *corner_db, node_t *path[MAX_DEPTH], int *d, int g, int bound);
};

#endif //INC_15618_FINAL_PROJECT_SOLVER_IDA_SEQ_H
