Equivalents
===========

This page outlines equivalents of concepts in one language in Murasaki.

Preprocessor directives and constants
-------------------------------------

C and C++ utilize preprocessor directives to make code more
efficient. By providing definitions that are injected into the code
before they are compile, it allows for one less variable that needs to
be referenced in a location in memory, instead of on runtime.

murasaki handles this by offering a 'production' build target that
optimizes constants as if they were preprocessor directives. This also
has the side effect of making it easier to see where a define was set::

    from myapp import conf

    def run_this_on_debug():
        /* in a production target build, with conf.DEBUG set to false, 
           this whole method would get optimized away */
        if conf.DEBUG:
            stdout.print("We're in debug mode!")
