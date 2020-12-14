//
// Code adapted from https://github.com/benbotto/rubiks-cube-cracker.git
//

#ifndef INC_15618_FINAL_PROJECT_EDGEPATTERNDATABASE_H
#define INC_15618_FINAL_PROJECT_EDGEPATTERNDATABASE_H

#include "../../cube.h"
#include "../PatternDatabase.h"
#include <cstdint>
#include <bitset>
using std::bitset;
#include <stdint.h>


namespace paracube
{
  /**
   * A base class for the two edge pattern databases.
   */
  class EdgePatternDatabase : public PatternDatabase
  {
  protected:
    typedef array<uint8_t, 7> perm_t;
    typedef RubiksCube::FACE F;

    static array<uint8_t, 4096> onesCountLookup;

    uint32_t getDatabaseIndex(const perm_t& edgePerm,
      const array<uint8_t, 7>& edgeOrientations) const;
  public:
    EdgePatternDatabase();
    virtual uint32_t getDatabaseIndex(const RubiksCube& cube) const = 0;
  };
}

#endif
