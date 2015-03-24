# VM Implementation Notes

## The VM

the VM class actually contains the context of the current
execution.

* it has the ability to execute functions
* it contains the context of the modules (scopeInstances) to import


## The Scope

The scope is basically an archetype for an actual set of registers
that code can interact with. It contains information about:

* it's parent
* the globals it loaded
    * we keep track of these to minimize the number of pointers we need to instantiate,
      and keep the stack small.
* the locals it has

## The ScopeInstance object

The ScopeInstance is basically a struct with the following:

* a pointer to the current scope (if lookup by name is ever necessary)
* an array of pointers to values in the parent scopes
* an array of values for the locals ('registers')

The vm mainly operates on these values.

## The Function

* functions have:
    * a scope object, from which to generate a scopeInstance to work with
    * a set of instructions to execute against the following locations of data:
        * locals (scopeInstance.locals)
        * globals (scopeInstance.globals)
        * modules (vm->modules)
