=====
Lists
=====

This page describes how Greyhawk deals with lists.

Lists are an interesting type, because clean methods to manipulate
lists can really determine how easy or hard it is to work with a
language in general. A lot of programming deals with iterating,
filtering, and operating on lists.

=====================
Functional Operations
=====================

Functional languages offer a common set of operations to operate on
lists and derive results:

* map -> apply an operation on a list and return the results
* filter -> return the elements that satisfy a specific requirement
* foldleft -> use an accumulator and return the final element

Python has an interesting philosophy where a list comprehension-like syntax is clearer over a keyword::

    # map
    (f(x) for x in list)
    # filter
    (x for x in list if condition(x))
    # foldleft
    # python doesn't have one for that...


It is clearer, but it makes it harder to easily add new functional operations.

Scala's is nice::

    // map
    list.map(f)
    // filter
    list.filter(condition)
    // foldleft
    list.foldleft(accumulator_initial)(folder)

=================================
Lists vs Generators
=================================

There's a distinction between a standard list and a generator:

* a list is an ordered set of values, which allows operations like:
    * specific element access
    * sublist access
* a generator yields several values until no values exist. Advantages are:
    * distributing tasks to several threads (each one just calls next() to get the next worker element)
    * processes next element on-the-fly: helpful for a large number of elements

Python has a method to distinguish list comprehensions with generators by changing the bracketing: ( or [::

    (f(x) for x in list)  # this is a generator
    [f(x) for x in list]  # this is a list expression


By default however, it seems like the expected behaviour would be
better creating lists instead of generators, which have distinctly
different properties. Thus, there should be explicit generator
functions to create a generator instead. Or better yet, use a 'yield' like in Scala and Python::

    var a = for (i <- list) yield i * 2  // scala

    # python
    def iterator():
      for i in list:
        yield i * 2
