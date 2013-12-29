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

Transactional Blocks
====================

typically, exception handling is used for cleanup or reporting, and
has nothing to do with actually modifying the behaviour of the
exception or catching it in some way. Thus, current try/except patterns can get really ugly.


    try:
        has_exception_a()
        has_exception_b()
    except AException:
        cleanup_a()
        reraise AException
    except BException:
        cleanup_b()
        reraise BException

There needs to be some method to handle this transaction-based
programming without altering the exception raised. D handles this with
the scope() keywords `scope <http://dlang.org/statement.html#ScopeStatement>`

We need this too, but I'm hoping to make it clearer that a particular cleanup is affiliated with a call:

    lock_resource() {
        exit:
            release_resource()
        failure:
            log.info("failed!")
        success:
            log.info("success!")
    }

Ternary
=======

Ternaries, as with any other code, ideally exhibits the following qualities:

* clear
* concise (i.e. minimal syntax)

Unfortunately, ternarys tend to be one or the other:

    condition ? true_expression : false_expression

is concise, but not clear

    true_expression if condition else false_expression

is kind of clear, but far from concise.

My current solutions is:

    if condition then true_expressions else false_expression

    a := if is_32_bit() then 32 else 64

Utilizing Expressions in Variable assignment
============================================

It's common to allow variable assignment in expressions:

    a := 32 if conditional_expressions() else 16
    
By making more statements into expressions, one can have more concise code:

(a la rust):

    let price = 
        if item == "salad" {
            2.50
        } else if item == "muffin" {
            2.25 
        } else {
            2
        }

Casting
=======

Two potential ways to cast:

    ((foo) x).method()
    (x as foo).method()

Syntax Extensions
=================

I'm not sure about this one, but rust has an interesting idea of extending the base syntax by adding top-level methods:

    go!(thread_body)

syntax extensions are denoted by a '!', to show that they are not part
of the standard keywords. This may be a nice way of implementing
bolt-commands, and a greate way of distinguishing between a built-in

Switch/Case
===========

Switch/Case statements should have no falling through (only evaluate the matching statement)

simple concurrency constructs
=============================

Io has an interesting construct that makes build concurrent
applications very simple. Since each invocation is actually a message,
it allows the easy conversion of a synchronous process with an
synchronous process:

    // synchronous
    Object name

    // asynchronous with future value returned (placeholder which gets replaced by the real thing)
    Object @name

    // asynchronous with no return value
    Object @@name
