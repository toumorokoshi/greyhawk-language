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
    try {
      string filename(argv[1]);
      ifstream input_stream(filename);
      TokenVector tokens = tokenizer.tokenize(input_stream);
      Parser parser(tokens);
      PBlock* mainBlock;
      mainBlock = parser.parseBlock();
      YAML::Node* yaml = mainBlock->toYaml();
      cout << (*yaml) << std::endl;
    } catch (parser::ParserException& e) {
      cout << ((core::GreyhawkException) e).message << endl;
      exit(1);
    } catch (lexer::LexerException& e) {
      std::cout << e.message << std::endl;
      if (e.specMessage != "") {
        std::cout << std::endl << "details:" << std::endl;
        std::cout << e.specMessage << std::endl;
      }
      exit(1);
    }

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
      } catch (lexer::LexerException& e) {
        std::cout << e.message << std::endl;
        if (e.specMessage != "") {
          std::cout << std::endl << "details:" << std::endl;
          std::cout << e.specMessage << std::endl;
        }
        continue;
      }
      exit(1);
    }
  }
}
