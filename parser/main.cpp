#include <iostream>
#include <stdio.h>
#include "codegenerator.hpp"
#include "yamlast.hpp"
#include "node.hpp"
#include <boost/program_options.hpp>

namespace po = boost::program_options;

extern NBlock* programBlock;
extern int yyparse();
extern FILE* yyin;

typedef struct CommandLineArguments {
  std::string file_name;
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
    ("llvm", po::value<bool>(&(args->llvm)), "print llvm IR code")
    ("ast", po::value<bool>(&(args->ast)), "print the ast")
    ("file_name", po::value<std::string>()->required(), "path to the file to compile");

  po::variables_map vm;
  
  try {
    po::store(po::command_line_parser(argc, argv)
              .options(desc)
              .positional(posixOptions)
              .run()
              , vm);
    args->file_name = vm["file_name"].as<std::string>();
    return *args;
  } catch (po::error& e) {
    std::cerr << "ERROR: " << e.what() << std::endl;
    std::cerr << desc << std::endl;
  }
}


int main(int argc, char *argv[]) {
  std::cout << "starting..." << std::endl;
  CommandLineArguments& args = getArguments(argc, argv);
  // set yyin before yyparse
  std::cout << args.file_name << std::endl;
  std::cout << "loading file..." << std::endl;
  yyin = fopen(args.file_name.c_str(), "r");
  yyparse();

  std::cout << "processing..." << std::endl;
  if (args.ast) {
    YamlAST astGenerator;
    YAML::Node* tree = astGenerator.generateTree(*programBlock);
    std::cout << (*tree) << std::endl;
  } else {
    CodeGenerator generator;
    generator.generateCode(*programBlock);
  }
  return 0;
}
