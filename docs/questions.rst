=========
Questions
=========

This page talks about some of the big questions that need to be answered in program language design.

How does one deal with really large classes? e.g. ones with a large list of methods
===================================================================================

It seems like classes with lots of methods is an anti-pattern: the
class can probably be factored out into smaller classes.

That said, some languages provide methods to fix this:

* go allows adding methods on a class anywhere
* c++ requires the header to contain all method declarations , but allows logic to be split out into separate files


Implementing observers
======================

It would be interesting to get the observer pattern in here.

    Foo.onChange(String name, ptr value):

    Foo.property.onChange.append(method)
