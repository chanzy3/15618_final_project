// #include "Controller/RubiksCubeController.h"
// #include "Controller/Searcher/IDACubeSearcher.h"
// #include "../Util/ThreadPool.h"
#include "KorfCubeSolver.h"
#include "Model/RubiksCubeModel.h"

void apply(busybin::RubiksCube &cube, char operation) {
  switch(operation) {
    case 'F':
      cube.move(busybin::RubiksCube::MOVE::F);
      break;
    case 'B':
      cube.move(busybin::RubiksCube::MOVE::B);
      break;
    case 'L':
      cube.move(busybin::RubiksCube::MOVE::L);
      break;
    case 'R':
      cube.move(busybin::RubiksCube::MOVE::R);
      break;
    case 'U':
      cube.move(busybin::RubiksCube::MOVE::U);
      break;
    case 'D':
      cube.move(busybin::RubiksCube::MOVE::D);
      break;
    default:
      fprintf(stderr, "unknown operation '%c'\n", operation);
      exit(1);
  }
}

int main(int argc, char* argv[])
{
  busybin::KorfCubeSolver korfSolver;
  korfSolver.initialize();

  while(true) {
    busybin::RubiksCubeIndexModel iCube;
    char option;
    printf("$ ");
    if (fscanf(stdin, "%c", &option) != 1) {
      return 1;
    }
    switch (option) {
      case 'q':
        return 0;
      case 'f': {
        char fname[40];
        fscanf(stdin, " %s\n", fname);

        FILE *f = fopen(fname, "r");
        char o;
        char operation;
        fscanf(f, "%c", &o);
        switch (o) {
          case 'O':
            while (fscanf(f, "%c", &operation) == 1) {
              if (operation >= 'B' && operation <= 'Z') {
                apply(iCube, operation);
              }
            }
            break;
        }
        break;
      }
      case 'O': {
        char operation;
        while (fscanf(stdin, "%c", &operation) == 1) {
          if (operation >= 'B' && operation <= 'Z') {
            apply(iCube, operation);
          }
          if (operation == '\n') {
            break;
          }
        }
        break;
      }
    }
    iCube.dbg();
    korfSolver.solveCube(iCube);
  }

  //move(MOVE ind)
}


