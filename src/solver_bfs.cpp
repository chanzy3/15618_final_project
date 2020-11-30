//
// Created by Tiane Zhu on 11/30/20.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "solver_bfs.h"

bool SolverBfs::bfs_solve_internal(cube_t *cube, int solution[MAX_DEPTH], int *num_steps, std::queue<node_t *> q[2]) {

  node_t *root = node_new_from_cube(cube);

  q[0].push(root);

  for (int iter=0; iter<DEPTH_LIMIT; iter++) {
    std::queue<node_t *> &from = q[iter%2];
    std::queue<node_t *> &to = q[(iter+1) % 2];

    node_t *n_old, *n;
    while (from.size() > 0) {
      n_old = from.front();
      from.pop();
      for (int op=0; op<TRANSITION_COUNT; op++) {
        n = node_cpy(n_old);

        n->steps[n->d] = op;
        n->d++;
        transition[op](n->cube);
        if (test_converge(n->cube)) {
          *num_steps = n->d;
          memcpy(solution, n->steps, MAX_DEPTH * sizeof(int));
          return true;
        }
        to.push(n);
      }

      node_destroy(n_old);
    }
  }

  return false;
}

bool SolverBfs::bfs_solve(cube_t *cube, int solution[MAX_DEPTH], int *num_steps) {
  bool ret;
  std::queue<node_t *> q[2];
  ret = bfs_solve_internal(cube, solution, num_steps, q);

  for (int i=0; i<2; i++) {
    while (q[i].size() != 0) {
      node_destroy(q[i].front());
      q[i].pop();
    }
  }

  return ret;
}

bool SolverBfs::solve(cube_t *cube, int solution[MAX_DEPTH], int *num_steps) {
  return bfs_solve(cube, solution, num_steps);
}

