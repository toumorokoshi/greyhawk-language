#include "lexer.hpp"
#include <boost/tokenizer.hpp>
#include <stdio.h>
#include <iostream>
#include <assert.h>

using namespace std;

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

int main() {
  string input;
  TokenVector tokens;
  cout << "Greyhawk lexer." << endl;
  while(true) {
    cout << ">>> ";
    getline(cin, input);
    try {
      tokens = tokenize(input);
    } catch (LexerException& e) {
      cout << e.message << endl;
      continue;
    }
    for (TokenVector::iterator it = tokens.begin(); it != tokens.end(); ++it) {
      cout << it->name << endl;
    }
 }
}
