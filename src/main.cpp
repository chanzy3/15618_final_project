#include <getopt.h>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>

#include "solver.h"
#include "cube.h"

void usage(const char* progname) {
  printf("Usage: %s [options]\n", progname);
  printf("Program Options:\n");
  printf("  -b  --bench                Benchmark mode\n");
  printf("  -f  --file  <FILENAME>     Input file name\n");
  printf("  -?  --help                 This message\n");
}


int main(int argc, char** argv) {
  bool benchmark = false;
  char *input_file_name = NULL;

  // parse commandline options ////////////////////////////////////////////
  int opt;
  static struct option long_options[] = {
      {"help",     0, 0,  '?'},
      {"bench",    0, 0,  'b'},
      {"file",     1, 0,  'f'},
      {0 ,0, 0, 0}
  };

  while ((opt = getopt_long(argc, argv, "bf:?", long_options, NULL)) != EOF) {
    switch (opt) {
      case 'b':
        benchmark = true;
        break;
      case 'f':
        input_file_name = optarg;
        break;
      case '?':
      default:
        usage(argv[0]);
        return 1;
    }
  }
  // end parsing of commandline options //////////////////////////////////////

  cube_t *cube = cube_new(true);

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

  FILE *f = fopen(input_file_name, "r");
  char option;
  char operation;
  fscanf(f, "%c", &option);
  switch (option) {
    case 'O':
      while (fscanf(f, "%c", &operation) == 1) {
        if (operation >= 'B' && operation <= 'Z') {
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

  int solution[MAX_DEPTH];
  int num_steps;
  if (!bfs_solve(cube, solution, &num_steps)) {
    fprintf(stderr, "did not find solution within %d steps\n", DEPTH_LIMIT);
    exit(0);
  }

  for (int i=0; i<num_steps; i++) {
    fprintf(stdout, "%s ", to_string(solution[i]));
  }
  fprintf(stdout, "\n");
}
