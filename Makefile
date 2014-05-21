.PHONY: compiler lexer tests parser
export

all: compiler lexer tests

compiler:
	$(MAKE) -C compiler

lexer:
	$(MAKE) -C compiler lexer

parser:
	$(MAKE) -C compiler parser

tests:
	$(MAKE) -C compiler tests
