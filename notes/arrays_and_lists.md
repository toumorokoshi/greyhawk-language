# Arrays and Lists

I like both arrays and lists. I think both have their place:

* arrays work well when the size is fixed
* lists work well when the size isn't

I think it makes sense to have a list type kind of be the default
type. Most of the type, you want some sort of unbounded object that
you can just add/remove as you please.

However, a raw array type is also necessary. This is to enabled
developers to raw their own performant objects, as necessary.

We could allow both to have the same syntax too:

    a := [1, 2, 3] // would this be an array, or a list?
    as_array := Array<Int>{1, 2, 3}
    as_list := List<Any>{1, 2, 3}
    as_map := Map{k:
    buffer := List<Int>()
