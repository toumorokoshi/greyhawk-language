LLVM_CONFIG=`llvm-config --libs core native mcjit interpreter x86` `llvm-config --cxxflags --ldflags` -fexceptions
OPTIONS=--std=c++11 -L/usr/lib -I/usr/include

all:
	clang++ *.cpp $(LLVM_CONFIG) $(OPTIONS) -o  ../../bin/vm2
