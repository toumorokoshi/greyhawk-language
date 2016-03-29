/*
   A start to a finite state machine. Eventually this may replace the standard lexer
 */
#include <string>
#include <map>
#include <vector>
#include "tokens.hpp"

#ifndef LEXER_FSM_HPP
#define LEXER_FSM_HPP

namespace lexer {

  typedef std::string::iterator StringIter;

  class FSMNode {

  private:
    void addChildInternal(StringIter it, StringIter end, L result);
    L getValueInternal(StringIter it, StringIter end);

  public:
    const char token;
    L value;
    std::map<char, FSMNode> children;
    FSMNode() : token(' '), value(BAD_TOKEN) {}
    FSMNode(char _token, L _value) :
      token(_token),
      value(_value) {}

    FSMNode addChildren(KeywordPairVector childrenPairs);
    bool hasChild(char childToken) { return children.find(childToken) != children.end(); }
    void addChild(std::string path, L result);
    L getValue(std::string path);
  };

}

#endif
