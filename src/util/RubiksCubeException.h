//
// Code adapted from https://github.com/benbotto/rubiks-cube-cracker.git
//

#ifndef INC_15618_FINAL_PROJECT_RUBIKSCUBEEXCEPTION_H
#define INC_15618_FINAL_PROJECT_RUBIKSCUBEEXCEPTION_H

#include <exception>
using std::exception;
#include <string>
using std::string;

namespace paracube
{
  class RubiksCubeException : public exception
  {
    string msg;

    RubiksCubeException();
  public:
    RubiksCubeException(const char* const what);
    RubiksCubeException(const string& what);
    virtual const char* what() const throw();
    virtual ~RubiksCubeException() throw();
  };
}

#endif

