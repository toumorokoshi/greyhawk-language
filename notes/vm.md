# VM Implementation Notes

## The VM

the VM class actually contains the context of the current
execution.

* it has the ability to execute functions
* it contains the context of the modules (scopeInstances) to import

## The Function

* functions have:
    * a scopeInstance (parent scope), to retrieve values from.
    * a set of instructions to execute against:
        * a set of registers
        * globals
        * modules


## ScopeInstance

* a scopeinstance contains
    * a scope, containing global values.
    * a set of global values
    * a set of local values
