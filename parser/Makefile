SHELL := /bin/bash

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
	clang++ -o murasaki -L/usr/lib build/*.cpp *.cpp `llvm-config --libs core native` `llvm-config --cxxflags --ldflags` -lyaml-cpp -I/usr/include/ -fexceptions -lboost_program_options

clean:
	rm -r build
