=====
Style
=====

Tabs and Spaces
===============

Tabs represent scope in Murasaki: spaces are only valid for separating
tokens, and never define scope. I.E. you can not use spaces
interchangeably with tabs, as each supports a specific purpose.

Naming
======

Naming in Murasaki is of the utmost importance, due to the fact that
naming determines various properties about the value:

Class attributes
----------------

TODO: find a better way to distinguish between variable vs method

* variable -> public variable (lower camel-case results in a public variable)
* _variable -> protected variable (lower camel-case + leading underscore)
* __variable -> private variable (lower camel-case + double leading underscore)
* Method -> public method (upper camel-case results in a public method)
* _Method -> protected method (upper camel-case results + leading underscore)
* __Method -> private method (upper camel-case results + double leading underscore)

Class Naming
------------

Classes types have brackets around them:

    <ConfigFactory>

Constants
---------

Anything with ALL_CAPS and underscores is always a constant::

    int8 ALL_CAPS = 5
