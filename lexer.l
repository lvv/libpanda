%{
#include "lexer.tab.h"

extern int binaryMode;

void debuglex(char *, char *, int);
char *returnStr(char *);

%}

%%

\%PDF-[0-9]+\.[0-9]+    { debuglex(yytext, "version", binaryMode);
                          yylval.textVal = (char *) returnStr(yytext);
                          return((binaryMode == 0) ? VERSION : ANYTHING);    }

[ \t\r\n]+              { debuglex(yytext, "whitespace", binaryMode);
                          if(binaryMode == 1){
                            yylval.textVal = (char *) returnStr(yytext);
                            return ANYTHING;
                          }                                                  }

\/[a-zA-Z\-0-9\.]+      { debuglex(yytext, "name", binaryMode);
                          yylval.textVal = (char *) returnStr(yytext + 1);
                          return((binaryMode == 0) ? NAME : ANYTHING);       }

                        /* --- stuff required for objects ---               */
[0-9]+\ [0-9]+\ R       { debuglex(yytext, "object reference", binaryMode);
                          yylval.textVal = (char *) returnStr(yytext);
                          return((binaryMode == 0) ? OBJREF : ANYTHING);     }

obj                     { debuglex(yytext, "obj", binaryMode);
                          return((binaryMode == 0) ? OBJ : ANYTHING);        }

endobj                  { debuglex(yytext, "endobj", binaryMode);
                          return((binaryMode == 0) ? ENDOBJ : ANYTHING);     }

stream                  { debuglex(yytext, "stream", binaryMode);
                          return STREAM;                                     }

endstream               { debuglex(yytext, "endstream", binaryMode);
                          return ENDSTREAM;                                  }

[+-]?[0-9]+             { debuglex(yytext, "integer", binaryMode);
                          yylval.intVal = atoi(yytext);
                          return((binaryMode == 0) ? INT : ANYTHING);        }

[+-]?[0-9]+\.[0-9]+     { debuglex(yytext, "floating point", binaryMode);
                          yylval.textVal = (char *) returnStr(yytext);
                          return((binaryMode == 0) ? FP : ANYTHING);        }

\<\<                    { debuglex(yytext, "<<", binaryMode);
                          return DBLLT;                                      }

\>\>                    { debuglex(yytext, ">>", binaryMode);
                          return DBLGT;                                      }

[a-zA-Z0-9\.:'+\-!]+    { debuglex(yytext, "string", binaryMode);
                          yylval.textVal = (char *) returnStr(yytext);
                          return((binaryMode == 0) ? STRING : ANYTHING);     }

\([a-zA-Z0-9\.\ :'+-]+\) {
                          debuglex(yytext, "bracketted string", binaryMode);
                          yylval.textVal = (char *) returnStr(yytext);
                          return((binaryMode == 0) ? STRING : ANYTHING);     }

                        /* --- Array handling ---                           */

\[                      { debuglex(yytext, "[", binaryMode);
                          return((binaryMode == 0) ? ARRAY : ANYTHING);      }

\]                      { debuglex(yytext, "]", binaryMode);
                          return((binaryMode == 0) ? ENDARRAY : ANYTHING);   }

                        /* --- Stuff used to match binary streams ---       */
[^a-zA-Z0-9+-\/\t\ \r\n()\[\]]+     { 
                          debuglex(yytext, "run of wierd characters",
                            binaryMode);
                          yylval.textVal = (char *) returnStr(yytext);
                          return ANYTHING;
                                                                             }

.                       { debuglex(yytext, "the catch all", binaryMode);
                          yylval.textVal = (char *) returnStr(yytext);
                          return ANYTHING;                                   }

                        /* --- Stuff needed for the xref and trailer ---    */

\%\%EOF                 { debuglex(yytext, "end of file", binaryMode);
                          return PDFEOF;                                        }

%%

void debuglex(char *text, char *desc, int binMode){
  #if defined DEBUG
    if(binMode == 0)
      printf("Text %s matched %s with binary mode being %d (false)\n", text, desc,
	     binMode);
    else
      printf("Suppressed debugging output for %s because we are in binary mode\n",
	     desc);
  #endif
}

char *returnStr(char *yytext){
  char *lval;

  #if defined DEBUG
     if(binaryMode == 0) printf("Calling returnStr on %s\n", yytext);
     printf("yytext was %d characters long\n", strlen(yytext));
  #endif

  if((lval = malloc(sizeof(char) * strlen(yytext))) == NULL)
    error("Could not make space for lexer return.");
  strncpy(lval, yytext, strlen(yytext));

  return lval;
}