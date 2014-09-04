SHELL := /bin/bash
LLVM_CONFIG=`llvm-config --libs core native mcjit interpreter x86` `llvm-config --cxxflags --ldflags` -fexceptions
BOOST_OPTIONS=-lboost_program_options
YAML_OPTIONS=-lyaml-cpp
OPTIONS=--std=c++11 -L/usr/lib -I/usr/include
.PHONY: lexer parser compiler

# deprecated, lexers and parsers are by hand now
all:
	mkdir -p build
	cp *.hpp build/
	cp *.h build/
# create parser
	bison -d -o build/parser.cpp parser.y
# create lexer
	flex -o build/tokens.cpp tokens.l
#	clang++ -o parser -lyaml-cpp -lLLVMCore -lLLVMSupport -I/usr/local/Cellar/llvm/3.3/include  -I/usr/local/include/ -D_DEBUG -D_GNU_SOURCE -D__STDC_CONSTANT_MACROS -D__STDC_FORMAT_MACROS -D__STDC_LIMIT_MACROS -L/usr/local/Cellar/llvm/3.3/lib -lz -lpthread -lm build/*.cpp *.cpp
# clang++ -o parser build/*.cpp *.cpp `llvm-config-3.3 --libs core native` `llvm-config-3.3 --cxxflags --ldflags`
# arch builder
	clang++ -o ../bin/greyhawk -L/usr/lib main/*.cpp build/*.cpp *.cpp `llvm-config --libs core native` `llvm-config --cxxflags --ldflags` -lyaml-cpp -I/usr/include/ -fexceptions -lboost_program_options

LEXER_FILES=lexer/*.cpp

lexer:
	clang++ $(LEXER_FILES) $(OPTIONS) main/lexer.cpp -o ../bin/lexer

PARSER_FILES=parser/*.cpp

parser:
	clang++ main/parser.cpp $(LEXER_FILES) $(PARSER_FILES) $(YAML_OPTIONS) $(OPTIONS) -o ../bin/parser

CODEGEN_FILES=codegen/*.cpp

# note: llvm_config must come after source files
compiler:
	clang++ main/greyhawk.cpp $(LEXER_FILES) $(PARSER_FILES) $(YAML_OPTIONS) $(BOOST_OPTIONS) $(OPTIONS) $(CODEGEN_FILES) $(LLVM_CONFIG) -o ../bin/greyhawk

tests:
	clang++ -o ../bin/unit_tests $(LEXER_FILES) $(PARSER_FILES) vm/*.cpp lexer/tests/*.cpp vm/tests/*.cpp main/tests.cpp $(OPTIONS) -lgtest $(YAML_OPTIONS)

clean:
	rm -r build
