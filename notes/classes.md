# implementation of classes

classes have the following:

* attributes: a string -> object map
* methods: a string -> method map

This is the 'type' of the class. The class will not allow any more,
nor any less attributes in the class itself.

in the vm, the class will be stored as an array of values.

to find the attribute by name, you must use the class object's map

## How a method is run

With a function, we should add another attribute as the 1st argument
which is the type itself, so

a.foo()

and
foo(a)

are basically identical.

this will require the loading of an attribute from a struct.

CLASS_INSTANTIATE <size> -> <target_register>
CLASS_INSTANTIATE 4 -> {0}
CLASS_LOAD_ATTRIBUTE <struct_register_index> <array_value> <target_index>
CLASS_LOAD_ATTRIBUTE {x}[y] -> {z}
