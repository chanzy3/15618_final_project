//
// Created by Tiane Zhu on 11/25/20.
//

#ifndef INC_15618_FINAL_PROJECT_SOLVER_H
#define INC_15618_FINAL_PROJECT_SOLVER_H

#include "cube.h"

#define MAX_DEPTH 26 // TODO(tianez):
#define DEPTH_LIMIT 4 // must be smaller than MAX_DEPTH

bool bfs_solve(cube_t *cube, int solution[MAX_DEPTH], int *num_steps);

// operation 0-17 (inclusive);
const char *to_string(int operation);

#endif //INC_15618_FINAL_PROJECT_SOLVER_H
