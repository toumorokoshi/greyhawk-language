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

#ifdef DEBUG
  #define debug(s) std::cerr << s << std::endl;
#else
  #define debug(s);
#endif

// these are initialized in main
static Tokenizer* tokenizer;
static GEnvironment* globalScope;
static GEnvironmentInstance* globalScopeInstance;
static GVM* vm;

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

// for debug purposes mainly
void printValues() {
  for (auto symbol : globalScope->localsTable) {
    auto name = symbol.first;
    auto object = globalScopeInstance->locals[symbol.second];
    auto type = globalScope->localsTypes[symbol.second];
    std::cout << name << ": ";
    if (type == getNoneType()) {
      std::cout << "none";
    } else if (type == getBoolType()) {
      std::cout << "bool " << (object.asBool ? "true" : "false");
    } else if (type == getFloatType()) {
      std::cout << "float " << object.asFloat;
    } else if (type == getInt32Type()) {
      std::cout << "int32 " << object.asInt32;
    } else if (type == getStringType()) {
      std::cout << "string ";
      auto str = object.asArray;
      auto elements = str->elements;
      for (int i = 0; i < str->size; i++) {
        printf("%c", elements[i].asChar);
      }
    }
    std::cout << std::endl;
  }
}

GValue run(CommandLineArguments& args, std::istream& input_stream) {
  debug("tokenizing...");
  TokenVector tokens = tokenizer->tokenize(input_stream);
  debug("tokenized!");
  Parser parser(tokens);
  debug("parsing block...!");
  auto pBlock = parser.parseBlock();
  debug("parsed!");

  if (args.ast) {
    dumpAST(pBlock);
  } else {
    int oldLocalsCount = globalScope->localsCount;
    auto instructions = generateRoot(globalScope, pBlock);
    debug("parsed.");

    if (args.bytecode) {
      for (auto typeKV: globalScope->typeByName) {
        std::cout << typeKV.first << ":" << std::endl;

        for (auto functionKV: typeKV.second->environment->functionByName) {
          std::cout << functionKV.first << " (" << functionKV.second << "):" << std::endl;
          printInstructions(functionKV.second->instructions);
          std::cout << std::endl;
        }
      }

      debug("printing bytecode.");
      for (auto functionKV: globalScope->functionByName) {
        std::cout << functionKV.first << " (" << functionKV.second << "):" << std::endl;
        printInstructions(functionKV.second->instructions);
        std::cout << std::endl;
      }
      std::cout << "main:" << std::endl;
      printInstructions(instructions);
      return {0};
    } else {
      debug("executing code.");
      // for now, we build temp registers
      auto registers = new GValue[globalScope->localsCount];

      // copy values into new longer register array if necessary
      for (int i = 0; i < oldLocalsCount; i++) {
        registers[i] = globalScopeInstance->locals[i];
      }

      globalScopeInstance->locals = registers;
      return executeInstructions(vm->modules, instructions, *globalScopeInstance);
    }

  }
}


void interpreter(CommandLineArguments& args) {
  std::string input;
  std::cout << "Greyhawk 0.0.2" << std::endl;
  while (true) {
    std::cout << ">> ";
    getline(std::cin, input);
    if (input.size() == 0) {
      break;
    }
    try {
      std::istringstream input_stream(input);
      auto returnValue = run(args, input_stream);

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
  globalScope = getBaseEnvironment().createChild();
  globalScopeInstance = \
    globalScope->createInstance(getBaseEnvironmentInstance());
  vm = new GVM();
  vm->modules = new GModules();
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
