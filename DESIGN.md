# Design Decisions for Greyhawk

## Why a VM?

In the future, Greyhawk will provide some metaprogramming
functionality. Having a VM makes implementing metaprogramming easier.

## Immutability by Default

When programming, debugging is a large portion of the work required to
finish an application. Mutable state makes debugging exponentially
more difficult: the number of possible modifications that could be
made to an object is effectively infinite, and it's difficult to find
side effects.

In most cases, mutable state is not necessary, nor desired. Having
immutability as the default ensures faster debugging and easier
reasoning about the logic of an application.

## Rigid Syntax

Programming with text comes with a certain level of aesthetic design:
where to put curly brackets, how to indent. Many arguments and
developer hours are focused on which pattern is best, and those rules
are difficult to enforce automatically. Instead, the arguing comes
from the language designers, reducing the amount time used on this
arguably useless topic.
