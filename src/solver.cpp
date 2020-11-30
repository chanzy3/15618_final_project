//
// Created by Tiane Zhu on 11/25/20.
//

#include <memory>
#include <queue>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "cycleTimer.h"
#include "solver.h"

#define TRANSITION_COUNT 18
// transition functions
void (*transition[TRANSITION_COUNT])(cube_t *) =
    {
        F1, B1, L1, R1, U1, D1,
        F2, B2, L2, R2, U2, D2,
        F3, B3, L3, R3, U3, D3,
    };

const char *Solver::to_string(int operation) {
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

node_t *node_new_from_cube(cube_t* cube) {
  node_t *node = node_new();
  memcpy(node->cube, cube, sizeof(cube_t));
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
///////////////// routing ///////////

bool Solver::solve(cube_t *cube, int solution[MAX_DEPTH], int *num_steps) {
  // time the execution
  bool solution_found;
  double start_time = CycleTimer::currentSeconds();
  switch (this->method) {
    case BFS:
      solution_found = bfs_solve(cube, solution, num_steps);
      break;
    case IDA:
      solution_found = ida_solve(cube, solution, num_steps);
      break;
    default:
      fprintf(stderr, "unknown method %d\n", this->method);
      exit(1);
  }
  double end_time = CycleTimer::currentSeconds();

  // print results
  if (!solution_found) {
    fprintf(stderr, "did not find solution within %d steps\n", DEPTH_LIMIT);
    exit(0);
  }

  for (int i=0; i<*num_steps; i++) {
    fprintf(stdout, "%s ", to_string(solution[i]));
  }
  fprintf(stdout, "\n");

  // print timing
  double overallDuration = end_time - start_time;
  fprintf(stdout, "Overall: %.3f ms\n", 1000.f * overallDuration);

  return solution_found;
}

///////////////////////////////// BFS /////////////////////////

bool bfs_solve_internal(cube_t *cube, int solution[MAX_DEPTH], int *num_steps, std::queue<node_t *> q[2]) {

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

bool Solver::bfs_solve(cube_t *cube, int solution[MAX_DEPTH], int *num_steps) {
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

/////////////////////////// IDA ////////////////////////////////

int search(paracube::CornerPatternDatabase *corner_db, node_t *path[MAX_DEPTH], int *d, CubeSet &cubes, int g, int bound);

char corner_db_input_filename[19] = "data/corner.pdb";

void Solver::ida_init() {
  if (!corner_db.fromFile(corner_db_input_filename)) {
    printf("failed to init corner_db from file '%s'\n", corner_db_input_filename);
    exit(1);
  }
}

void Solver::ida_destroy() {
  // delete this->corner_db;
}

int h(paracube::CornerPatternDatabase *corner_db, node_t *node, int d) {
  //
  // return 1;
  DBG_CUBE(node->cube);
  return corner_db->getNumMoves(*node->cube);
}

int cost(node_t *n1, node_t *n2) {
  return 1;
}

// Pseudo code from wikipedia
#define FOUND 0
#define INFTY 0x7FFFFFFF
bool Solver::ida_solve(cube_t *cube, int solution[MAX_DEPTH], int *num_steps) {
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

  CubeSet cubes;
  while (1) {
    int t = search(&this->corner_db, path, &d, cubes, 0, bound);
    DBG_PRINTF("t: %d\n\n", t);
    if (t == FOUND) {
      node_t *n = path[d - 1];
      *num_steps = n->d;
      memcpy(solution, n->steps, MAX_DEPTH * sizeof(int));
      for (int i=0; i<MAX_DEPTH; i++) {
        DBG_PRINTF("%d ", solution[i]);
      }
      DBG_PRINTF("\n");
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

bool cube_visited(const CubeSet &cubes, cube_t *cube) {
  return cubes.find(*cube) != cubes.end();
}

int search(paracube::CornerPatternDatabase *corner_db, node_t *path[MAX_DEPTH], int *d, CubeSet &cubes, int g, int bound) {
  node_t *node = path[(*d) - 1];
  int f = g + h(corner_db, node, (*d) - 1);
  DBG_PRINTF("search h %d\n", f - g);

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

    DBG_PRINTF("!!! %s\n", Solver::to_string(op));
    for (int i=0; i<node->d; i++) {
      DBG_PRINTF("%s ", Solver::to_string(node->steps[i]));
    }
    DBG_PRINTF("\n");
    DBG_PRINTF("=========\n");
    ppp(node->cube);
    ppp(c);
    DBG_PRINTF("=========\n");

    if (cube_visited(cubes, c)) {
      free(c);
      free(n);
      continue;
    }

    n->steps[n->d] = op;
    n->d++;

    // path.push(succ)
    path[*d] = n;
    (*d)++;
    cubes.emplace(*c);

    int t = search(corner_db, path, d, cubes, g + cost(node, n), bound);

    if (t == FOUND) {
      return FOUND;
    }

    if (t < min) {
      min = t;
    }

    // path.pop()
    (*d)--;
    auto it = cubes.find(*c);
    if (it != cubes.end()) {
      cubes.erase(it);
    }

    free(c);
    free(n);
  }

  return min;
}
