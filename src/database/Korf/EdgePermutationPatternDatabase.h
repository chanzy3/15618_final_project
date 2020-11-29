//
// Code adapted from https://github.com/benbotto/rubiks-cube-cracker.git
//

#ifndef INC_15618_FINAL_PROJECT_EDGEPERMUTATIONPATTERNDATABASE_H
#define INC_15618_FINAL_PROJECT_EDGEPERMUTATIONPATTERNDATABASE_H

#include "../PatternDatabase.h"
#include "../../cube.h"
#include "../../RubiksCubeIndexModel.h"
#include "../PatternDatabase.h"
#include <cstdint>
#include <bitset>
using std::bitset;
#include <array>
using std::array;

namespace paracube
{
  /**
   * A pattern database that holds the permutations of the 12 edges.
   */
  class EdgePermutationPatternDatabase : public PatternDatabase
  {
    typedef array<uint8_t, 12> perm_t;

    static array<uint8_t, 4096> onesCountLookup;

  public:
    EdgePermutationPatternDatabase();
    uint32_t getDatabaseIndex(const RubiksCube& cube) const;
  };
}

#endif

