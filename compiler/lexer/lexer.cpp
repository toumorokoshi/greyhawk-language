#include "lexer.hpp"
#include <boost/tokenizer.hpp>
#include <stdio.h>
#include <iostream>
#include <assert.h>

using namespace std;

namespace lexer {

    // comparison operators
    /*
    Token T_ASSIGN = Token("assign", "=");
    Token T_EQ = Token("equals", "==");
    Token T_NE = Token("not equals", "!=");
    Token T_LT = Token("less than", "<");
    Token T_LEQ = Token("less than or equal", "<=");
    Token T_GT = Token("greater than", ">");
    Token T_GEQ = Token("greater than or equal", ">=");
    */

    // keywords
    Keyword* T_ELSE = new Keyword("else");
    Keyword* T_IF = new Keyword("if");
    Keyword* T_IS = new Keyword("is");
    Keyword* T_RETURN = new Keyword("return");

    // constants
    Keyword* T_FALSE = new Keyword("false");
    Keyword* T_TRUE = new Keyword("true");

   // boolean operations

    KeywordVector keywordList {
      T_ELSE,
        T_IF,
        T_IS,
        T_RETURN,
        T_FALSE,
        T_TRUE
    };

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

    TokenVector tokenize(string input) {

      TokenVector tokens;

      string current_token = "";

      for (char& c : input) {
        if (!isAlphaNumeric(c)) {
          throw LexerException("Can not handle non-alphanumeric characters!");
        } else {

          current_token += c;
          for (KeywordVector::iterator it = keywordList.begin(); it != keywordList.end(); ++it) {
            if (current_token.compare((*it)->symbol) == 0) {
              current_token = "";
              tokens.push_back(*it);
            }
          }

        }
      }

      if (current_token.compare("") != 0) {
        throw LexerException("invalid token: " + current_token);
      }
      return tokens;
    }

}