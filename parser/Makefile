all:
	mkdir -p build
	cp *.h build/
	bison -d -o build/parser.cpp parser.y
	flex -o build/tokens.cpp tokens.l
	clang++ -o parser `llvm-config-3.3 --libs core` `llvm-config-3.3 --cppflags --ldflags` build/*.cpp *.cpp 

clean:
	rm -r build
