%{
#include "integrateswig.tab.h"

char *returnStr(char *);
%}

%%

\/\*+                    { return COMMENTSTART;                               }

\*+\/                    { return COMMENTEND;                                 }

#                        { return HASH;                                       }

\<                       { return LT;                                         }

\>                       { return GT;                                         }

\"                       { return QUOTE;                                      }

[a-zA-Z0-9.:,]+          { return WORD;                                       }

%%

char *returnStr(char *yytext){
  char *lval;

  if((lval = malloc(sizeof(char) * strlen(yytext) + 1)) == NULL)
    error("Could not make space for lexer return.");
  strncpy(lval, yytext, strlen(yytext) + 1);

  return lval;
}
