================
The VM, in-depth
================

This page explains some points about the VM.

There are a few areas in the layout of a greyhawk vm application:

---------------------
The Instruction Space
---------------------

The instruction space contains:

* the main program, at position 0
* methods, laid out afterward

------
Frames
------

Frames contain an array of registers. The size of the array is
completely dependent upon the number of registers necessary for
the code that is about to be executed.

The first n registers for a function call are always the arguments to the
function itself. Any additional registers are used within the method.

thus, method calls are much more expensive than just inlining the
code. Ouch. That should be improved on.
