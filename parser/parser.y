%{
  #include <cstdio>
  #include "node.h"
  NBlock *programBlock; // top level block

  extern int yylex();
  void yyerror(const char *s) { std::printf("Error: %s\n", s); }
%}

// different node type
%union {
  Node *node;
  NBlock *block;
  NExpression *expr;
  NStatement *stmt;
  NIdentifier *ident;
  NVariableDeclaration *var_decl;
  NVariableDeclaration *var_assignment_decl;
  std::vector<NVariableDeclaration*> *varvec;
  std::vector<NExpression*> *exprvec;
  std::string *string;
  int token;
}

/* terminal tokens */

// identifiers/constants
%token <string> TIDENTIFIER TINTEGER TDOUBLE TRETURN
// comparison tokens 
%token <token> TCEQ TCNE TCLT TCLE TCGT TCGE TEQUAL
// method/function related tokens
%token <token> TLPAREN TRPAREN TCOMMA TDOT TCOLON TINDENT TUNINDENT
 // assignment-related tokens 
%token <token> TDECLARE
// operator-related tokens
%token <token> TPLUS TMINUS TMUL TDIV

// non-terminal tokens
%type <ident> ident
%type <expr> numeric expr
%type <varvec> func_decl_args
%type <exprvec> call_args
%type <block> program stmts block
%type <stmt> stmt var_decl var_assignment_decl func_decl
%type <token> comparison

 // operator precedence
%left TPLUS TMINUS
%left TMUL TDIV

%start program 

%%

program : stmts { programBlock = $1; }
;

stmts : stmt { $$ = new NBlock(); $$->statements.push_back($<stmt>1); }
      | stmts stmt { $1->statements.push_back($<stmt>2); }
;

stmt : 
  var_decl
| func_decl
| expr
;

block : 
  TINDENT stmts TUNINDENT { $$ = $2; } |
| TINDENT stmts { $$ = $2; }
;

var_decl : ident ident TDECLARE expr { $$ = new NVariableDeclaration(*$1, *$2, $4); }
;

// this contains assigment operators:
// Foo foo
// Foo foo = Foo()
var_assignment_decl : 
  ident ident { $$ = new NVariableDeclaration(*$1, *$2); }
| ident ident TEQUAL expr { $$ = new NVariableDeclaration(*$1, *$2, $4); }
;

func_decl : ident ident TLPAREN func_decl_args TRPAREN TCOLON block 
            { $$ = new NFunctionDeclaration(*$1, *$2, *$4, *$7); delete $4; }
;

func_decl_args : 
  { $$ = new VariableList(); } // no args
| var_assignment_decl { $$ = new VariableList(); $$->push_back($<var_decl>1); }
| func_decl_args TCOMMA var_assignment_decl { $1->push_back($<var_decl>3); }
;

ident : TIDENTIFIER { $$ = new NIdentifier(*$1); delete $1; }
;

numeric : 
  TINTEGER { $$ = new NInteger(atol($1->c_str())); delete $1; }
| TDOUBLE { $$ = new NDouble(atof($1->c_str())); delete $1; } 
;

expr : 
  ident TEQUAL expr { $$ = new NAssignment(*$<ident>1, *$3); }
| ident TLPAREN call_args TRPAREN { $$ = new NMethodCall(*$1, *$3); delete $3; }
| ident { $<ident>$ = $1; }
| numeric
| expr comparison expr { $$ = new NBinaryOperator(*$1, $2, *$3); }
| TLPAREN expr TRPAREN { $$ = $2; }
| TRETURN expr { $$ = $2; }
;

call_args :
  { $$ = new ExpressionList(); }
| expr { $$ = new ExpressionList(); $$->push_back($1); }
| call_args TCOMMA expr { $1->push_back($3); }
;

comparison : 
  TCEQ 
| TCNE 
| TCLT 
| TCLE 
| TCGT 
| TCGE 
| TPLUS 
| TMINUS 
| TMUL 
| TDIV
;

%%
