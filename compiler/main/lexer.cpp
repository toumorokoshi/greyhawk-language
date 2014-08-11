/*
   This generates the 'lexer' executable under 'bin'.
   It brings up a prompt that will read in a string and return a proper set of tokens
 */
#include "../lexer/tokenizer.hpp"
#include <sstream>
#include <fstream>

using namespace std;
using namespace lexer;

void parseTokens(TokenVector& tokens) {
  cout << tokens.size() << " tokens found." << endl;
  for (TokenVector::const_iterator it = tokens.begin(); it != tokens.end(); ++it) {
    cout << (*it)->getFullDescription() << endl;
  }
}

int main(int argc, char* argv[]) {
  string input;
  Tokenizer tokenizer;
  if (argc == 2) {
    string filename(argv[1]);
    ifstream input_stream(filename);
    TokenVector tokens = tokenizer.tokenize(input_stream);
    parseTokens(tokens);
  } else {
    cout << "Greyhawk lexer." << endl;
    while(true) {
      cout << ">>> ";
      getline(cin, input);
      if (input.size() == 0) {
        break;
      }
      try {
        istringstream input_stream(input);
        TokenVector tokens = tokenizer.tokenize(input_stream);
        parseTokens(tokens);
      } catch (LexerException& e) {
        cout << e.message << endl;
        continue;
      }
    }
  }
}
