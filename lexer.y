%{
  //  #include "constants.h"
  //  #include "functions.h"

  int    binaryMode;
  //  pdf    *yypdf;
  //  object *yycurobj = NULL;
%}

          /* Define the possible yylval values */
%union {
  int      intVal;
  char     *textVal;
}

%token <textVal> VERSION
%token <textVal> NAME <textVal> STRING
%token <textVal> OBJREF <textVal> OBJ <textVal> ENDOBJ <intVal> INT 
%token <textVal> FP <textVal> DBLLT <textVal> DBLGT 
%token <textVal> STREAM <textVal> ENDSTREAM
%token <textVal> ARRAY <textVal> ENDARRAY
%token <textVal> PDFEOF
%token <textVal> ANYTHING

%%

          /*********************************************************
            At the moment we accept a full document, or just objects
            this needs to be changed sometime
          *********************************************************/
pdf       : header objects xref trailer
          ;

header    : VERSION binary {}
          ;

objects   : object objects
          |
          ;

object    : INT INT OBJ {
                      printf("Found an object\n");

		      // Create the object in the new PDF we are building
		      //     yycurobj = newobject(yypdf, gNormal);
                                                                             }
                        DBLLT dict DBLGT stream ENDOBJ
          ;

dict      : NAME STRING dict {}
          | NAME NAME dict {}                /* e.g. /Type /Page */
          | NAME ARRAY arrayvals ENDARRAY dict {}
          | NAME OBJREF dict {}
          | NAME DBLLT dict DBLGT dict {}
          | NAME INT dict {}
          |
          ;

arrayvals : OBJREF arrayvals {}
          | INT arrayvals {}
          |
          ;

stream    : STREAM { binaryMode = 1; } binary { binaryMode = 0; } ENDSTREAM 
          |
          ;

binary    : ANYTHING binary {}
          |
          ;

xref      : STRING INT INT xrefitems {}
          ;

xrefitems : INT INT STRING xrefitems {}
          |
          ;

trailer   : STRING DBLLT dict DBLGT STRING INT PDFEOF {}
;

%%

main(){
  // We are not looking into a stream at the moment
  binaryMode = 0;

  // Startup the pdf library and stuff
  //  initpanda();
  //  if((yypdf = pdfopen("lexical.pdf", "r")) == NULL)
  //    error("lexer: could not open lexical.pdf");

  yyparse();
}

yyerror(char *s){
  fprintf(stderr, "Error is %s\n", s);
  exit(42);
}
