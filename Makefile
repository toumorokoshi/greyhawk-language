.PHONY: compiler lexer tests
export

all: compiler lexer tests

compiler:
	$(MAKE) -C compiler

lexer:
	$(MAKE) -C compiler lexer

tests:
	$(MAKE) -C compiler tests
