Greyhawk Language
=================

Greyhawk is a highly opinionated programming language. Some of the defining features are:

* runs on a virtual machine (e.g. Java)
* immutability by default
* tight restrictions around syntax

If you're interested in the why, please read the DESIGN.md page.

Building the compiler
---------------------

The Compiler uses the following:

* c++11
* llvm as the intermediary layer for the compiler
* gtest, for unit testing
* yaml-cpp to output the syntax tree in a human-readable format
* boost, for:
  * the command line parser

Installation
------------

The following tools are required:

* flex for lexer (apt-get flex)
* llvm headers + libraries (apt-get llvm-3.3-dev)
* sphinx for docs (pip install sphinx)
