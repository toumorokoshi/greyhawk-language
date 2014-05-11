#include "../lexer/lexer.hpp"

using namespace std;
using namespace lexer;

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
      cout << it->getDescription() << endl;
    }
 }
}
