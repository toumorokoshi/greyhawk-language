%{

#include <stdio.h>
#include "murasaki.tab.h"

%}

%% 
[123456789] { return NONZERO_DIGIT; }
. { return INVALID_CHARACTER; }
%%
