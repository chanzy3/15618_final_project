//
// Created by Tiane Zhu on 11/25/20.
//

#include <memory>
#include <queue>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <cstdint>
#include <vector>
#include <functional>
#include "cycleTimer.h"
#include "solver.h"
#include <stack>

#define TRANSITION_COUNT 18
#define MAX_SEND_SIZE 1000

extern int numProcessors;
extern int processorId;
MPI_Datatype mpi_ans_t;
MPI_Datatype mpi_ans_t2;
MPI_Datatype mpi_node_t;
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
    ;
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
  case IDA_MPI2:
    solution_found = ida_solve_mpi_v2(cube, solution, num_steps);
    break;
  case IDA_MPI3:
    solution_found = ida_solve_mpi_v3(cube, solution, num_steps);
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
  if (this->method == IDA_MPI || this->method == IDA_MPI2 || this->method == IDA_MPI3)
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
      MPI_Abort(MPI_COMM_WORLD, 0);
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

int bounded_dfs(std::stack<node_t *> &nodeStack, paracube::CornerPatternDatabase *corner_db, node_t *path[MAX_DEPTH], int *d, CubeSet cubes, int bound);
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
  std::stack<node_t *> nodeStack;
  while (1)
  {
    // int t = search(&this->corner_db, path, &d, cubes, 0, bound);
    // std::cout << "Bound is" << bound << "depth is: " << d << std::endl;
    cubes.clear();
    cubes.insert(*cube);

    root = node_new_from_cube(cube);
    nodeStack.push(root);
    std::cout << "Size of cubes set is" << cubes.size() << std::endl;
    std::cout << "size of the nodestack is" << nodeStack.size() << std::endl;

    int t = bounded_dfs(nodeStack, &this->corner_db, path, &d, cubes, bound);

    DBG_PRINTF("t: %d\n\n", t);
    if (t == FOUND)
    {
      node_t *n = path[d];
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
    d = 1;
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
    MPI_Recv(&ans, 1, mpi_ans_t, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &mpi_sta);
    switch (mpi_sta.MPI_TAG)
    {
    case ALLOCATE_TAG:
      if (nextBound == FOUND)
      {
        numWorkers--;

        MPI_Send(&buf, 1, mpi_ans_t, mpi_sta.MPI_SOURCE,
                 ABORT_TAG, MPI_COMM_WORLD);
        std::cout << numWorkers << ", number of workers remaining: " << std::endl;
        if (numWorkers == 0)
        {
          return;
        }
      }
      else
      {
        if (branch_id == TRANSITION_COUNT)
        {
          bound = nextBound;
          nextBound = INFTY;
          branch_id = 0;
        }

        buf.branch_id = branch_id;
        buf.bound = bound;
        memcpy(buf.solution, ans.solution, sizeof(int) * MAX_DEPTH);
        buf.depth = 1;

        MPI_Send(&buf, 1, mpi_ans_t, mpi_sta.MPI_SOURCE, START_TAG, MPI_COMM_WORLD);
        branch_id++;
      }

      break;

    case UPDATE_TAG:

      if (ans.bound == FOUND && flag == false)
      {
        memcpy(solution, ans.solution, sizeof(int) * MAX_DEPTH);
        *num_steps = ans.depth + 1;
        // std::cout  << "Found solution!" << std::endl;
        for (int i = 0; i < *num_steps; i++)
        {
          std::cout << Solver::to_string(solution[i]) << " ";
        }
        std::cout << std::endl;
        flag = true;
        return;
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

    MPI_Send(&ans, 1, mpi_ans_t, 0, ALLOCATE_TAG, MPI_COMM_WORLD);

    MPI_Recv(&buf, 1, mpi_ans_t, 0, MPI_ANY_TAG, MPI_COMM_WORLD, &mpi_sta);

    switch (mpi_sta.MPI_TAG)
    {
    case START_TAG:

      op = buf.branch_id;

      root = node_new_from_cube(cube);

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
    }
    else
      run_worker(cube, &this->corner_db);
    // MPI_Barrier(MPI_COMM_WORLD);
    return true;
  }
}

/////////////////////////////MPIDA Version1 //////////////////////////////////////////

// Expand as many frontier nodes as the number of processors in the first expansion
// Performed on master node
typedef struct ans2
{
  int bound;
  int depth;
  int is_partial;
  int contract_size;
  int solution[MAX_DEPTH];
} ans_t2;

MPI_Datatype createType_v2()
{
  MPI_Datatype new_type;
  const int nitems = 5;
  MPI_Datatype types[5] = {MPI_INT, MPI_INT, MPI_INT, MPI_INT, MPI_INT};
  MPI_Aint offsets[5];
  int blocklens[5] = {1, 1, 1, 1, MAX_DEPTH};

  offsets[0] = offsetof(ans_t2, bound);
  offsets[1] = offsetof(ans_t2, depth);
  offsets[2] = offsetof(ans_t2, is_partial);
  offsets[3] = offsetof(ans_t2, contract_size);
  offsets[4] = offsetof(ans_t2, solution);

  MPI_Type_create_struct(nitems, blocklens, offsets, types, &new_type);
  MPI_Type_commit(&new_type);
  return new_type;
}

typedef struct Node
{
  node_t *node;
  int f;
} Node_t;

class ComparisonClass
{
public:
  bool operator()(const Node_t &a, const Node_t &b)
  {
    return a.f > b.f;
  }
};

// only expand those having lower f value
std::vector<node_t *> bfs_expand_greedy(cube_t *cube, int num_workers, bool &is_partial, int &contract_size, paracube::CornerPatternDatabase *corner_db)
{
  int g;
  std::vector<node_t *> res;
  std::priority_queue<Node_t, std::vector<Node_t>, ComparisonClass> pq;
  Node_t n_parent;
  node_t *n;

  node_t *root_node = node_new_from_cube(cube);
  g = root_node->d + h(corner_db, root_node, root_node->d);
  Node_t root = {root_node, g};

  pq.push(root);

  while (pq.size() < num_workers)
  {
    n_parent = pq.top();
    pq.pop();

    for (int op = 0; op < TRANSITION_COUNT; op++)
    {
      if (pq.size() + 1 >= num_workers && op != TRANSITION_COUNT - 1)
      {
        // pq.push(n_parent);
        contract_size = op;
        is_partial = true;
        while (!pq.empty())
        {
          res.push_back(pq.top().node);
          pq.pop();
        }
        // keep the contracted node at the last of the list
        res.push_back(n_parent.node);
        return res;
      }
      n = node_cpy(n_parent.node);

      n->steps[n->d] = op;
      n->d++;
      transition[op](n->cube);
      g = n->d + h(corner_db, n, n->d);
      Node_t n_pq{n, g};
      pq.push(n_pq);
    }
  }
  // No nodes are contracted here
  while (!pq.empty())
  {
    res.push_back(pq.top().node);
    pq.pop();
  }
  return res;
}

std::vector<node_t *> bfs_expand(cube_t *cube, int num_tasks, bool &is_partial, int &contract_size)
{
  std::vector<node_t *> frontier_nodes;
  node_t *root = node_new_from_cube(cube);
  frontier_nodes.push_back(root);
  node_t *n_parent, *n;

  while ((int)frontier_nodes.size() < num_tasks)
  {
    n_parent = frontier_nodes.front();
    frontier_nodes.erase(frontier_nodes.begin());
    for (int op = 0; op < TRANSITION_COUNT; op++)
    {

      // partial contraction to parents is number of decesdents exceed the number of workers
      // In this case, the last node will be the contraction node
      if (frontier_nodes.size() >= num_tasks)
      {
        frontier_nodes.pop_back();
        frontier_nodes.push_back(n_parent);
        contract_size = op - 1;
        is_partial = true;
        return frontier_nodes;
      }
      n = node_cpy(n_parent);

      n->steps[n->d] = op;
      n->d++;
      transition[op](n->cube);

      frontier_nodes.push_back(n);
    }
  }
  return frontier_nodes;
}

void run_master_v2(paracube::CornerPatternDatabase *corner_db, cube_t *cube, int solution[MAX_DEPTH], int *num_steps)
{
  bool is_partial = false;
  int contract_size = 0;
  MPI_Status mpi_sta;
  int num_workers = numProcessors - 1;
  ans_t2 buf;
  ans_t2 ans;
  int num_messages_recv = 0;

  int bound = INFTY;
  int next_bound = INFTY;

  node_t *n;

  std::vector<node_t *> frontier_nodes;
  frontier_nodes = bfs_expand_greedy(cube, num_workers, is_partial, contract_size, corner_db);
  // frontier_nodes = bfs_expand(cube, num_workers, is_partial, contract_size);

  // intial bound is the minimum of the f value of frontier nodes
  for (int i = 0; i < frontier_nodes.size(); i++)
  {
    n = frontier_nodes[i];
    bound = std::min(bound, n->d + h(corner_db, n, n->d));
    // std::cout << "g value of node " << i << "is: " << n->d + h(corner_db, n, n->d) << std::endl;
  }

  // assign work to number of workers
  for (int i = 0; i < num_workers; i++)
  {
    n = frontier_nodes[i];
    buf.bound = bound;
    buf.depth = n->d;
    memcpy(buf.solution, n->steps, n->d * sizeof(int));
    buf.is_partial = 0;
    buf.contract_size = 0;
    if (i == num_workers - 1)
    {
      if (is_partial)
      {
        buf.is_partial = true;
        buf.contract_size = contract_size;
      }
    }

    MPI_Send(&buf, 1, mpi_ans_t2, i + 1, START_TAG, MPI_COMM_WORLD);
  }

  while (1)
  {
    MPI_Recv(&ans, 1, mpi_ans_t2, MPI_ANY_SOURCE, FINISH_TAG, MPI_COMM_WORLD, &mpi_sta);
    if (ans.bound == FOUND)
    {
      memcpy(solution, ans.solution, sizeof(int) * MAX_DEPTH);
      *num_steps = ans.depth + 1;
      // Abort MPI_COMM immediately, this will cause speedup anomaly
      return;
    }
    if (ans.bound < next_bound)
    {
      next_bound = ans.bound;
    }
    num_messages_recv++;
    if (num_messages_recv == num_workers)
    {
      bound = next_bound;
      next_bound = INFTY;
      num_messages_recv = 0;
      // broadcast bound to every worker node
      for (int i = 0; i < num_workers; i++)
      {
        MPI_Send(&bound, 1, MPI_INT, i + 1, UPDATE_TAG, MPI_COMM_WORLD);
      }
    }
  }
}

void run_worker_v2(cube_t *cube, paracube::CornerPatternDatabase *corner_db)
{
  MPI_Status mpi_sta;
  ans_t2 ans;
  ans_t2 buf;
  int depth;
  int bound;
  int is_partial;
  int contract_size;
  CubeSet cubes;
  int steps[MAX_DEPTH];
  node_t *root, *cur_n;
  cube_t *c;
  node_t *path[MAX_DEPTH];
  std::stack<node_t *> nodeStack;
  int op;
  int res_depth;

  while (1)
  {
    MPI_Recv(&buf, 1, mpi_ans_t2, 0, MPI_ANY_TAG, MPI_COMM_WORLD, &mpi_sta);

    if (mpi_sta.MPI_TAG == START_TAG)
    {
      root = node_new_from_cube(cube);
      bound = buf.bound;

      path[0] = root;
      cur_n = node_cpy(root);
      cubes.insert(*(cur_n->cube));
      memcpy(steps, buf.solution, MAX_DEPTH * sizeof(int));
      is_partial = buf.is_partial;
      contract_size = buf.contract_size;

      depth = buf.depth;
      for (int i = 0; i < depth; i++)
      {
        op = steps[i];
        cur_n = node_cpy(cur_n);
        c = cur_n->cube;
        transition[op](c);
        cur_n->d++;
        cur_n->steps[i] = op;
        cubes.insert(*(cur_n->cube));
        path[i + 1] = cur_n;
      }

      // If partial contraction, make sure add indications of nodes that are added to
      // other nodes
      if (is_partial == 1)
      {
        cur_n = path[depth];
        for (op = 0; op < contract_size; op++)
        {
          cur_n = node_cpy(cur_n);
          c = cur_n->cube;
          transition[op](c);
          cubes.insert(*(cur_n->cube));
        }
      }
      depth++;
    }

    else if (mpi_sta.MPI_TAG == UPDATE_TAG)
      bound = buf.bound;

    nodeStack.push(node_cpy(path[depth - 1]));
    ans.bound = bounded_dfs(nodeStack, corner_db, path, &res_depth, cubes, bound);
    cur_n = path[res_depth];
    ans.depth = cur_n->d - 1;
    memcpy(ans.solution, cur_n->steps, MAX_DEPTH * sizeof(int));

    MPI_Send(&ans, 1, mpi_ans_t2, 0, FINISH_TAG, MPI_COMM_WORLD);
  }
}

bool Solver::ida_solve_mpi_v2(cube_t *cube, int solution[MAX_DEPTH], int *num_steps)
{
  if (numProcessors == 1)
    ida_solve(cube, solution, num_steps);
  else
  {
    MPI_Comm_size(MPI_COMM_WORLD, &numProcessors);
    MPI_Comm_rank(MPI_COMM_WORLD, &this->processorId);

    mpi_ans_t2 = createType_v2();
    MPI_Barrier(MPI_COMM_WORLD);

    if (this->processorId == 0)
    {
      run_master_v2(&this->corner_db, cube, solution, num_steps);
    }
    else
      run_worker_v2(cube, &this->corner_db);

    return true;
  }
}




//////////////////////////////v3 /////////////////////////
typedef struct mpi_node
{
  uint8_t cube[54];
  int steps[MAX_DEPTH];
  int d;
} mpi_node_s;

// measure this method
int bounded_dfs(std::stack<node_t *> &nodeStack, paracube::CornerPatternDatabase *corner_db, node_t *path[MAX_DEPTH], int *d, CubeSet cubes, int bound)
{
  node_t *curNode, *nodeTop;
  int res = INFTY;
  int f;
  cube_t *c;
  mpi_node_s n;
  uint8_t *cubeArr;
  int ans, size_buf, sendWorkNum, sendStackSize;
  int flag = 0;
  mpi_node_s stackBuf[MAX_SEND_SIZE];
  MPI_Request irreq;
  MPI_Status itest_sta, mpi_sta;

  while (!nodeStack.empty())
  {
#ifdef WORKSTEAL
    // "listen for" request_workload signal from master node
    MPI_Irecv(&ans, 1, MPI_INT, 0, REQUEST_WORKLOAD, MPI_COMM_WORLD, &irreq);
#endif

    curNode = nodeStack.top();
    nodeStack.pop();

    for (int op = 0; op < TRANSITION_COUNT; op++)
    {

      node_t *n = node_cpy(curNode);
      cube_t *c = n->cube;

      transition[op](c);

      n->steps[n->d] = op;
      n->d++;
      f = n->d + h(corner_db, n, n->d);

      if (f > bound)
      {
        res = std::min(f, res);
        free(c);
        free(n);
        continue;
      }

      path[n->d] = n;
      *d = n->d;

      if (test_converge(n->cube))
        return FOUND;

      cubes.insert(*c);
      nodeStack.push(n);
    }
    free(curNode);
#ifdef WORKSTEAL
    // to test if we got the workload request yet
    std::cout << "Start Testing" << std::endl;
    MPI_Test(&irreq, &flag, &itest_sta);
    std::cout << "Finish testing" << "flag: " << flag << std::endl;
    // if yes, reply workload to master processor
    if (flag != 0)
    {
      std::cout << "detect idle" << std::endl;
      size_buf = nodeStack.size();
      // Tell master node how large the stack is
      MPI_Send(&size_buf, 1, MPI_INT, 0, SIZE_TAG, MPI_COMM_WORLD);
      // Receive from master whether to send nodes
      MPI_Recv(&sendWorkNum, 1, MPI_INT, 0, IF_SEND_TAG, MPI_COMM_WORLD, &mpi_sta);

      if (sendWorkNum != 0)
      {
        // send half of its stack to idle nodes
        sendStackSize = std::min((int)nodeStack.size() / 2, MAX_SEND_SIZE);
        // shall we send visited set as well?
        for (int i = 0; i < sendStackSize; i++)
        {
          nodeTop = nodeStack.top();
          c = nodeTop->cube;

          cubeArr = c->convertCubeToArr();
          memcpy(n.steps, nodeTop->steps, sizeof(int) * MAX_DEPTH);
          memcpy(n.cube, cubeArr, sizeof(uint8_t) * 54);
          n.d = nodeTop->d;

          stackBuf[i] = n;
          cubes.insert(*(c));

          nodeStack.pop();
          free(nodeTop);
        }

        // TODO: define mpi_node_t
        MPI_Send(stackBuf, sendStackSize, mpi_node_t, sendWorkNum, SEND_WORK, MPI_COMM_WORLD);
        flag = false;
      }
    }
    else
    {
      // If no work received, abort request
      MPI_Cancel(&irreq);
      MPI_Request_free(&irreq);
    }

#endif
  }
  return res;
}

////////////////IDA v3 with work stealing /////////

MPI_Datatype createMpiNode()
{
  MPI_Datatype new_type;
  const int nitems = 3;
  MPI_Datatype types[3] = {MPI_UINT8_T, MPI_INT, MPI_INT};
  MPI_Aint offsets[3];
  int blocklens[3] = {54, MAX_DEPTH, 1};

  offsets[0] = offsetof(mpi_node_s, cube);
  offsets[1] = offsetof(mpi_node_s, steps);
  offsets[2] = offsetof(mpi_node_s, d);

  MPI_Type_create_struct(nitems, blocklens, offsets, types, &new_type);
  MPI_Type_commit(&new_type);
  return new_type;
}


void run_master_v3(paracube::CornerPatternDatabase *corner_db, cube_t *cube, int solution[MAX_DEPTH], int *num_steps)
{
  /////////////Initial Distribution ////////////////
  bool is_partial = false;
  int contract_size = 0;
  MPI_Status mpi_sta;
  int num_workers = numProcessors - 1;
  ans_t2 buf;
  ans_t2 ans;
  int num_messages_recv = 0;
  int askWorkLoad=0, maxWorkLoad = -INFTY, bufWorkLoad, numWorkerSend = -1;
  int stealWorkInfo[2];

  int bound = INFTY;
  int next_bound = INFTY;

  node_t *n;
  bool isTermination[100];

  std::vector<node_t *> frontier_nodes;
  frontier_nodes = bfs_expand_greedy(cube, num_workers, is_partial, contract_size, corner_db);
  // frontier_nodes = bfs_expand(cube, num_workers, is_partial, contract_size);

  for (int i = 0; i < num_workers; i++)
    isTermination[i] = false;
  // intial bound is the minimum of the f value of frontier nodes
  for (int i = 0; i < frontier_nodes.size(); i++)
  {
    n = frontier_nodes[i];
    bound = std::min(bound, n->d + h(corner_db, n, n->d));
    // std::cout << "g value of node " << i << "is: " << n->d + h(corner_db, n, n->d) << std::endl;
  }

  // assign work to number of workers
  for (int i = 0; i < num_workers; i++)
  {
    n = frontier_nodes[i];
    buf.bound = bound;
    buf.depth = n->d;
    memcpy(buf.solution, n->steps, n->d * sizeof(int));
    buf.is_partial = 0;
    buf.contract_size = 0;
    if (i == num_workers - 1)
    {
      if (is_partial)
      {
        buf.is_partial = true;
        buf.contract_size = contract_size;
      }
    }

    MPI_Send(&buf, 1, mpi_ans_t2, i + 1, START_TAG, MPI_COMM_WORLD);
    // MPI_Recv()
  }

  //////////////// Loop //////////////////
  while (1)
  {

    MPI_Recv(&ans, 1, mpi_ans_t2, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &mpi_sta);
    // std::cout << "mpi receiving" << std::endl;
    switch (mpi_sta.MPI_TAG)
    {
    case FINISH_TAG:
      if (ans.bound == FOUND)
      {
        memcpy(solution, ans.solution, sizeof(int) * MAX_DEPTH);
        *num_steps = ans.depth + 1;
        // Abort MPI_COMM immediately, this will cause speedup anomaly
        return;
      }

      if (ans.bound < next_bound)
      {
        next_bound = ans.bound;
      }

      maxWorkLoad = -INFTY;
      numWorkerSend = -1;
      // Ask every working worker node their workload (except for the one that asked work)
      for (int i = 0; i < num_workers; i++)
      {
        if (!isTermination[i] && (i + 1 != mpi_sta.MPI_SOURCE))
        {
          MPI_Send(&askWorkLoad, 1, MPI_INT, i + 1, REQUEST_WORKLOAD, MPI_COMM_WORLD);
          MPI_Recv(&bufWorkLoad, 1, MPI_INT, i + 1, SIZE_TAG, MPI_COMM_WORLD, &mpi_sta);

          if (maxWorkLoad < bufWorkLoad && bufWorkLoad > 5)
          {
            maxWorkLoad = bufWorkLoad;
            numWorkerSend = i + 1;
          }
        }
      }
      
      // Tell idle node if they would expect to receive from others
      stealWorkInfo[0] = numWorkerSend;
      stealWorkInfo[1] = maxWorkLoad;
      std::cout << "Start sending steal work info " << std::endl;
      MPI_Send(stealWorkInfo, 2, MPI_INT, mpi_sta.MPI_SOURCE, IF_STEAL_TAG, MPI_COMM_WORLD);

      // Tell them if they need to send work if yes, tell them where to send, or else tell them 0
      
      for (int i = 0; i < num_workers; i++)
      {
        if (i + 1 == numWorkerSend && !isTermination[i] && i + 1 != mpi_sta.MPI_SOURCE)
        {
          MPI_Send(&mpi_sta.MPI_SOURCE, 1, MPI_INT, i + 1, IF_SEND_TAG, MPI_COMM_WORLD);
        }
        else
        {
          MPI_Send(0, 1, MPI_INT, i + 1, IF_SEND_TAG, MPI_COMM_WORLD);
        }
      }
      break;

    case TERMINATION_TAG:
      num_messages_recv++;
      isTermination[mpi_sta.MPI_SOURCE - 1] = true;
      break;
    }

    if (num_messages_recv == num_workers)
    {
      bound = next_bound;
      next_bound = INFTY;
      num_messages_recv = 0;

      // broadcast bound to every worker node
      for (int i = 0; i < num_workers; i++)
      {
        MPI_Send(&bound, 1, MPI_INT, i + 1, UPDATE_TAG, MPI_COMM_WORLD);
      }
    }
  }
}

void run_worker_v3(cube_t *cube, paracube::CornerPatternDatabase *corner_db)
{
  MPI_Status mpi_sta, mpi_sta2;
  ans_t2 ans;
  ans_t2 buf;
  int depth, bound, is_partial, contract_size, op, res_depth, stealSize, flag=0, ansWorkload, flag2=0;
  MPI_Request irreq, irreqrec; 

  int stealWorkInfo[2];

  CubeSet cubes;
  int steps[MAX_DEPTH];
  mpi_node_s stealBuf[MAX_SEND_SIZE], cur_mpi_node;
  node_t *root, *cur_n;
  cube_t *c;
  node_t *path[MAX_DEPTH];
  std::stack<node_t *> nodeStack, stealNodeStack;


  while (1)
  {
    MPI_Recv(&buf, 1, mpi_ans_t2, 0, MPI_ANY_TAG, MPI_COMM_WORLD, &mpi_sta);

    if (mpi_sta.MPI_TAG == START_TAG)
    {
      root = node_new_from_cube(cube);
      bound = buf.bound;

      path[0] = root;
      cur_n = node_cpy(root);
      cubes.insert(*(cur_n->cube));
      memcpy(steps, buf.solution, MAX_DEPTH * sizeof(int));
      is_partial = buf.is_partial;
      contract_size = buf.contract_size;

      depth = buf.depth;
      for (int i = 0; i < depth; i++)
      {
        op = steps[i];
        cur_n = node_cpy(cur_n);
        c = cur_n->cube;
        transition[op](c);
        cur_n->d++;
        cur_n->steps[i] = op;
        cubes.insert(*(cur_n->cube));
        path[i + 1] = cur_n;
      }

      // If partial contraction, make sure add indications of nodes that are added to
      // other nodes
      if (is_partial == 1)
      {
        cur_n = path[depth];
        for (op = 0; op < contract_size; op++)
        {
          cur_n = node_cpy(cur_n);
          c = cur_n->cube;
          transition[op](c);
          cubes.insert(*(cur_n->cube));
        }
      }
      depth++;
    }

    else if (mpi_sta.MPI_TAG == UPDATE_TAG)
      bound = buf.bound;
    std::cout << "Bound is : " << bound << std::endl;
    nodeStack.push(node_cpy(path[depth - 1]));
    ans.bound = bounded_dfs(nodeStack, corner_db, path, &res_depth, cubes, bound);
    cur_n = path[res_depth];
    ans.depth = cur_n->d - 1;
    memcpy(ans.solution, cur_n->steps, MAX_DEPTH * sizeof(int));


    // TODO: RECV AFTER SEND
    flag = 0; 
    flag2 = 0;
    MPI_Isend(&ans, 1, mpi_ans_t2, 0, FINISH_TAG, MPI_COMM_WORLD, &irreq);
    // std::cout << "I'm here " << std::endl;
    do{
      // std::cout << "Not yet finish sending" << std::endl;
      MPI_Irecv(&ansWorkload, 1, MPI_INT, 0, REQUEST_WORKLOAD, MPI_COMM_WORLD, &irreqrec);
      MPI_Test(&irreqrec, &flag2, &mpi_sta2);
      if (flag2 != 0)
      {
        // std::cout << "detected!" << std::endl;
        ansWorkload=0;
        MPI_Send(&ansWorkload, 1, MPI_INT, 0, SIZE_TAG, MPI_COMM_WORLD);
      }

      MPI_Test(&irreq, &flag, &mpi_sta);
    } while(flag != 1);


    // idle processor waiting for work indefintely, if no work, terminate
    while (1) {
      std::cout << "Start receiving stealworkinfo: " << std::endl;

      MPI_Recv(stealWorkInfo, 2, MPI_INT, 0, IF_STEAL_TAG, MPI_COMM_WORLD, &mpi_sta);
      std::cout << "Finish receiving stealworkinfo: " << stealSize << std::endl;

      stealSize = stealWorkInfo[1];
      std::cout << "steal size is: " << stealSize << std::endl;

      if (stealWorkInfo[0] != -1) {
        MPI_Recv(stealBuf, stealSize, mpi_node_t, stealWorkInfo[0], SEND_WORK, MPI_COMM_WORLD, &mpi_sta);
        // convert received info to stack
        for (int i = 0; i < stealSize; i++) 
        { 
          cur_mpi_node = stealBuf[i];
          cur_n = node_new();
          *(cur_n->cube) = Cube(cur_mpi_node.cube);
          // cur_n->steps = cur_mpi_node.steps;
          memcpy(cur_n->steps, cur_mpi_node.steps, MAX_DEPTH * sizeof(int));
          cur_n->d = cur_mpi_node.d;
          stealNodeStack.push(cur_n);

        }

        // perform bounded dfs on stolen stack, same bound, same visited cubes, same path
        ans.bound = bounded_dfs(stealNodeStack, corner_db, path, &res_depth, cubes, bound);

        cur_n = path[res_depth];
        ans.depth = cur_n->d - 1;
        memcpy(ans.solution, cur_n->steps, MAX_DEPTH * sizeof(int));
        MPI_Send(&ans, 1, mpi_ans_t2, 0, FINISH_TAG, MPI_COMM_WORLD);

      }
      else {
        MPI_Send(NULL, 0, MPI_INT, 0, TERMINATION_TAG, MPI_COMM_WORLD);
        break;
      }
    }
  
  }
}


bool Solver::ida_solve_mpi_v3(cube_t *cube, int solution[MAX_DEPTH], int *num_steps)
{
  if (numProcessors == 1)
    ida_solve(cube, solution, num_steps);
  else
  {
    // std::cout << "This is here" << std::endl;

    MPI_Comm_size(MPI_COMM_WORLD, &numProcessors);
    MPI_Comm_rank(MPI_COMM_WORLD, &this->processorId);
    
    mpi_ans_t2 = createType_v2();
    mpi_node_t = createMpiNode();
    MPI_Barrier(MPI_COMM_WORLD);

    if (this->processorId == 0)
    {
      run_master_v3(&this->corner_db, cube, solution, num_steps);
    }
    else
      run_worker_v3(cube, &this->corner_db);

    return true;
  }
}