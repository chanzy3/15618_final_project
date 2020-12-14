#include <getopt.h>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>

#include "cube.h"
#include "debug.h"
#include "solver.h"

// int processorId;
int numProcessors;
void usage(const char *progname)
{
  printf("Usage: %s [options] method \n", progname);
  printf("Valid methods are: BFS, IDA\n");
  printf("Program Options:\n");
  printf("  -f  --file  <FILENAME>     Input file name\n");
  printf("  -?  --help                 This message\n");
}

int main(int argc, char **argv)
{

  char *input_file_name = NULL;
  char *solverName = NULL;
  enum method method = BFS;

  // parse commandline options ////////////////////////////////////////////
  int opt;
  static struct option long_options[] = {
      {"help", 0, 0, '?'},
      {"file", 1, 0, 'f'},
      {0, 0, 0, 0}};

  while ((opt = getopt_long(argc, argv, "p:f:?", long_options, NULL)) != EOF)
  {
    switch (opt)
    {
    case 'f':
      input_file_name = optarg;
      break;
    case 'p':
      numProcessors = atoi(optarg);
      break;
    case '?':
    default:
      usage(argv[0]);
      return 1;
    }
  }
  // end parsing of commandline options //////////////////////////////////////

  if (optind + 1 > argc)
  {
    fprintf(stderr, "Error: missing solver name\n");
    usage(argv[0]);
    return 1;
  }

  solverName = argv[optind];
  //printf(solverName);
  if (strcmp(solverName, "BFS") == 0)
  {
    method = BFS;
  }
  else if (strcmp(solverName, "IDA") == 0)
  {
    method = IDA;
  }
  else if (strcmp(solverName, "IDA_MPI") == 0)
  {
    method = IDA_MPI;
  }
  else if (strcmp(solverName, "IDA_MPI2") == 0)
  {
    method = IDA_MPI2;
  }
  else if (strcmp(solverName, "IDA_MPI3") == 0)
  {
    method = IDA_MPI3;
  }
  else if (strcmp(solverName, "IDA_MPI4") == 0)
  {
    method = IDA_MPI4;
  }
  else
  {
    fprintf(stderr, "Unknown method name (%s)\n", solverName);
    usage(argv[0]);
    return 1;
  }

  // start parsing input file for cube spec //////////////////////////////////////
  //
  // input file formats
  //
  // 1. (6 x 3 x 3)
  // C
  // 1 2 3
  // 4 5 6
  // 7 8 9
  // ...
  //
  //  2. operations
  // O
  // F F B ....

  cube_t *cube = cube_new(true);

  FILE *f = fopen(input_file_name, "r");
  char option;
  char operation;
  fscanf(f, "%c", &option);
  switch (option)
  {
  case 'O':
    while (fscanf(f, "%c", &operation) == 1)
    {
      if (operation >= 'B' && operation <= 'Z')
      {
        apply(cube, operation);
      }
    }
    break;
  case 'C':
    // TODO(tianez):
    break;
  default:
    fprintf(stderr, "invalid input file option '%c'\n", option);
    exit(1);
  }
  // end parsing input file for cube spec //////////////////////////////////////
  if (method == IDA_MPI || method == IDA_MPI2 || method == IDA_MPI3 || method == IDA_MPI4)
    MPI_Init(&argc, &argv);

  Solver solver(method, numProcessors);

  // start execution and benchmark /////////////////////////////////////////////
  int solution[MAX_DEPTH];
  int num_steps;

  
  solver.solve(cube, solution, &num_steps);
  if (method == IDA_MPI || method == IDA_MPI2 || method == IDA_MPI3 || method == IDA_MPI4)
    MPI_Finalize();
}
