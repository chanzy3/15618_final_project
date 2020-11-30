#ifndef _BUSYBIN_TEST_GOAL_H_
#define _BUSYBIN_TEST_GOAL_H_

#include "Goal.h"
#include "../RubiksCube.h"
#include "../RubiksCubeModel.h"
#include <cstdint>

namespace busybin
{
  /**
   * Test goal for checking solvers.
   */
  class TestGoal : public Goal
  {
  public:
    bool isSatisfied(RubiksCube& cube);
    string getDescription() const;
  };
}

#endif

