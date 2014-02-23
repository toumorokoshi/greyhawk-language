Compiling
=========

The 'greyhawk build' command build files. There are some options associated with it:

target
------

The 'target' directive determines whether the build is designed for debugging or production::

    --target=debug
    --target=production

Production provides a set of optimizations that are designed to make
code run faster. Debug will compile all code, including code that can
be optimized away. In production mode specifically:

* any constants that can be factored out will be removed from the global context
* any code paths that are never run will be factored out
