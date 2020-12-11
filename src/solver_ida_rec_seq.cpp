//
// Created by Tiane Zhu on 11/30/20.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "solver_ida_rec_seq.h"

int gggg = 0;

bool SolverIdaRecSeq::solve(cube_t *cube, int *solution, int *num_steps) {
  return ida_solve_seq(cube, solution, num_steps);
}

bool SolverIdaRecSeq::ida_solve_seq(cube_t *cube, int solution[MAX_DEPTH], int *num_steps) {
  // TODO(tianez): trade extra computation to save memory:
  // redefine path into a list of ints (ops) and save only 1 cube:
  // likely not needed since IDA is memory constrained

  node_t *root = node_new_from_cube(cube);

  node_t *path[MAX_DEPTH];
  int d;
  int bound;

  path[0] = root;
  d = 1;
  bound = h(&this->corner_db, root, d);
  DBG_PRINTF("initial bound %d\n", bound);

  while (1) {
    int t = search_seq(&this->corner_db, path, &d, 0, bound);
    DBG_PRINTF("t: %d\n\n", t);
    if (t == FOUND) {
      node_t *n = path[d - 1];
      *num_steps = n->d;
      memcpy(solution, n->steps, MAX_DEPTH * sizeof(int));
      for (int i=0; i<MAX_DEPTH; i++) {
        DBG_PRINTF("%d ", solution[i]);
      }
      DBG_PRINTF("\n");

      printf("transitions: %d\n", gggg);
      return true;
    }
    if (t == INFTY) {
      return false;
    }
    bound = t;
  }

  // TODO(tianez): free entire path from 0 to d (inc)

  return false;
}

int SolverIdaRecSeq::search_seq(paracube::CornerPatternDatabase *corner_db, node_t *path[MAX_DEPTH], int *d, int g, int bound) {
  node_t *node = path[(*d) - 1];
  int f = g + h(corner_db, node, (*d) - 1);
  DBG_PRINTF("search_seq h %d\n", f - g);

#ifdef PRINT_PATH
  for (int i=0; i<node->d; i++) {
    printf("%s ", Solver::to_string(node->steps[i]));
  }
  printf("\n");
#endif


  if (f > bound) {
    return f;
  }

  if (test_converge(node->cube)) {
    return FOUND;
  }

  int min = INFTY;

  for(int op=0; op<TRANSITION_COUNT; op++) {
    node_t *n = node_cpy(node);
    cube_t *c = n->cube;

    gggg++;
    transition[op](c); // succ->cube

    DBG_PRINTF("!!! %s\n", Solver::to_string(op));
    for (int i=0; i<node->d; i++) {
      DBG_PRINTF("%s ", Solver::to_string(node->steps[i]));
    }
    DBG_PRINTF("\n");
    DBG_PRINTF("=========\n");
    ppp(node->cube);
    ppp(c);
    DBG_PRINTF("=========\n");

    n->steps[n->d] = op;
    n->d++;

    // path.push(succ)
    path[*d] = n;
    (*d)++;

    int t = search_seq(corner_db, path, d, g + cost(node, n), bound);

    if (t == FOUND) {
      return FOUND;
    }

    if (t < min) {
      min = t;
    }

    // path.pop()
    (*d)--;

    free(c);
    free(n);
  }

  return min;
}

