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
extern int numProcessors;
extern int processorId;
MPI_Datatype mpi_ans_t;
// transition functions
void (*transition[TRANSITION_COUNT])(cube_t *) =
    {
        F1,
        B1,
        L1,
        R1,
        U1,
        D1,
        F2,
        B2,
        L2,
        R2,
        U2,
        D2,
        F3,
        B3,
        L3,
        R3,
        U3,
        D3,
};

const char *Solver::to_string(int operation)
{
  switch (operation)
  {
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
typedef struct node
{
  cube_t *cube;
  int steps[MAX_DEPTH];
  int d;
} node_t;

node_t *node_new()
{
  node_t *node = (node_t *)malloc(sizeof(node_t));
  if (node == NULL)
  {
    perror("malloc node");
    exit(1);
  }

  node->cube = cube_new(false);
  node->d = 0;

  return node;
}

node_t *node_new_from_cube(cube_t *cube)
{
  node_t *node = node_new();
  memcpy(node->cube, cube, sizeof(cube_t));
  return node;
}

node_t *node_cpy(node_t *node)
{
  node_t *ret = node_new();
  memcpy(ret->cube, node->cube, sizeof(cube_t));
  memcpy(ret->steps, node->steps, MAX_DEPTH * sizeof(int));
  ret->d = node->d;
  return ret;
}

void node_destroy(node_t *node)
{
  free(node->cube);
  free(node);
}
////////////////// signature ////////////////////
// bool Solver::ida_solve_mpi(cube_t *cube, int solution[MAX_DEPTH], int *num_steps);
///////////////// routing ////////////////////

bool Solver::solve(cube_t *cube, int solution[MAX_DEPTH], int *num_steps)
{
  // time the execution
  bool solution_found;
  double start_time = CycleTimer::currentSeconds();
  switch (this->method)
  {
  case BFS:
    solution_found = bfs_solve(cube, solution, num_steps);
    break;
  case IDA:
    solution_found = ida_solve(cube, solution, num_steps);
    break;
  case IDA_MPI:
    solution_found = ida_solve_mpi(cube, solution, num_steps);
    break;
  default:
    fprintf(stderr, "unknown method %d\n", this->method);
    exit(1);
  }
  double end_time = CycleTimer::currentSeconds();

  // print results
  if (!solution_found)
  {
    fprintf(stderr, "did not find solution within %d steps\n", DEPTH_LIMIT);
    exit(0);
  }
  if (this->method == IDA_MPI)
  {
    if (this->processorId == 0)
    {
      for (int i = 0; i < *num_steps; i++)
      {
        fprintf(stdout, "%s ", to_string(solution[i]));
      }
      fprintf(stdout, "\n");

      // print timing
      double overallDuration = end_time - start_time;
      fprintf(stdout, "Overall: %.3f ms\n", 1000.f * overallDuration);
    }
  }
  else
  {
    for (int i = 0; i < *num_steps; i++)
    {
      fprintf(stdout, "%s ", to_string(solution[i]));
    }
    fprintf(stdout, "\n");

    // print timing
    double overallDuration = end_time - start_time;
    fprintf(stdout, "Overall: %.3f ms\n", 1000.f * overallDuration);
  }

  return solution_found;
}

///////////////////////////////// BFS /////////////////////////

bool bfs_solve_internal(cube_t *cube, int solution[MAX_DEPTH], int *num_steps, std::queue<node_t *> q[2])
{

  node_t *root = node_new_from_cube(cube);

  q[0].push(root);

  for (int iter = 0; iter < DEPTH_LIMIT; iter++)
  {
    std::queue<node_t *> &from = q[iter % 2];
    std::queue<node_t *> &to = q[(iter + 1) % 2];

    node_t *n_old, *n;
    while (from.size() > 0)
    {
      n_old = from.front();
      from.pop();
      for (int op = 0; op < TRANSITION_COUNT; op++)
      {
        n = node_cpy(n_old);

        n->steps[n->d] = op;
        n->d++;
        transition[op](n->cube);
        if (test_converge(n->cube))
        {
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

bool Solver::bfs_solve(cube_t *cube, int solution[MAX_DEPTH], int *num_steps)
{
  bool ret;
  std::queue<node_t *> q[2];
  ret = bfs_solve_internal(cube, solution, num_steps, q);

  for (int i = 0; i < 2; i++)
  {
    while (q[i].size() != 0)
    {
      node_destroy(q[i].front());
      q[i].pop();
    }
  }

  return ret;
}

/////////////////////////// IDA ////////////////////////////////

int search(paracube::CornerPatternDatabase *corner_db, node_t *path[MAX_DEPTH], int *d, CubeSet &cubes, int g, int bound);

char corner_db_input_filename[19] = "data/corner.pdb";

void Solver::ida_init(int numProcessors = 1)
{
  this->numProcessors = numProcessors;

  if (!corner_db.fromFile(corner_db_input_filename))
  {
    printf("failed to init corner_db from file '%s'\n", corner_db_input_filename);
    exit(1);
  }
}

void Solver::ida_destroy()
{
  // delete this->corner_db;
}

int h(paracube::CornerPatternDatabase *corner_db, node_t *node, int d)
{
  //
  // return 1;
  DBG_CUBE(node->cube);
  return corner_db->getNumMoves(*node->cube);
}

int cost(node_t *n1, node_t *n2)
{
  return 1;
}

// Pseudo code from wikipedia
#define FOUND 0
#define INFTY 0x7FFFFFFF
bool Solver::ida_solve(cube_t *cube, int solution[MAX_DEPTH], int *num_steps)
{
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
  while (1)
  {
    int t = search(&this->corner_db, path, &d, cubes, 0, bound);
    DBG_PRINTF("t: %d\n\n", t);
    if (t == FOUND)
    {
      node_t *n = path[d - 1];
      *num_steps = n->d;
      memcpy(solution, n->steps, MAX_DEPTH * sizeof(int));
      for (int i = 0; i < MAX_DEPTH; i++)
      {
        DBG_PRINTF("%d ", solution[i]);
      }
      DBG_PRINTF("\n");
      return true;
    }
    if (t == INFTY)
    {
      return false;
    }
    bound = t;
  }

  // TODO(tianez): free entire path from 0 to d (inc)

  return false;
}

bool cube_visited(const CubeSet &cubes, cube_t *cube)
{
  return cubes.find(*cube) != cubes.end();
}

int search(paracube::CornerPatternDatabase *corner_db, node_t *path[MAX_DEPTH], int *d, CubeSet &cubes, int g, int bound)
{
  node_t *node = path[(*d) - 1];
  int f = g + h(corner_db, node, (*d) - 1);
  DBG_PRINTF("search h %d\n", f - g);
  // std::cout << "Depth is : " << *d << std::endl;

  //   for (int i = 0; i < node->d; i++)
  //   {
  //     printf("%s ", Solver::to_string(node->steps[i]));
  //   }
  //   printf("\n");

  if (f > bound)
  {
    //printf("f: %d, bound: %d\n", f, bound);
    return f;
  }

  if (test_converge(node->cube))
  {
    return FOUND;
  }

  int min = INFTY;

  for (int op = 0; op < TRANSITION_COUNT; op++)
  {
    node_t *n = node_cpy(node);
    cube_t *c = n->cube;

    transition[op](c); // succ->cube

    // DBG_PRINTF("!!! %s\n", Solver::to_string(op));
    // DBG_PRINTF("!!! Bound is : %d\n", bound);
    // for (int i = 0; i < node->d; i++)
    // {
    //   DBG_PRINTF("%s ", Solver::to_string(node->steps[i]));
    // }
    // R3 D3 B3 L3 U3 B3 L3 R3
    // DBG_PRINTF("\n");
    // DBG_PRINTF("=========\n");
    // ppp(node->cube);
    // ppp(c);
    // DBG_PRINTF("=========\n");

    if (cube_visited(cubes, c))
    {
      free(c);
      free(n);
      continue;
    }

    n->steps[n->d] = op;
    n->d++;

    // path.push(succ)
    path[*d] = n;
    (*d)++;
    cubes.insert(*c);

    int t = search(corner_db, path, d, cubes, g + cost(node, n), bound);

    if (t == FOUND)
    {
      //     for (int i = 0; i < node->d; i++)
      // {
      //   printf("%s ", Solver::to_string(node->steps[i]));
      // }
      // printf("\n");
      return FOUND;
    }

    if (t < min)
    {
      min = t;
    }

    // path.pop()
    (*d)--;
    auto it = cubes.find(*c);
    if (it != cubes.end())
    {
      cubes.erase(it);
    }

    free(c);
    free(n);
  }

  return min;
}

///////////////////////////////////////////   IDA_MPI   /////////////////////////////////////////////

typedef struct ans
{
  int bound;
  int depth;
  int branch_id;
  int solution[MAX_DEPTH];
} ans_t;

MPI_Datatype createType()
{
  MPI_Datatype new_type;
  const int nitems = 4;
  MPI_Datatype types[4] = {MPI_INT, MPI_INT, MPI_INT, MPI_INT};
  MPI_Aint offsets[4];
  int blocklens[4] = {1, 1, 1, MAX_DEPTH};

  offsets[0] = offsetof(ans_t, bound);
  offsets[1] = offsetof(ans_t, depth);
  offsets[2] = offsetof(ans_t, branch_id);
  offsets[3] = offsetof(ans_t, solution);

  MPI_Type_create_struct(nitems, blocklens, offsets, types, &new_type);
  MPI_Type_commit(&new_type);
  return new_type;
}

void run_master(paracube::CornerPatternDatabase *corner_db, cube_t *cube, int solution[MAX_DEPTH], int *num_steps)
{
  MPI_Status mpi_sta;
  node_t *root = node_new_from_cube(cube);

  // node_t *path[MAX_DEPTH];
  int numWorkers = numProcessors - 1;
  ans_t ans;
  ans_t buf;
  bool flag = false;

  // ans.solution[0] = root;
  int d;
  int bound;
  int nextBound = INFTY;

  int branch_id = 0;

  //path[0] = root;
  d = 1;
  bound = h(corner_db, root, d);

  CubeSet cubes;

  while (1)
  {
    // recv t
    // std::cout << "Receiving messages from worker, nextbound is " << nextBound << std::endl;
    MPI_Recv(&ans, 1, mpi_ans_t, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &mpi_sta);
    // std::cout << "Finished receiving messages from worker:" << mpi_sta.MPI_SOURCE << std::endl;
    switch (mpi_sta.MPI_TAG)
    {
    case ALLOCATE_TAG:
      // std::cout << "Allocating tags" << std::endl;
      if (nextBound == FOUND)
      {
        numWorkers--;
        // std::cout << "Sending Abort information" << std::endl;

        MPI_Send(&buf, 1, mpi_ans_t, mpi_sta.MPI_SOURCE,
                 ABORT_TAG, MPI_COMM_WORLD);
        // std::cout << "Finish sending abort information to worker:  " << mpi_sta.MPI_SOURCE << std::endl;
        std::cout << numWorkers << ", number of workers remaining: " << std::endl;
        if (numWorkers == 0)
        {
          // std::cout << "Exiting " << std::endl;
          return;
        }
      }
      else
      {
        if (branch_id == TRANSITION_COUNT) {
          bound = nextBound;
          nextBound = INFTY;
          branch_id = 0;
        }
        // if (branch_id < TRANSITION_COUNT)
        // {
          // prepare buffer to send to worker node
        buf.branch_id = branch_id;
        buf.bound = bound;
        memcpy(buf.solution, ans.solution, sizeof(int) * MAX_DEPTH);
        buf.depth = 1;

        MPI_Send(&buf, 1, mpi_ans_t, mpi_sta.MPI_SOURCE, START_TAG, MPI_COMM_WORLD);
        // std::cout << "branch_id:" << branch_id << std::endl;
        branch_id++;
        // }
        // Finish iterative deepening for bound
        // else
        // {
        //   // set the bound to nextBound and start assigning jobs again
        //   // bound = nextBound;

        //   // If bound is 0, we found a solution, abort the rest nodes

        //   // MPI_Finalize();

        //   // If boun is not 0, solution not found yet,
        //   bound = nextBound;
        //   nextBound = INFTY;
        //   branch_id = 0;

        //   buf.branch_id = branch_id;
        //   buf.bound = bound;
        //   memcpy(buf.solution, ans.solution, sizeof(int) * MAX_DEPTH);
        //   buf.depth = 1;

        //   MPI_Send(&buf, 1, mpi_ans_t, mpi_sta.MPI_SOURCE, START_TAG, MPI_COMM_WORLD);
        //   branch_id++;
        // }
      }

      break;

    case UPDATE_TAG:
      // std::cout << "Update tag recieved !" << std::endl;
      // one worker node found the solution, update solution and depth
      //std::cout << "Bound master is:" << ans.bound << std::endl;
      if (ans.bound == FOUND && flag == false)
      {
        memcpy(solution, ans.solution, sizeof(int) * MAX_DEPTH);
        *num_steps = ans.depth+1;
        // std::cout  << "Found solution!" << std::endl;
        for (int i = 0; i < *num_steps; i++)
        {
          std::cout << Solver::to_string(solution[i]) << " ";
        }
        std::cout << std::endl;
        flag = true;
      }
      nextBound = std::min(nextBound, ans.bound);
      break;
    }
  }
}

void Solver::run_worker(cube_t *cube, paracube::CornerPatternDatabase *corner_db)
{
  MPI_Status mpi_sta;
  ans_t ans;
  ans_t buf;
  ans_t res;
  int depth;
  int bound;
  int op;
  node_t *root;
  node_t *path[MAX_DEPTH];
  node_t *n;
  cube_t *c;

  CubeSet cubes;

  while (1)
  {
    // if (this->processorId == 3) {
    //   std::cout << "Sending allocating message" << std::endl;
    // }
    // std::cout << "worker id: " << this->processorId << "Sending allocating message" << std::endl;
    MPI_Send(&ans, 1, mpi_ans_t, 0, ALLOCATE_TAG, MPI_COMM_WORLD);
    // std::cout << "worker id: " << this->processorId << "Finish sending allocating message" << std::endl;
    //  if (this->processorId == 1) {
    // std::cout << "Finish sending allocating message" << std::endl;
    // }
    // std::cout << "worker: " << this->processorId  << " Wating for recieve buffer form master" << std::endl;
    MPI_Recv(&buf, 1, mpi_ans_t, 0, MPI_ANY_TAG, MPI_COMM_WORLD, &mpi_sta);
    // std::cout << "worker: " << this->processorId << "  Finished receiving buffer form master" << std::endl;

    switch (mpi_sta.MPI_TAG)
    {
    case START_TAG:

      op = buf.branch_id;

      root = node_new_from_cube(cube);

      // node_t *path[MAX_DEPTH];

      path[0] = root;
      // perform on original cube, search one more step
      n = node_cpy(root);
      c = n->cube;
      transition[op](c); // succ->cube
      n->d = 1;
      n->steps[0] = op;

      path[1] = n;
      depth = 2;
      //cubes.clear();

      bound = buf.bound;

      res.bound = search(corner_db, path, &depth, cubes, 0, bound);
      n = path[depth - 1];
      res.depth = n->d - 1;
      memcpy(res.solution, n->steps, MAX_DEPTH * sizeof(int));

      MPI_Send(&res, 1, mpi_ans_t, 0, UPDATE_TAG, MPI_COMM_WORLD);

      break;
    case ABORT_TAG:
      // std::cout << "Aborting tasks: " << this->processorId <<  std::endl;
      return;
    }
  }
}

bool Solver::ida_solve_mpi(cube_t *cube, int solution[MAX_DEPTH], int *num_steps)
{

  if (numProcessors == 1)
  {
    ida_solve(cube, solution, num_steps);
  }
  else
  {
    // std::cout << "I'm here" << std::endl;
    MPI_Comm_size(MPI_COMM_WORLD, &numProcessors);
    MPI_Comm_rank(MPI_COMM_WORLD, &this->processorId);

    mpi_ans_t = createType();
    MPI_Barrier(MPI_COMM_WORLD);

    if (this->processorId == 0)
    {
      run_master(&this->corner_db, cube, solution, num_steps);
      //  for (int i = 0; i < *num_steps; i++) {
      //    std::cout << to_string(solution[i]) << " ";
      //  }
      //  std::cout << std::endl;
    }
    else
      run_worker(cube, &this->corner_db);
    MPI_Barrier(MPI_COMM_WORLD);
    return true;
  }
}