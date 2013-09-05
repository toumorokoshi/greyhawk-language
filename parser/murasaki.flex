%{

#include <stdio.h>
#include "murasaki.tab.h"

void yyerror(const char *str)
{
        fprintf(stderr,"error: %s\n",str);
}
 

%}

%% 
[123456789] { return NONZERO_DIGIT; }
. { yyerror("Invalid Character!"); }
%%
