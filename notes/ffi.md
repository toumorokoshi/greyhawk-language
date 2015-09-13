# Greyhawk's foreign function interface.

* it would be nice to just explicitly load the module at the end, as a built in:


    extern Int multiplyByTwo(Int x)

    // this should be relative to this file's declaration.
    loadModule("./multiply.so")
