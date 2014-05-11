COMPILER_PATH=./bin/greyhawk
.PHONY: compiler tests
export

all: compiler tests

compiler:
	$(MAKE) -C compiler

tests:
	$(MAKE) -C compiler tests
