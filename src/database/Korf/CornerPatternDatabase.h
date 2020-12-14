//
// Code adapted from https://github.com/benbotto/rubiks-cube-cracker.git
//

#ifndef INC_15618_FINAL_PROJECT_CORNERPATTERNDATABASE_H
#define INC_15618_FINAL_PROJECT_CORNERPATTERNDATABASE_H

#include "../../cube.h"
#include "../PatternDatabase.h"
#include <cstdint>
#include <bitset>
#include <stdint.h>
#include <array>

using std::bitset;

namespace paracube
{
  /**
   * This class provides a map of all the possible scrambles of the 8 corner
   * cubies to the number of moves required to get the solved state.
   */
  class CornerPatternDatabase : public PatternDatabase
  {
    typedef array<uint8_t, 8> perm_t;
    // typedef RubiksCube::FACE F;

    array<uint8_t, 256> onesCountLookup;

  public:
    CornerPatternDatabase();
    uint32_t getDatabaseIndex(const Cube& cube) const;
  };
}

#endif
