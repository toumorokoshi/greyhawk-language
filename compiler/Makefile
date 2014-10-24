SHELL := /bin/bash
# remember, libs go AFTER source files
LLVM_CONFIG=`llvm-config --libs core native mcjit interpreter x86` `llvm-config --cxxflags --ldflags` -fexceptions
BOOST_OPTIONS=-lboost_program_options
YAML_OPTIONS=-lyaml-cpp
OPTIONS=--std=c++11 -L/usr/lib -I/usr/include
.PHONY: lexer parser compiler vm2 tests

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

VM_FILES=vm/*.cpp

parser:
	clang++ main/parser.cpp $(VM_FILES) $(LEXER_FILES) $(PARSER_FILES) $(YAML_OPTIONS) $(OPTIONS) -o ../bin/parser

# note: llvm_config must come after source files
compiler:
	clang++ main/greyhawk.cpp $(LEXER_FILES) $(PARSER_FILES) $(YAML_OPTIONS) $(BOOST_OPTIONS) $(OPTIONS) $(VM_FILES) -g -o ../bin/greyhawk

GTEST_FILES=./gtest/lib/.libs/libgtest*.a -I./gtest/include

tests:
	clang++ -g tests/**/*.cpp -o ../bin/unit_tests $(OPTIONS) $(GTEST_FILES) $(VM_FILES)

clean:
	rm -r build

vm:
	$(MAKE) -c vm2 all
