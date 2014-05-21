#include "lexer.hpp"
#include <stdio.h>

using namespace std;

namespace lexer {

  OperatorFSM operatorFSM =
    OperatorFSM(' ', NULL).addChildren(operatorPairs);


  bool isAlpha(char c) {
    if ('a' <= c && c <= 'z') {
      return true;
    } else if ('A' <= c && c <= 'Z') {
      return true;
    }
    return false;
  }

  bool isNumeric(char c) {
    return ('0' <= c && c <= '9');
  }

  bool isAlphaNumeric(char c) {
    return isAlpha(c) || isNumeric(c);
  }
}
