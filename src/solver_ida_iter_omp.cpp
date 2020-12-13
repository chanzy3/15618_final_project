//
// Created by Tiane Zhu on 12/11/20.
//

#include <algorithm>

// TODO(tianez): remove
// #include "cycleTimer.h"

#include "solver_ida_iter_omp.h"

#ifdef COUNT_TRANSITIONS
extern int ida_iter_omp_num_transitions;
extern int ida_iter_omp_num_transitions_top_level;
#endif

bool SolverIdaIterOmp::solve(cube_t *cube, int *solution, int *num_steps) {
  return ida_solve_iter_omp(cube, solution, num_steps);
}

// TODO(tianez): atomic
extern int curr_iter_mean;

bool SolverIdaIterOmp::ida_solve_iter_omp(cube_t *cube, int solution[MAX_DEPTH], int *num_steps) {
  // TODO(tianez): trade extra computation to save memory:
  // redefine path into a list of ints (ops) and save only 1 cube:
  // likely not needed since IDA is memory constrained

  node_iter_t path[MAX_DEPTH];
  node_iter_t *root = &(path[0]);
  int bound;

  node_iter_init_from_cube(&path[0], cube);
  bound = h(&this->corner_db, root);
  DBG_PRINTF("initial bound %d\n", bound);

  while (1) {
    // reset problem
    root->g = 0;
    root->d = 0;
    root->op = 0;
    root->min = INFTY;

#pragma omp atomic // write
    curr_iter_mean += INFTY - curr_iter_mean;

    int solution_length = -1;
    /*
#pragma omp parallel
    {
#pragma omp single
      {
        search_iter_omp(&this->corner_db, path, &solution_length, bound);
      }
    }
    */
    search_iter_omp(&this->corner_db, path, &solution_length, bound);

    DBG_PRINTF("t: %d\n\n", t);
    if (solution_length >= 0) {
      *num_steps = solution_length;
      int s;
      for (s = 0; s < solution_length; s++) {
        solution[s] = (path[s].op) - 1;
      }

#ifdef COUNT_TRANSITIONS
      printf("top: %d, task: %d\n", ida_iter_omp_num_transitions_top_level, ida_iter_omp_num_transitions);
#endif

      return true;
    }

    int t;
// #pragma omp atomic read
    t = curr_iter_mean;

    if (t == INFTY) {
      return false;
    }
    bound = t;
  }

  // TODO(tianez): free entire path from 0 to d (inc)

  return false;
}

// TODO(tianez): atomic
extern bool found;

