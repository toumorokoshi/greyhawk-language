Language Objectives
===================

Greyhawk is driven by a set of objectives that determine the choices made for the language.

Readibility is first priority
-----------------------------

Terse, hard to read code is a symptom of 'black-box' programs and
libraries, or components that can only be modified by a select few due
to their complex nature or the methodology by which it was written.

It is a goal of Greyhawk that anyone with a decent amount of
experience with the language can understand what another person's code
is doing.

One idiomatic way of doing things
---------------------------------

With readability in mind comes standardization. Humans are good at
recognizing patterns, and Greyhawk relies on that strength by ensuring
that there is only one idiomatic way of performing a particular
function, and making the idiomatic way simple.

Everything follows convention
-----------------------------

Special keywords and syntax can cause confusion and ambiguity to
someone not familiar with these deviations (Such as python2's 'print'
and 'assert' being effectively methods that do not need arguments
passed in parentheses). Greyhawk attempts to minimize that by ensuring
that all features of the code follow a standard.

'Style' does not exist
----------------------

Most languages allow a lot of freedom when it comes to coding style, choices like:

* spaces vs tabs in python
* free spacing and indentation in most bracket-based languages
* underscores or casing in variable naming

Cause large differences between code look and feel. Greyhawk attempts
to ensure that any 'style' deviations from standard Greyhawk code result in:

1. A Syntax error
2. A change in behaviour in the code.

Everything is Permitted
-----------------------

Some languages have said they restrict behaviour because programmers
will use it improperly. Greyhawk should not share that
sentiment. Everything is permitted. There may be one idiomatic way of
doing it, but restricting behaviour based off of an alleged clumsiness
restricts a language in unnescessary ways.
