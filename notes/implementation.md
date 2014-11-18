# Compiler Phases

The following phases currently exist in grehawk:

* Lexer: tokenizes input
* Parser: converts tokens into AST/VM Objects
* VM: can execute VM objects

We should probably do something like:

* Lexer: tokenizes input
* Style Checker: enforces styling rules (e.g. only one space between operators)
* Parser: converts tokens into AST
* CodeGen: reads AST, creates VM Objects
  * typechecking is performed here
  * scope errors, etc, are checked here
* VM: reads AST and creates a vm objects which can execute code
  * vm contains the ability to JIT things as necessary

* How the global scope is loaded
  * when a module is imported:
    * import() first checks if the module exists in the globals area
      * if it does, import it in
      * if not load_file is called on it which:
         * executes the file, which returns a rootScope object
         * the rootScopeObject is added into the globals area
      * however, this doesn't make it easily loadable.
      * you have to make it easy to load and compile the value.
  *

Examples That Don't Work:

* class.gh
* sandbox.gh
* stdin.gh
* yaml.gh
