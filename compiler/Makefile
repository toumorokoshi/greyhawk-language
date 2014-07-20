SHELL := /bin/bash
LLVM_CONFIG=`llvm-config --libs core native` `llvm-config --cxxflags --ldflags`
BOOST_OPTIONS=-lboost_program_options -lboost_regex
OPTIONS=--std=c++11 -L/usr/lib -I/usr/include $(BOOST_OPTIONS)
.PHONY: lexer parser

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
	clang++ -o ../bin/lexer $(LEXER_FILES) main/lexer.cpp $(OPTIONS)

PARSER_FILES=parser/*.cpp

parser:
	clang++ -o ../bin/parser $(LEXER_FILES) $(PARSER_FILES) yamlast.cpp main/parser.cpp -lyaml-cpp $(OPTIONS)

tests:
	clang++ -o ../bin/unit_tests $(LEXER_FILES) $(PARSER_FILES) lexer/tests/*.cpp parser/tests/*.cpp main/tests.cpp $(OPTIONS) -lgtest

clean:
	rm -r build