void SolverIdaIterOmp::search_iter_omp(paracube::CornerPatternDatabase *corner_db, node_iter_t path[MAX_DEPTH], int *solution_length, int bound) {
  OMP_PRINTF("bound: %d\n", bound);

  int task_creation_depth_limit = DEPTH_LIMIT_MIN + 1; // (1 loc taken by initial state)
  if (bound > TASK_BOUND_TARGET + DEPTH_LIMIT_MIN) {
    task_creation_depth_limit = bound - TASK_BOUND_TARGET + 1;
  }

  OMP_PRINTF("task_creation_depth_limit: %d\n", task_creation_depth_limit);
#ifdef COUNT_TRANSITIONS
  printf("top: %d, task: %d\n", ida_iter_omp_num_transitions_top_level, ida_iter_omp_num_transitions);
#endif

  // does not need to be atomic, locked access
  bool task_creation_completed = false;

  // TODO(tianez): init once;
  omp_lock_t task_creation_lock;
  omp_init_lock(&task_creation_lock);

  int path_d = 1;

#pragma omp parallel
  {
    /*
#pragma omp single
    {
      OMP_PRINTF("num_threads %d\n", omp_get_num_threads());
    }
     */
    while (1) {
      node_iter_t *local_path = NULL;
      int starting_depth = -1;

      omp_set_lock(&task_creation_lock);

      int found_c, task_creation_completed_c;
// #pragma omp atomic read
      found_c = found;
      if (found_c) {
        omp_unset_lock(&task_creation_lock);
        goto serial_task_creation_end; // found
      }

// #pragma omp atomic read
      task_creation_completed_c = task_creation_completed;
      if (task_creation_completed_c) {
        omp_unset_lock(&task_creation_lock);
        goto serial_task_creation_end; // complete task creation
      }

      while (path_d < task_creation_depth_limit) {
        node_iter_t *n_curr = &(path[path_d - 1]); // curr node
        node_iter_t *n_prev = path_d == 1 ? NULL : n_curr - 1;

        PPATH(path, path_d);

#ifdef PRUNE
        if (n_prev != NULL) {
          OMP_DBG_PRINTF("for %s, ", to_string(n_prev->op - 1));
          while (can_prune(n_prev->op - 1, n_curr->op)) {
            OMP_DBG_PRINTF("pruned %s, ", to_string(n_curr->op));
            n_curr->op++; // skip an op
          }
          OMP_DBG_PRINTF("continuing with %s\n", to_string(n_curr->op));
        }
#endif

        if (n_curr->op >= TRANSITION_COUNT) {
          // finished all ops

          if (path_d == 1) {
            // after last task spawned
            task_creation_completed = true;

            omp_unset_lock(&task_creation_lock);
            goto serial_task_creation_end;
          } else {
            // replaced by atomic update to curr_iter_min after task
            // n_prev->min = std::min(n_prev->min, n_curr->min);

            // node_iter_destroy(n_curr); // stack.pop
            // path[path_d - 1] = NULL;
            path_d--;

            continue; // resume work on previous problem, current problem complete
          }
        } else {
          node_iter_t *n_next = n_curr + 1;
          node_iter_cpy(n_next, n_curr);

          // generate next problem:
          // input: cube, g, d
          // problem internal state: min, op (iterate from 0 - 17)

#ifdef COUNT_TRANSITIONS
#pragma omp atomic
          ida_iter_omp_num_transitions_top_level++;
#endif

          // cube
          transition[n_curr->op](&(n_next->cube));
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

          if (test_converge(&(n_next->cube))) {
            // node_iter_destroy(n_next);

            // TODO(tianez): update and think
#pragma omp atomic // write
            curr_iter_mean -= curr_iter_mean; // TODO(tianez): hack of FOUND;

            *solution_length = path_d + 1; // TODO(tianez): correct?

            omp_unset_lock(&task_creation_lock);
            goto serial_task_creation_end;
          }

          if (f > bound) { // discard
            n_curr->min = std::min((int) n_curr->min, f);
            // node_iter_destroy(n_next);
          } else {

            if (path_d + 1 < task_creation_depth_limit) {
              // path[path_d] = n_next; // stack.push
              path_d++;
            } else {
              size_t siz = MAX_DEPTH * sizeof(node_iter_t);
              local_path = (node_iter_t *) malloc(siz);
              memcpy(local_path, path, siz);


              starting_depth = path_d + 1;

              break;
            }
          }
        }
      }

      omp_unset_lock(&task_creation_lock);

      if (starting_depth != -1) {
        // double s = CycleTimer::currentSeconds();
        int local_solution_length = search_iter_omp_helper(corner_db, local_path, bound, starting_depth);
        // double e = CycleTimer::currentSeconds();
        // OMP_PRINTF("r: %f ms\n", (e - s) * 1000.0f);

        // TODO(tianez): update global and sync
        if (curr_iter_mean > (int) local_path[starting_depth - 1].min) {
#pragma omp atomic // write
          curr_iter_mean -= curr_iter_mean - ((int) local_path[starting_depth - 1].min);
        }


        if (local_solution_length > 0) {
#pragma omp atomic // write
          found |= 1; // TODO(tianez): hack true;

          omp_set_lock(&task_creation_lock);

          memcpy(path, local_path, MAX_DEPTH * sizeof(node_iter_t));
          *solution_length = local_solution_length;

          omp_unset_lock(&task_creation_lock);

          free(local_path);
          goto serial_task_creation_end;
        }

        // TODO(tianez): solution length

        free(local_path);
      }

      local_path = NULL;
      starting_depth = -1;
    }

    serial_task_creation_end:
      int x = 0; // TODO(tianez): better way?

#pragma omp barrier
  }

  /*
    size_t siz = MAX_DEPTH * sizeof(node_iter_t);
    node_iter_t *local_path = malloc(siz);
    memcpy(local_path, path, siz);
    */

  omp_destroy_lock(&task_creation_lock);
}

int SolverIdaIterOmp::search_iter_omp_helper(paracube::CornerPatternDatabase *corner_db, node_iter_t path[MAX_DEPTH], int bound, int starting_depth) {
  node_iter_t *root = &(path[starting_depth - 1]);
  int path_d = starting_depth;

  if (test_converge(&(root->cube))) {
    return path_d;
  }

  // TODO(tianez): correct cond?
  while (1) {
    node_iter_t *n_curr = &(path[path_d - 1]); // curr node
    node_iter_t *n_prev = path_d == 1 ? NULL : n_curr - 1;

#ifdef PRUNE
    if (n_prev != NULL) {
      while (can_prune(n_prev->op - 1, n_curr->op)) {
        n_curr->op++; // skip an op
      }
    }
#endif

    // PPATH(path, path_d);
    // PWPATH(path, path_d);

    if (n_curr->op >= TRANSITION_COUNT) {
      // TODO(tianez): good place?
      if (found) {
        return -1;
      }
      // finished all ops

      if (path_d == starting_depth) {
        // TODO(tianez):
        return -1; // don't free root
      } else {
        n_prev->min = std::min(n_prev->min, n_curr->min);

        // node_iter_destroy(n_curr); // stack.pop
        // path[path_d - 1] = NULL;
        path_d--;

        continue; // resume work on previous problem, current problem complete
      }
    } else {
      node_iter_t *n_next = n_curr + 1;
      node_iter_cpy(n_next, n_curr);

      // generate next problem:
      // input: cube, g, d
      // problem internal state: min, op (iterate from 0 - 17)


#ifdef COUNT_TRANSITIONS
#pragma omp atomic
        ida_iter_omp_num_transitions++;
#endif

      // cube
      transition[n_curr->op](&(n_next->cube));
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

      if (test_converge(&(n_next->cube))) {
        // node_iter_destroy(n_next);

        n_curr->min = FOUND;
        return path_d; // return found (solution_length)
      }

      if (f > bound) { // discard
        n_curr->min = std::min((int) n_curr->min, f);
        // node_iter_destroy(n_next);
      } else {
        // path[path_d] = n_next; // stack.push
        path_d++;
      }
    }
  }
}

