==================
Eventing Framework
==================

Murasaki should have a core library to handle eventing.

I'm not sure what that looks like yet, but it should do the following:

* a publisher should not have to be aware of it's subscribers::
    event.fire(context)
* the event should be fireable by anyone, or by a single class. that
  should be configurable or declarable
* the passed parameters of the callback should be statically typed, so
  we can take advantange of type safety.
