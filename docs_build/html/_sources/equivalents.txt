Equivalents
===========

This page outlines equivalents of concepts in one language in Greyhawk.

Preprocessor directives and constants
-------------------------------------

C and C++ utilize preprocessor directives to make code more
efficient. By providing definitions that are injected into the code
before they are compile, it allows for one less variable that needs to
be referenced in a location in memory, instead of on runtime.

Greyhawk handles this by offering a 'production' build target that
optimizes constants as if they were preprocessor directives. This also
has the side effect of making it easier to see where a define was set::

    from myapp import conf

    def run_this_on_debug():
        /* in a production target build, with conf.DEBUG set to false, 
           this whole method would get optimized away */
        if conf.DEBUG:
            stdout.print("We're in debug mode!")

Since null methods are optimized as well in a production build, you
could easily create remove all debug logging message calls by wrapping
it in a method which only runs anything when the debug variable is
set::

    class ExtendedLog extends Log:

        def DebugLog(String message):
            if conf.DEBUG:
                Debug(message)
