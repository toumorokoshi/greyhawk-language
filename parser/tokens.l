%{
#include <string.h>
#include "node.h"
#include "parser.hpp"
#define SAVE_TOKEN yylval.string = new std::string(yytext, yyleng)
#define TOKEN(t) (yylval.token = t)
extern "C" int yywrap() { }
 
 int current_line_indent = 0; /* indentation of current line */
 int indent_level = 0; /* indentation level passed to the parser */

%}

  /* start state for parsing indentation */
%x indent

%%
  /* 
   logic to handle indentation
   based off of code here: 
   http://stackoverflow.com/questions/1413204/how-to-use-indentation-as-block-delimiters-with-bison-and-flex 
  */
<indent>" "            { printf("A line can not start with spaces! only tabs denote indentation.\n"); yyterminate(); }

<indent>[\t]           { current_line_indent++; }
<indent>.              {
  unput(*yytext);
   if (current_line_indent > indent_level) {
     indent_level++;
     return TOKEN(TINDENT);
   } else if (current_line_indent < indent_level) {
     indent_level--;
     return TOKEN(TUNINDENT);
   }
   BEGIN(INITIAL);
 }

  /* regular logic */
<INITIAL>"\n"           { current_line_indent = 0; BEGIN(indent); }
[[:alpha:]_][[:alnum:]_]*    SAVE_TOKEN; return TIDENTIFIER;
[[:digit:]]+\.[[:digit:]]*    SAVE_TOKEN; return TDOUBLE;
[[:digit:]]+                  SAVE_TOKEN; return TINTEGER;
"="                          return TOKEN(TEQUAL);
"=="                   return TOKEN(TCEQ);
"!="                   return TOKEN(TCNE);
"<"                    return TOKEN(TCLT);
"<="                   return TOKEN(TCLE);
">"                    return TOKEN(TCGT);
">="                   return TOKEN(TCGE);
"("                    return TOKEN(TLPAREN);
")"                    return TOKEN(TRPAREN);
":"                    return TOKEN(TCOLON);
":="                   return TOKEN(TDECLARE);
"."                    return TOKEN(TDOT);
","                    return TOKEN(TCOMMA);
"+"                    return TOKEN(TPLUS);
"-"                    return TOKEN(TMINUS);
"*"                    return TOKEN(TMUL);
"/"                    return TOKEN(TDIV);
" "                    ; /* spaces are null operators */
.                      printf("Unknown token %s! \n", yytext); yyterminate();

%%
