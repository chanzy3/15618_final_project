//
// Created by Tiane Zhu on 11/30/20.
//

#ifndef INC_15618_FINAL_PROJECT_SOLVER_BFS_H
#define INC_15618_FINAL_PROJECT_SOLVER_BFS_H

#include <queue>

#include "solver.h"

#define DEPTH_LIMIT 6 // must be smaller than MAX_DEPTH

class SolverBfs : public Solver {

public:

  SolverBfs() : Solver() {

  }

  bool solve(cube_t *cube, int solution[MAX_DEPTH], int *num_steps); // override;

private:

  bool bfs_solve(cube_t *cube, int solution[MAX_DEPTH], int *num_steps);
  bool bfs_solve_internal(cube_t *cube, int solution[MAX_DEPTH], int *num_steps, std::queue<node_t *> q[2]);

};

#endif //INC_15618_FINAL_PROJECT_SOLVER_BFS_H
