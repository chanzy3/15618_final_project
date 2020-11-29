//
// Code adapted from https://github.com/benbotto/rubiks-cube-cracker.git
//

#ifndef INC_15618_FINAL_PROJECT_EDGEG2PATTERNDATABASE_H
#define INC_15618_FINAL_PROJECT_EDGEG2PATTERNDATABASE_H

#include "../../cube.h"
#include "../../RubiksCubeIndexModel.h"
#include "EdgePatternDatabase.h"
#include <cstdint>

namespace paracube
{
  /**
   * See EdgeG1PatternDatabase.h.  This indexes the other 7 edges: FL, BL, BR, DF,
   * DL, DB, and DR.
   */
  class EdgeG2PatternDatabase : public EdgePatternDatabase
  {
  public:
    uint32_t getDatabaseIndex(const RubiksCube& cube) const;
  };
}

#endif
