//
// Code adapted from https://github.com/benbotto/rubiks-cube-cracker.git
//

#ifndef INC_15618_FINAL_PROJECT_EDGEG1PATTERNDATABASE_H
#define INC_15618_FINAL_PROJECT_EDGEG1PATTERNDATABASE_H

#include "../../cube.h"
#include "../../RubiksCubeIndexModel.h"
#include "EdgePatternDatabase.h"
#include <cstdint>
#include <stdint.h>


namespace paracube
{
  /**
   * A database for 7 of the 12 edges that indexes edge positions and
   * the number of moves required to get the edges to the solved state.  The 7
   * edges are UB, UR, UF, UL, FR, FL, and BL.
   */
  class EdgeG1PatternDatabase : public EdgePatternDatabase
  {
  public:
    uint32_t getDatabaseIndex(const RubiksCube& cube) const;
  };
}

#endif
