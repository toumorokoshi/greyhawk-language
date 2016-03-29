.PHONY: compiler lexer tests parser vm
export

all: compiler lexer tests

compiler:
	$(MAKE) -C compiler compiler

lexer:
	$(MAKE) -C compiler lexer

parser:
	$(MAKE) -C compiler parser

tests:
	$(MAKE) -C compiler tests

vm:
	$(MAKE) -c compiler vm2
