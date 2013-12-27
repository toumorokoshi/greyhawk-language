all:
	mkdir -p build
	cp *.h build/
	bison -d -o build/parser.cpp parser.y
	flex -o build/tokens.cpp tokens.l
	g++ -o parser `llvm-config --libs core jit native --cxxflags --ldflags` build/*.cpp *.cpp 

clean:
	rm -r build
