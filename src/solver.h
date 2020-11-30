//
// Created by Tiane Zhu on 11/25/20.
//

#ifndef INC_15618_FINAL_PROJECT_SOLVER_H
#define INC_15618_FINAL_PROJECT_SOLVER_H

#include <stdint.h>

#include "cube.h"
#include "debug.h"

enum method {
  BFS = 0,
  IDA_SEQ = 1,
  IDA_OMP = 2
};

#define MAX_DEPTH 26 // TODO(tianez):

class Solver {

public:
  Solver() {
  }

  bool timedSolve(cube_t *cube);
  virtual bool solve(cube_t *cube, int solution[MAX_DEPTH], int *num_steps) = 0;

  // operation 0-17 (inclusive);
  static const char *to_string(int operation);

protected:
#define TRANSITION_COUNT 18
// transition functions
  void (*transition[TRANSITION_COUNT])(cube_t *) =
      {
          F1, B1, L1, R1, U1, D1,
          F2, B2, L2, R2, U2, D2,
          F3, B3, L3, R3, U3, D3,
      };
};

// search node
typedef struct node {
  cube_t *cube;
  int steps[MAX_DEPTH];
  int d;
} node_t;

node_t *node_new();
node_t *node_new_from_cube(cube_t* cube);
node_t *node_cpy(node_t *node);
void node_destroy(node_t *node);

#endif //INC_15618_FINAL_PROJECT_SOLVER_H
