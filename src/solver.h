//
// Created by Tiane Zhu on 11/25/20.
//

#ifndef INC_15618_FINAL_PROJECT_SOLVER_H
#define INC_15618_FINAL_PROJECT_SOLVER_H

#include <stdint.h>

#include "cube.h"
#include "debug.h"
#include "database/PatternDatabase.h"
#include "database/Korf/CornerPatternDatabase.h"

enum method {
  BFS = 0,
  IDA_SEQ = 1,
  IDA_OMP = 2
};

class Solver {
public:
  Solver(enum method method) : method(method), corner_db() {
    switch (method) {
      case IDA_SEQ:
      case IDA_OMP:
        ida_init();
        break;
      default:
        break;
    }
  }

  ~Solver() {
    ida_destroy();
  }
#define MAX_DEPTH 26 // TODO(tianez):
#define DEPTH_LIMIT 6 // must be smaller than MAX_DEPTH

  bool solve(cube_t *cube, int solution[MAX_DEPTH], int *num_steps);

  // operation 0-17 (inclusive);
  static const char *to_string(int operation);

private:
  enum method method;

  bool bfs_solve(cube_t *cube, int solution[MAX_DEPTH], int *num_steps);

  void ida_init();
  void ida_destroy();
  bool ida_solve_seq(cube_t *cube, int solution[MAX_DEPTH], int *num_steps);
  bool ida_solve_omp(cube_t *cube, int solution[MAX_DEPTH], int *num_steps);

  paracube::CornerPatternDatabase corner_db;
};

#endif //INC_15618_FINAL_PROJECT_SOLVER_H
