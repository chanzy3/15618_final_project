//
// Created by Tiane Zhu on 11/25/20.
//

#include <memory>
#include <queue>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "solver.h"



const char *to_string(int operation) {
  switch(operation) {
    case 0:
      return "F1";
    case 1:
      return "B1";
    case 2:
      return "L1";
    case 3:
      return "R1";
    case 4:
      return "U1";
    case 5:
      return "D1";
    case 6:
      return "F2";
    case 7:
      return "B2";
    case 8:
      return "L2";
    case 9:
      return "R2";
    case 10:
      return "U2";
    case 11:
      return "D2";
    case 12:
      return "F3";
    case 13:
      return "B3";
    case 14:
      return "L3";
    case 15:
      return "R3";
    case 16:
      return "U3";
    case 17:
      return "D3";
    default:
      return "??";
  }
}

// search node
typedef struct node {
  cube_t *cube;
  int steps[MAX_DEPTH];
  int d;
} node_t;

node_t *node_new() {
  node_t *node = (node_t *) malloc(sizeof(node_t));
  if (node == NULL) {
    perror("malloc node");
    exit(1);
  }

  node->cube = cube_new(false);
  node->d = 0;

  return node;
}



node_t *node_cpy(node_t *node) {
  node_t *ret = node_new();
  memcpy(ret->cube, node->cube, sizeof(cube_t));
  memcpy(ret->steps, node->steps, MAX_DEPTH * sizeof(int));
  ret->d = node->d;
  return ret;
}

void node_destroy(node_t *node) {
  free(node->cube);
  free(node);
}

bool bfs_solve_internal(cube_t *cube, int solution[MAX_DEPTH], int *num_steps, std::queue<node_t *> q[2]) {

  node_t *root = node_new();
  memcpy(root->cube, cube, sizeof(cube_t));

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

bool bfs_solve(cube_t *cube, int solution[MAX_DEPTH], int *num_steps) {
  bool ret;
  std::queue<node_t *> q[2];
  ret = bfs_solve_internal(cube, solution, num_steps, q);

  for (int i=0; i<2; i++) {
    while (q[i].size() != 0) {
      node_destroy(q[i].front());
      q[i].pop();
    }
  }
  //ppp(cube);
  return ret;
}


bool ida_solve(cube_t *cube, int solution[MAX_DEPTH], int *num_steps) {

}