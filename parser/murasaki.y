%{
#include <stdio.h>
#include <string.h>
 
void yyerror(const char *str)
{
        fprintf(stderr,"error: %s\n",str);
}
 
int yywrap()
{
        return 1;
} 
  
main()
{
        yyparse();
} 

%}

%token NONZERO_DIGIT INVALID_CHARACTER

%%

nonzero_digit : NONZERO_DIGIT | 
                NONZERO_DIGIT nonzero_digit 
    { printf("Testing...\n"); } ;
%%
