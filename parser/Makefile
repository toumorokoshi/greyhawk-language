all:
	mkdir -p build
	cp node.h build/
	bison -d -o build/parser.cpp parser.y
	flex -o build/tokens.cpp tokens.l
	g++ -o parser build/parser.cpp build/tokens.cpp main.cpp -I/usr/lib/llvm-2.9/include  -DNDEBUG -D_GNU_SOURCE -D__STDC_LIMIT_MACROS -D__STDC_CONSTANT_MACROS

clean:
	rm -r build
