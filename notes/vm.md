# VM Implementation Notes

## The VM

the VM class actually contains the context of the current
execution.

* it has the ability to execute functions
* it contains the context of the modules (environmentInstances) to import


## Environment

The environment is named as such to remove any ambiguity in it's implementation: it
is not based on Stack Frames, nor is it a representation of scope.

The environment is basically an archetype for an actual set of registers
that code can interact with. It contains information about:

* it's parent
* the globals it loaded
    * we keep track of these to minimize the number of pointers we need to instantiate,
      and keep the stack small.
* the locals it has

## The EnvironmentInstance

The EnivornmentInstance is basically a struct with the following:

* a pointer to the current environment (if lookup by name is ever necessary)
* an array of pointers to values in the parent environments
* an array of values for the locals ('registers')

The vm mainly operates on these values.

## The Function

* functions have:
    * a environment object, from which to generate a environmentInstance to work with
    * a set of instructions to execute against the following locations of data:
        * locals (environmentInstance.locals)
        * globals (environmentInstance.globals)
        * modules (vm->modules)
