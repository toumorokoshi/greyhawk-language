=======
Lambdas
=======

Lambdas are a necessity for clean, concise functional
programming. having short, abbreviated syntax is a great way to express concepts quickly.

On the other hand, it makes testing those methods very difficult.

However, everything is permitted. The only alternative to lambdas is
on-the-fly method definitions, which is expensive from an authoring
point of view. So we should allow lambdas.

I like Scala's lambdas::

  (x) => x * 2
  (args) => result

Types seem like they can be infered and statically checked.

Alternatively, python's lambdas::

  lambda args: result

Not as clean as Scala, imo.
