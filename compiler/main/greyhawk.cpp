#include <iostream>
#include <stdio.h>
#include "../lexer/tokenizer.hpp"
#include "../vm/vm.hpp"
#include "../vm/execution_engine.hpp"
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
static GScope* globalScope;

typedef struct CommandLineArguments {
  std::string fileName;
  bool ast;
  bool bytecode;
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
    ("bytecode", "print the bytecode")
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
    args->bytecode = vm.count("bytecode") > 0;
    return *args;

  } catch (po::error& e) {
    std::cerr << "ERROR: " << e.what() << std::endl;
    std::cerr << desc << std::endl;
    exit(1);

  }
}

void dumpAST(PNode* node) {
  auto yaml = node->toYaml();
  std::cout << (*yaml) << std::endl;
}

void run(CommandLineArguments& args, std::istream& input_stream) {
  TokenVector tokens = tokenizer->tokenize(input_stream);
  Parser parser(tokens);
  auto pBlock = parser.parseBlock();

  if (args.ast) {
    dumpAST(pBlock);
  } else {
    auto instructions = generateRoot(globalScope, pBlock);

    if (args.bytecode) {
      for (auto function : globalScope->functionTable) {
        std::cout << function.first << " (" << function.second << "):" << std::endl;
        printInstructions(function.second->instructions);
        std::cout << std::endl;
      }
      std::cout << "main:" << std::endl;
      printInstructions(instructions);
    } else {
      // for now, we build temp registers
      GValue registers[globalScope->frame->registerCount];
      executeInstructions(instructions, registers);
    }

  }
}


void interpreter(CommandLineArguments& args) {
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
      run(args, input_stream);

   } catch (LexerException& e) {
      std::cout << e.message << std::endl;
      continue;
    } catch (ParserException& e) {
      std::cout << e.message << std::endl;
      continue;
    }
  }
}

int main(int argc, char *argv[]) {
  tokenizer = new Tokenizer();
  globalScope = new GScope(new GFrame());
  CommandLineArguments& args = getArguments(argc, argv);

  try {
    if (args.fileName != "") {
      std::ifstream input_stream(args.fileName);
      run(args, input_stream);

    } else {
      interpreter(args);
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
