# Modules

GreyHawk supports modules: units of code that can export values and functions,
and can be imported into other libraries as well.

source code is compiled into bytecode, which represents an unloaded module.

on import, the bytecode is executed, and the final module is then available.

in the future, there will be options that be set at the top of the file to
save the module fully executed, and to load that instead.



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


# How the file is loaded

* when a file is compiled, it becomes a module:
    execFile(fileName) -> GModule.

* execFile:

    * parser loads the file into a PBlock
    * compiles the file to a list of instructions + a module file.
    * executes the instructions.
    * returns the module.
