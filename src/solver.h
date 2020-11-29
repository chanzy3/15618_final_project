//
// Created by Tiane Zhu on 11/25/20.
//

#ifndef INC_15618_FINAL_PROJECT_SOLVER_H
#define INC_15618_FINAL_PROJECT_SOLVER_H

#include "cube.h"
#include "debug.h"

#define TRANSITION_COUNT 18
// transition functions
void (*transition[TRANSITION_COUNT])(cube_t *) =
    {
        F1, B1, L1, R1, U1, D1,
        F2, B2, L2, R2, U2, D2,
        F3, B3, L3, R3, U3, D3,
    };

#define MAX_DEPTH 26 // TODO(tianez):
#define DEPTH_LIMIT 4 // must be smaller than MAX_DEPTH

bool bfs_solve(cube_t *cube, int solution[MAX_DEPTH], int *num_steps);
bool ida_solve(cube_t *cube, int solution[MAX_DEPTH], int *num_steps);

// operation 0-17 (inclusive);
const char *to_string(int operation);

#endif //INC_15618_FINAL_PROJECT_SOLVER_H
