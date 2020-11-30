// #include "Controller/RubiksCubeController.h"
// #include "Controller/Searcher/IDACubeSearcher.h"
// #include "../Util/ThreadPool.h"
#include "KorfCubeSolver.h"
#include "Model/RubiksCubeModel.h"

int main(int argc, char* argv[])
{
  // busybin::RubiksCubeController ctlr;
  // ctlr.run();
  // return 0;
  busybin::RubiksCubeIndexModel iCube;
  iCube.move(busybin::RubiksCubeModel::MOVE::U);
  iCube.move(busybin::RubiksCubeModel::MOVE::U);
  iCube.move(busybin::RubiksCubeModel::MOVE::R);
  iCube.move(busybin::RubiksCubeModel::MOVE::R);
  busybin::KorfCubeSolver korfSolver;
  korfSolver.initialize();
  korfSolver.solveCube(iCube);

  //move(MOVE ind)
}

