//
// Created by Tiane Zhu on 11/30/20.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "solver_ida_omp.h"

#define EX_PRINTF(format, ...) { \
  omp_set_lock(&printlock); \
  printf(format, ##__VA_ARGS__); \
  omp_unset_lock(&printlock); \
}

bool SolverIdaOmp::solve(cube_t *cube, int *solution, int *num_steps) {
  return ida_solve_omp(cube, solution, num_steps);
}

uint8_t iteration_t;
int depth;

bool SolverIdaOmp::ida_solve_omp(cube_t *cube, int solution[MAX_DEPTH], int *num_steps) {
  // TODO(tianez): trade extra computation to save memory:
  // redefine path into a list of ints (ops) and save only 1 cube:
  // likely not needed since IDA is memory constrained

  node_t *root = node_new_from_cube(cube);

  node_t *path[MAX_DEPTH];
  int d;
  uint8_t bound;

  path[0] = root;
  d = 1;
  bound = h(&this->corner_db, root, d);
  DBG_PRINTF("initial bound %d\n", bound);
  while (1) {
#pragma omp parallel
    {
#pragma omp single
      {
        fprintf(stdout, "Num omp threads: %d\n", omp_get_num_threads());
        search_omp_para(&this->corner_db, path, &d, 0, bound);
      }
      // TODO(tianez): assumed implicit barrier for all threads
    }

    DBG_PRINTF("iteration_t: %d\n\n", iteration_t);
    printf("iteration_t: %d\n", iteration_t);
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

void SolverIdaOmp::search_omp_para(paracube::CornerPatternDatabase *corner_db, node_t *path[MAX_DEPTH], int *d, uint8_t g, uint8_t bound) {
  node_t *node = path[(*d) - 1];
  uint8_t f = g + h(corner_db, node, (*d) - 1);
  DBG_PRINTF("search_omp_para h %d\n", f - g);

#ifdef PRINT_PATH
  omp_set_lock(&printlock);

  printf("%hhu+%hhu=%hhu/%hhu next_cost %hhu para: ", g, f-g, f, bound, g + cost(node, node));
  for (int i=0; i<node->d; i++) {
    printf("%s ", Solver::to_string(node->steps[i]));
  }
  printf("\n");

  omp_unset_lock(&printlock);
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

#pragma omp task firstprivate(op, private_path, private_d, bound, g) shared(node)
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

      uint8_t t = search_omp(corner_db, private_path, private_d, g + cost(node, n), bound);

#pragma omp critical
      {
        if (t == FOUND) {
          iteration_t = t; // TODO(tianez): prob don't need this, as we have FOUND being the smallest possible t

          memcpy(path, private_path, MAX_DEPTH * sizeof(node_t *));
          memcpy(d, private_d, sizeof(int));
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

uint8_t SolverIdaOmp::search_omp(paracube::CornerPatternDatabase *corner_db, node_t *path[MAX_DEPTH], int *d, uint8_t g, uint8_t bound) {
  node_t *node = path[(*d) - 1];
  uint8_t f = g + h(corner_db, node, (*d) - 1);
  DBG_PRINTF("search_omp h %d\n", f - g);

#ifdef PRINT_PATH
  omp_set_lock(&printlock);

  printf("%hhu+%hhu=%hhu/%hhu next_cost %hhu norm: ", g, f-g, f, bound, g + cost(node, node));
  for (int i=0; i<node->d; i++) {
    printf("%s ", Solver::to_string(node->steps[i]));
  }
  if (test_converge(node->cube)) {
    printf("converged");
  }
  printf("\n");

  omp_unset_lock(&printlock);
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
