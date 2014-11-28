# LOAD_MODULE = map access on module heap
# LOAD_MODULE_VALUE = map access on module

LOAD_MODULE "sys" -> {0}
LOAD_MODULE_VALUE {0} "write" -> {1}
CALL {1} "foo"

How is a module built?

to build a module:

- lex + parse module source -> instruction set
- keep scope to map keys to values
- execute the code within the module
- iterate through the scope, retrieve the values from the
  registers, and create a module map.
