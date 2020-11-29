//
// Code adapted from https://github.com/benbotto/rubiks-cube-cracker.git
//

#include "RubiksCubeException.h"

namespace paracube
{
  /**
   * Ctor from const char.
   * @param what The message.
   */
  RubiksCubeException::RubiksCubeException(const char* const what)
  {
    this->msg = what;
  }

  /**
   * Ctor from string.
   * @param what The message.
   */
  RubiksCubeException::RubiksCubeException(const string& what)
  {
    this->msg = what;
  }

  /**
   * Get the exception message.
   */
  const char* RubiksCubeException::what() const throw()
  {
    return this->msg.c_str();
  }

  /**
   * Do-nothing deconstructor.
   */
  RubiksCubeException::~RubiksCubeException() throw() {}
}

