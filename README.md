Greyhawk Language
=================

Greyhawk is a highly opinionated programming language. Some of the defining features are:

* runs on a virtual machine (e.g. Java)
* immutability by default
* tight restrictions around syntax

If you're interested in the why, please read the DESIGN.md page.

Building the Interpreter
------------------------

The interpreter is written in Rust, specifically version 1.7.0. Install rust, and build the
interpreter with:

    cargo build
