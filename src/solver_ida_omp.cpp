//
// Created by Tiane Zhu on 11/30/20.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "solver_ida_omp.h"

bool SolverIdaOmp::solve(cube_t *cube, int *solution, int *num_steps) {
  return ida_solve_omp(cube, solution, num_steps);
}

int iteration_t;
int depth;

bool SolverIdaOmp::ida_solve_omp(cube_t *cube, int solution[MAX_DEPTH], int *num_steps) {
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
#pragma omp parallel
    {
#pragma omp single
      {
        search_omp_para(&this->corner_db, path, &d, 0, bound);
      }
      // TODO(tianez): assumed implicit barrier for all threads
    }

    DBG_PRINTF("t: %d\n\n", t);
    if (iteration_t == FOUND) {
      node_t *n = path[d - 1];
      *num_steps = n->d;
      memcpy(solution, n->steps, MAX_DEPTH * sizeof(int));
      for (int i = 0; i < MAX_DEPTH; i++) {
        DBG_PRINTF("%d ", solution[i]);
      }
      DBG_PRINTF("\n");
      return true;
    }
    if (iteration_t == INFTY) {
      return false;
    }
    bound = iteration_t;
  }

  // TODO(tianez): free entire path from 0 to d (inc)

  return false;
}

bool found = false;

void SolverIdaOmp::search_omp_para(paracube::CornerPatternDatabase *corner_db, node_t *path[MAX_DEPTH], int *d, int g, int bound) {
  node_t *node = path[(*d) - 1];
  int f = g + h(corner_db, node, (*d) - 1);
  DBG_PRINTF("search_omp_para h %d\n", f - g);

#ifdef PRINT_PATH
  for (int i=0; i<node->d; i++) {
    printf("%s ", Solver::to_string(node->steps[i]));
  }
  printf("\n");
#endif

  if (f > bound) {
    iteration_t = f;
    return;
    // return f;
  }

  if (test_converge(node->cube)) {
    iteration_t = FOUND;
    return;
    // return FOUND;
  }

  for(int op=0; op<TRANSITION_COUNT; op++) {
    node_t **private_path;
    int *private_d;

    private_path = (node_t **) malloc(MAX_DEPTH * sizeof(node_t *));
    memcpy(private_path, path, MAX_DEPTH * sizeof(node_t *)); // TODO(tianez): can be *d * private_path
    private_d = (int *) malloc(sizeof(int));

#pragma omp task firstprivate(op, private_path, private_d) shared(node, g, bound)
    {
      node_t *n = node_cpy(node);
      cube_t *c = n->cube;

      transition[op](c); // succ->cube

      /*
      DBG_PRINTF("!!! %s\n", Solver::to_string(op));
      for (int i=0; i<node->d; i++) {
        DBG_PRINTF("%s ", Solver::to_string(node->steps[i]));
      }
      DBG_PRINTF("\n");
      DBG_PRINTF("=========\n");
      ppp(node->cube);
      ppp(c);
      DBG_PRINTF("=========\n");
       */

      n->steps[n->d] = op;
      n->d++;

      // path.push(succ)
      private_path[*private_d] = n;
      (*private_d)++;

      int t = search_omp(corner_db, private_path, private_d, g + cost(node, n), bound);

#pragma omp critical
      {
        if (t == FOUND) {
          iteration_t = t; // TODO(tianez): prob don't need this, as we have FOUND being the smallest possible t
        }

        if (t < iteration_t) {
          iteration_t = t;
        }
      }

      // path.pop()
      (*private_d)--;

      free(c);
      free(n);
      free(private_path);
      free(private_d);
    }
  }
}

int SolverIdaOmp::search_omp(paracube::CornerPatternDatabase *corner_db, node_t *path[MAX_DEPTH], int *d, int g, int bound) {
  node_t *node = path[(*d) - 1];
  int f = g + h(corner_db, node, (*d) - 1);
  DBG_PRINTF("search_omp h %d\n", f - g);

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

    transition[op](c); // succ->cube

    /*
    DBG_PRINTF("!!! %s\n", Solver::to_string(op));
    for (int i=0; i<node->d; i++) {
      DBG_PRINTF("%s ", Solver::to_string(node->steps[i]));
    }
    DBG_PRINTF("\n");
    DBG_PRINTF("=========\n");
    ppp(node->cube);
    ppp(c);
    DBG_PRINTF("=========\n");
     */

    n->steps[n->d] = op;
    n->d++;

    // path.push(succ)
    path[*d] = n;
    (*d)++;

    int t = search_omp(corner_db, path, d, g + cost(node, n), bound);

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
