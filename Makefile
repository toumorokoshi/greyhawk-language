.PHONY: compiler lexer
export

all: compiler lexer

compiler:
	$(MAKE) -C compiler

lexer:
	$(MAKE) -C compiler lexer
