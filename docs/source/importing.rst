=================
Importing Modules
=================

Importing modules in Murasaki work as follows::

  import a.b
  import a.b as c
  from d import e

  a.b.Foo()
  e.Foo()

Package/Namespace names are always in all lowercase. No underscores, dashes, or special characters are allowed::

    json
    configparser
