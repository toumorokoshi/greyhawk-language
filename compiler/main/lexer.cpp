#include "../lexer/lexer.hpp"

using namespace std;
using namespace lexer;

int main() {
  string input;
  Tokenizer tokenizer;
  TokenVector tokens;
  cout << "Greyhawk lexer." << endl;
  while(true) {
    cout << ">>> ";
    getline(cin, input);
    try {
      tokens = tokenizer.tokenize(input);
    } catch (LexerException& e) {
      cout << e.message << endl;
      continue;
    }
    cout << tokens.size() << " tokens found." << endl;
    for (TokenVector::const_iterator it = tokens.begin(); it != tokens.end(); ++it) {
      cout << (*it)->getDescription() << endl;
    }
 }
}
