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

  // single character, non-alphanumeric characters
  Character* T_LPAREN = new Character('(');
  Character* T_RPAREN = new Character(')');
  Character* T_DOT = new Character('.');
  Character* T_COMMA = new Character(',');
  Character* T_PLUS = new Character('+');
  Character* T_MINUS = new Character('-');
  Character* T_MUL = new Character('*');
  Character* T_DIV = new Character('/');

  KeywordVector keywordList {
      T_ELSE,
      T_IF,
      T_IS,
      T_RETURN,
      T_FALSE,
      T_TRUE
  };

  CharacterVector characterList {
      T_LPAREN,
      T_RPAREN,
      T_DOT,
      T_COMMA,
      T_PLUS,
      T_MINUS,
      T_MUL,
      T_DIV
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
        // create an identifier token
        if (current_token.compare("") != 0) {
          tokens.push_back(new Identifier(current_token));
          current_token = "";
        }

        bool matching_token = false;
        for (CharacterVector::iterator it = characterList.begin(); it != characterList.end(); ++it) {
          if ((*it)->symbol == c) {
            matching_token = true;
            tokens.push_back(*it);
          }
        }

        if (!matching_token) {
          throw LexerException("Unable to find token matching " + string(1, c));
        }

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
