#include "fsm.hpp"

#ifndef LEXER_UTILS_HPP
#define LEXER_UTILS_HPP

namespace lexer {

  inline bool isAlpha(char c) {
    if ('a' <= c && c <= 'z') {
      return true;
    } else if ('A' <= c && c <= 'Z') {
      return true;
    }
    return false;
  }

  inline bool isNumeric(char c) {
    return '0' <= c && c <= '9';
  }

  inline bool isCapital(char c) {
    return 'A' <= c && c <= 'Z';
  }

  inline bool isAlphaNumeric(char c) {
    return isAlpha(c) || isNumeric(c);
  }

  inline bool isTokenBreakCharacter(char c) {
    static const std::vector<char> breakChar {' ', '\n', '\t'};

    for (auto it = breakChar.begin(); it != breakChar.end(); ++it) {
      if ((*it) == c) { return true; }
    }
    return false;
  }
}

#endif
