SHELL := /bin/bash

all:
	mkdir -p build
	cp *.h build/
	bison -d -o build/parser.cpp parser.y
	flex -o build/tokens.cpp tokens.l
#	clang++ -o parser -lLLVMCore -lLLVMSupport -I/usr/local/Cellar/llvm/3.3/include  -D_DEBUG -D_GNU_SOURCE -D__STDC_CONSTANT_MACROS -D__STDC_FORMAT_MACROS -D__STDC_LIMIT_MACROS -L/usr/local/Cellar/llvm/3.3/lib -lz -lpthread -lm build/*.cpp *.cpp 
	clang++ -o parser build/*.cpp *.cpp `llvm-config-3.3 --libs core` `llvm-config-3.3 --cxxflags --ldflags` 

clean:
	rm -r build
