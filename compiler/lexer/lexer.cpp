#include "lexer.hpp"
#include <boost/tokenizer.hpp>
#include <stdio.h>
#include <iostream>
#include <assert.h>

using namespace std;

Token T_FOO = Token("foo", "testing");
Token T_BAR = Token("bar", "none");
Token T_NOTHING = Token("null", " ", true);

TokenVector tokensByPrecedence {
  T_FOO,
  T_BAR,
  T_NOTHING
};

TokenVector tokenize(string input) {

  TokenVector tokens;

  string current_token = "";

  for (char& c : input) {
    current_token += c;

    for(TokenVector::iterator it = tokensByPrecedence.begin(); it != tokensByPrecedence.end(); ++it) {
      boost::smatch token_matches;
      if(regex_match(current_token, token_matches, it->pattern)) {

        assert (token_matches.size() == 1);

        if (!it->ignore) {
          tokens.push_back(*it);
        }
        // we should only have one match here. If we don't there's probably a bug.
        current_token = "";
      }
    }
  }

  if (current_token.compare("") != 0) {
    throw LexerException("invalid token: " + current_token);
  }
  return tokens;
}
