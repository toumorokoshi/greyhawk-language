============
Data As Code
============

Lisp and it's derivatives have a very powerful concept embedded into
them: Data as Code. Data and code should have a merry union in
greyhawk as well.

I think the best way to achieve this is through ensuring that object
initialization specifically mirrors metadata pretty well::

  { FileMenu
    { Tab[]
      { Tab "File" [
        if isOSX() { Command "New" } else None
        { Command "Open" }
        { Command "Save" }
        { Command "Save as..." }
      ]
    }
  }

  def create_filemenu():
      return { FileMenu
        { Tab "Test" }
      }

  { Map
    "test": "foo",
    "too": "food",
    "bor": "baz",
    "roco": "no"
  }
