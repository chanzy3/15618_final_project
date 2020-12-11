//
// Created by Tiane Zhu on 12/10/20.
//

#ifndef INC_15618_FINAL_PROJECT_SOLVER_IDA_ITER_BASE_H
#define INC_15618_FINAL_PROJECT_SOLVER_IDA_ITER_BASE_H

#include "solver_ida_base.h"

#ifdef PRINT_PATH
#define PPATH(p, d) { \
  for (int i=0; i<d; i++) { \
    printf("%s ", Solver::to_string(p[i].op - 1)); \
  } \
  printf("\n"); \
}
#else
#define PPATH(p, d)
#endif

#ifdef PRINT_PATH
#define PWPATH(p, pd) { \
  for (int i=0; i<pd; i++) { \
    printf("(_, %d, %d, %d, %s) ", p[i]->g, p[i]->d, p[i]->min, Solver::to_string(p[i]->op - 1)); \
  } \
  printf("\n"); \
}
#else
#define PWPATH(p, d)
#endif

// definition of a problem to solve
struct node_iter {
  cube_t cube;
  /*
  int op; // next op
  int g;
  int d;
  int min; // current level min
   */
  uint8_t op; // next op
  uint8_t g;
  uint8_t d;
  uint8_t min; // current level min
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

  void node_iter_init(node_iter_t *node);
  void node_iter_init_from_cube(node_iter_t*node, cube_t *cube);
  void node_iter_cpy(node_iter_t *dst, node_iter_t *src);

  uint8_t h(paracube::CornerPatternDatabase *corner_db, node_iter_t *node);
  uint8_t cost(node_iter_t *n1, node_iter_t *n2);
};

#endif //INC_15618_FINAL_PROJECT_SOLVER_IDA_ITER_BASE_H
