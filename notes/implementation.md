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


Examples That Don't Work:

* arrays.gh
* class.gh
* foreach.gh
* for.gh
* sandbox.gh
* stdin.gh
* yaml.gh
