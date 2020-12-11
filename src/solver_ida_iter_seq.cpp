//
// Created by Tiane Zhu on 12/10/20.
//

#include "solver_ida_iter_seq.h"

#define MIN(a, b) a < b ? a : b;

#ifdef PRINT_PATH
#define PPATH(p, d) { \
  for (int i=0; i<d; i++) { \
    printf("%s ", Solver::to_string(p[i]->op - 1)); \
  } \
  printf("\n"); \
}
#else
#define PPATH(p, d)
#endif

#ifdef PRINT_PATH
#define PWPATH(p, pd) { \
  for (int i=0; i<pd; i++) { \
    printf("(_, %d, %d, %d, %s) ", p[i]->g, p[i]->d, p[i]->min, Solver::to_string(p[i]->op - 1)); \
  } \
  printf("\n"); \
}
#else
#define PWPATH(p, d)
#endif

bool SolverIdaIterSeq::solve(cube_t *cube, int *solution, int *num_steps) {
  return ida_solve_iter_seq(cube, solution, num_steps);
}

bool SolverIdaIterSeq::ida_solve_iter_seq(cube_t *cube, int solution[MAX_DEPTH], int *num_steps) {
  // TODO(tianez): trade extra computation to save memory:
  // redefine path into a list of ints (ops) and save only 1 cube:
  // likely not needed since IDA is memory constrained

  node_iter_t *root = node_iter_new_from_cube(cube);

  node_iter_t *path[MAX_DEPTH];
  int bound;

  path[0] = root;
  bound = h(&this->corner_db, root);
  DBG_PRINTF("initial bound %d\n", bound);

  while (1) {
    // reset problem
    root->g = 0;
    root->d = 0;
    root->op = 0;
    root->min = INFTY;

    int solution_length = search_iter_seq(&this->corner_db, path, bound);

    DBG_PRINTF("t: %d\n\n", t);
    if (solution_length >= 0) {
      *num_steps = solution_length;
      int s;
      for (s = 0; s < solution_length; s++) {
        solution[s] = (path[s]->op) - 1;
      }

      return true;
    }

    int t = root->min;

    if (t == INFTY) {
      return false;
    }
    bound = t;
  }

  // TODO(tianez): free entire path from 0 to d (inc)

  return false;
}

int SolverIdaIterSeq::search_iter_seq(paracube::CornerPatternDatabase *corner_db, node_iter_t *path[MAX_DEPTH], int bound) {
  node_iter_t *root = path[0];
  int path_d = 1; // start from index 1

  if (test_converge(root->cube)) {
    return FOUND;
  }

  // TODO(tianez): correct cond?
  while (1) {
    node_iter_t *n_curr = path[path_d - 1]; // curr node

    PPATH(path, path_d);
    PWPATH(path, path_d);

    if (n_curr->op >= TRANSITION_COUNT) {
      // finished all ops

      if (path_d == 1) {
        // TODO(tianez):
        return -1; // don't free root
      } else {
        node_iter_t *n_prev = path[path_d - 2];

        n_prev->min = MIN(n_prev->min, n_curr->min);

        node_iter_destroy(n_curr); // stack.pop
        path[path_d - 1] = NULL;
        path_d--;

        continue; // resume work on previous problem, current problem complete
      }
    } else {
      node_iter_t *n_next = node_iter_cpy(n_curr);

      // generate next problem:
      // input: cube, g, d
      // problem internal state: min, op (iterate from 0 - 17)

      // cube
      transition[n_curr->op](n_next->cube);
      (n_curr->op)++; // go to next op
      // g
      n_next->g += cost(n_next, n_curr);
      // d
      n_next->d++;
      // min
      n_next->min = INFTY; // TODO(tianez): need this? is this optimization?
      // op
      n_next->op = 0; // start from first op

      // Keep next problem or not ? depending on f and convergence, choices:
      // 1. convergence:  return found
      // 2. f > bound:    discard, update current problem min
      // 3. f <= bound:   keep, next iteration will work on new problem
      int f = n_next->g + h(corner_db, n_next);

      if (test_converge(n_next->cube)) {
        node_iter_destroy(n_next);

        n_curr->min = FOUND;
        return path_d; // return found (solution_length)
      }

      if (f > bound) { // discard
        n_curr->min = MIN(n_curr->min, f);
        node_iter_destroy(n_next);
      } else {
        path[path_d] = n_next; // stack.push
        path_d++;
      }
    }
  }

  /*
  for(int op=0; op<TRANSITION_COUNT; op++) {
    node_t *n = node_cpy(node);
    cube_t *c = n->cube;

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

  // TODO(tianez): remove, now at root->min
  // return min;
  */
}

