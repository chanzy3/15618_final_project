#ifndef INC_15618_FINAL_PROJECT_IDASOVLER_H
#define INC_15618_FINAL_PROJECT_IDASOVLER_H

#include "cube.h"
#include "debug.h"
#include "solver.h"
#include <vector>
#include <stack>
#include <iostream>
#include <array>
#include <queue>
#include <functional>
#include "PatternDatabase.h"

#define MAX_DEPTH 26 
#define DEPTH_LIMIT 4 

// bool ida_solver(cube_t *cube, int solution[MAX_DEPTH], int *num_steps);
class IDACubeSolver {
    struct Node {
        cube_t *iCube;
        uint8_t moveInd;
        uint8_t depth;
    };

    struct PrioritizedMove
    {
      cube_t cube;
      uint8_t moveInd;
      uint8_t estMoves; // Priority.  Least number of moves to most.
      bool operator>(const PrioritizedMove& rhs) const
      {
        return this->estMoves > rhs.estMoves;
      }
    };
    private:
        const busybin::PatternDatabase *pPatternDB;
    public:
        IDACubeSolver(const busybin::PatternDatabase *pPatternDB); //constructor
        std::vector<MOVE> findGoal(cube_t *cube);
};

// operation 0-17 (inclusive);
//const char *to_string(int operation);

#endif //INC_15618_FINAL_PROJECT_IDASOVLER_H