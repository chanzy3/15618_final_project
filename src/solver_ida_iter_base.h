//
// Created by Tiane Zhu on 12/10/20.
//

#ifndef INC_15618_FINAL_PROJECT_SOLVER_IDA_ITER_BASE_H
#define INC_15618_FINAL_PROJECT_SOLVER_IDA_ITER_BASE_H

#include "solver_ida_base.h"

// definition of a problem to solve
struct node_iter {
  cube_t* cube;
  int op; // next op
  int g;
  int d;
  int min; // current level min
};

typedef node_iter node_iter_t;

class SolverIdaIterBase : public SolverIdaBase {

public:
  SolverIdaIterBase() : SolverIdaBase() {
  }

protected:
  node_iter_t *node_iter_new();
  node_iter_t *node_iter_new_from_cube(cube_t* cube);
  node_iter_t *node_iter_cpy(node_iter_t *node);
  void node_iter_destroy(node_iter_t *node);

  uint8_t h(paracube::CornerPatternDatabase *corner_db, node_iter_t *node);
  uint8_t cost(node_iter_t *n1, node_iter_t *n2);
};

#endif //INC_15618_FINAL_PROJECT_SOLVER_IDA_ITER_BASE_H
