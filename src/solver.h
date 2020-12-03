//
// Created by Tiane Zhu on 11/25/20.
//

#ifndef INC_15618_FINAL_PROJECT_SOLVER_H
#define INC_15618_FINAL_PROJECT_SOLVER_H

#include "cube.h"
#include "debug.h"
#include "database/PatternDatabase.h"
#include "database/Korf/CornerPatternDatabase.h"
#include <omp.h>
#include <mpi.h>

enum method {
  BFS = 0,
  IDA = 1,
  IDA_PARA = 2
};

#define UPDATE_TAG 1
#define ALLOCATE_TAG 2
#define ABORT_TAG 3
#define START_TAG 4
#define BROADCAST_TAG 5

class Solver {
public:


  Solver(enum method method) : method(method), corner_db() {
    switch (method) {
      case IDA:
        ida_init();
        break;
      case IDA_PARA:
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

typedef struct {
  int bound;
  int depth;
  int branch_id;
  int solution[MAX_DEPTH];
  
} ans_t;

  bool solve(cube_t *cube, int solution[MAX_DEPTH], int *num_steps);

  // operation 0-17 (inclusive);
  static const char *to_string(int operation);

private:
  enum method method;



  bool bfs_solve(cube_t *cube, int solution[MAX_DEPTH], int *num_steps);

  void ida_init();
  void ida_destroy();
  bool ida_solve(cube_t *cube, int solution[MAX_DEPTH], int *num_steps);
  bool ida_solve_mpi(cube_t *cube, int solution[MAX_DEPTH], int *num_steps);
  // bool ida_para_helper(cube_t *cube, int solution[MAX_DEPTH], int *num_steps, bool &t, node_t **path, CubeSet &cubes);

  paracube::CornerPatternDatabase corner_db;
};

#endif //INC_15618_FINAL_PROJECT_SOLVER_H
