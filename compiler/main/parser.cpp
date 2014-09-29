#include "../exceptions.hpp"
#include "../lexer/tokenizer.hpp"
#include "../parser/parser.hpp"
#include <iostream>
#include <sstream>
#include <fstream>

using namespace std;
using namespace parser;
using namespace lexer;

void mainParseTokens(TokenVector& tokens) {
  Parser parser(tokens);
  auto block = parser.parseBlock();
  YAML::Node* yaml = block->toYaml();
  cout << (*yaml) << std::endl;
}

int main(int argc, char* argv[]) {
  string input;
  Tokenizer tokenizer;
  if (argc == 2) {
    string filename(argv[1]);
    ifstream input_stream(filename);
    TokenVector tokens = tokenizer.tokenize(input_stream);
    Parser parser(tokens);
    PBlock* mainBlock;
    try {
      mainBlock = parser.parseBlock();
    } catch (parser::ParserException& e) {
      cout << ((core::GreyhawkException) e).message << endl;
      exit(1);
    }
    YAML::Node* yaml = mainBlock->toYaml();
    cout << (*yaml) << std::endl;

  } else {
    cout << "Greyhawk parser." << endl;
    while(true) {
      cout << ">>> ";
      getline(cin, input);
      if (input.size() == 0) {
        break;
      }
      try {
        istringstream input_stream(input);
        TokenVector tokens = tokenizer.tokenize(input_stream);
        mainParseTokens(tokens);
      } catch (parser::ParserException& e) {
        cout << ((core::GreyhawkException) e).message << endl;
        continue;
      }
    }
  }
}
