Greyhawk Language
=================

Greyhawk Language is a playground for a language that has the syntax I would like to use.

Installation
------------

The following tools are required:

* flex for lexer (apt-get flex)
* llvm headers + libraries (apt-get llvm-3.3-dev)
* sphinx for docs (pip install sphinx)

Building the compiler
---------------------

The Compiler uses the following:

* c++11
* llvm as the intermediary layer for the compiler
* gtest, for unit testing
* yaml-cpp to output the syntax tree in a human-readable format
* boost, for:
  * the command line parser
