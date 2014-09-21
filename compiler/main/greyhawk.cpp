#include <iostream>
#include <stdio.h>
#include "../lexer/tokenizer.hpp"
#include "../vm/vm.hpp"
#include "../parser/parser.hpp"
#include <boost/program_options.hpp>
#include <sstream>
#include <fstream>

namespace po = boost::program_options;
using namespace lexer;
using namespace parser;
using namespace VM;



// these are initialized in main
static Tokenizer* tokenizer;
static VMScope* globalScope;

typedef struct CommandLineArguments {
  std::string fileName;
  bool ast;
  bool llvm;
} CommandLineArguments;

CommandLineArguments& getArguments(int argc, char*argv[]) {

  CommandLineArguments* args = new CommandLineArguments();
  args->ast = false;
  args->llvm = false;

  po::positional_options_description posixOptions;
  posixOptions.add("file_name", 1);
  po::options_description desc("Options");
  desc.add_options()
    ("help", "Print help message")
    ("ast", "print the ast")
    ("file_name", po::value<std::string>()->required(), "path to the file to compile");

  po::variables_map vm;

  try {
    po::store(po::command_line_parser(argc, argv)
              .options(desc)
              .positional(posixOptions)
              .run()
              , vm);

    if (vm.count("file_name") > 0) {
      args->fileName = vm["file_name"].as<std::string>();
    }

    args->ast = vm.count("ast") > 0;
    return *args;

  } catch (po::error& e) {
    std::cerr << "ERROR: " << e.what() << std::endl;
    std::cerr << desc << std::endl;
    exit(1);

  }
}

void interpreter() {
  std::string input;
  std::cout << "Greyhawk 0.0.1" << std::endl;
  while (true) {
    std::cout << ">> ";
    getline(std::cin, input);
    if (input.size() == 0) {
      break;
    }
    try {
      std::istringstream input_stream(input);
      TokenVector tokens = tokenizer->tokenize(input_stream);
      auto token_position = tokens.begin();
      Parser parser(globalScope, token_position, tokens);
      auto statement = parser.parseStatement();

      if (auto expression = dynamic_cast<VMExpression*>(statement)) {
        auto value = expression->evaluate(*globalScope);
        if (value != NULL) {
          VM::vm_print(*new std::vector<VMObject*>{value});
        }

      } else {
        statement->execute(*globalScope);

      }

      //parseTokens(tokens);
    } catch (LexerException& e) {
      std::cout << e.message << std::endl;
      continue;
    } catch (ParserException& e) {
      std::cout << e.message << std::endl;
      continue;
    } catch (VMException& e) {
      std::cout << e.message << std::endl;
      continue;
    }
  }
}

/* void handleTopLevelExpression(llvm::Function* function) {
  // function->dump();
  //void* f_pointer = executionEngine->getPointerToFunction(function);
  } */


int main(int argc, char *argv[]) {
  tokenizer = new Tokenizer();
  globalScope = new VMScope(getBuiltinScope());
  CommandLineArguments& args = getArguments(argc, argv);

  try {
    if (args.fileName != "") {
      std::ifstream input_stream(args.fileName);
      TokenVector tokens = tokenizer->tokenize(input_stream);
      auto token_position = tokens.begin();
      Parser parser(globalScope, token_position, tokens);
      auto rootBlock = parser.parseBlock();
      rootBlock->execute();

    } else {
      interpreter();
    }

  } catch (parser::ParserException& e) {
    std::cout << e.message << std::endl;
    exit(1);
  } catch (lexer::LexerException& e) {
    std::cout << e.message << std::endl;
    if (e.specMessage != "") {
      std::cout << std::endl << "details:" << std::endl;
      std::cout << e.specMessage << std::endl;
    }
    exit(1);
  }
  return 0;
}
