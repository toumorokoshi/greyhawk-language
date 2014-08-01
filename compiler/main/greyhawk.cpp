#include <iostream>
#include <stdio.h>
#include "../lexer/tokenizer.hpp"
#include "../parser/yamlast.hpp"
#include "../parser/parser.hpp"
#include "../codegen/jit.hpp"
#include "../codegen/codegenerator.hpp"
#include "../codegen/exceptions.hpp"
#include <boost/program_options.hpp>
#include <sstream>
#include <fstream>

namespace po = boost::program_options;
using namespace std;
using namespace lexer;
using namespace parser;

// static llvm::ExecutionEngine *executionEngine;
// these are initialized in main
static Tokenizer* tokenizer;
static codegen::JIT* jit;

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
    ("llvm", "print llvm IR code")
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

void parseTokens(TokenVector& tokens) {
  auto token_position = tokens.begin();
  YAML::Node* yaml;
  try {
    auto node = parser::parseStatement(token_position, tokens);
    yaml = YamlAST::generate(*node);
  } catch (ParserException) {
    auto node = parser::parseExpression(token_position, tokens);
    yaml = YamlAST::generate(*node);
  }
  cout << (*yaml) << std::endl;
}

void interpreter() {
  string input;
  std::cout << "Greyhawk 0.0.1" << std::endl;
  while (true) {
    std::cout << ">> ";
    getline(cin, input);
    if (input.size() == 0) {
      break;
    }
    try {
      istringstream input_stream(input);
      TokenVector tokens = tokenizer->tokenize(input_stream);
      auto token_position = tokens.begin();
      auto node = parser::parseExpression(token_position, tokens);
      jit->executeExpression(node);
      //parseTokens(tokens);
    } catch (LexerException& e) {
      cout << e.message << endl;
      continue;
    } catch (ParserException& e) {
      cout << e.message << endl;
      continue;
    } catch (codegen::CodeGenException& e) {
      cout << e.message << endl;
      continue;
    }
  }
}

/* void handleTopLevelExpression(llvm::Function* function) {
  // function->dump();
  //void* f_pointer = executionEngine->getPointerToFunction(function);
  } */


int main(int argc, char *argv[]) {
  // THIS MUST BE CALLED
  // BEFORE LLVM RUNS CODE
  llvm::InitializeNativeTargetAsmPrinter();
  llvm::InitializeNativeTargetAsmParser();
  tokenizer = new Tokenizer();
  jit = new codegen::JIT();
  CommandLineArguments& args = getArguments(argc, argv);

  try {
    if (args.fileName != "") {
      ifstream input_stream(args.fileName);
      TokenVector tokens = tokenizer->tokenize(input_stream);
      auto token_position = tokens.begin();
      auto node = parser::parseBlock(token_position, tokens);

      if (args.ast) {
        YamlAST astGenerator;
        YAML::Node* tree = astGenerator.generateTree(*node);
        std::cout << (*tree) << std::endl;
      } else {
        jit->runBlock(*node);
      }
    } else {
      interpreter();
    }

  } catch (codegen::CodeGenException& e) {
    cout << e.message << endl;
    exit(1);
  } catch (parser::ParserException& e) {
    cout << e.message << endl;
    exit(1);
  }
  return 0;
}
