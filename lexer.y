%{
  #include "constants.h"
  #include "functions.h"

  int    binaryMode;
  pdf    *yypdf;
  object *yycurobj = NULL;
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

%type <textVal> binary

%%

          /*********************************************************
            At the moment we accept a full document, or just objects
            this needs to be changed sometime
          *********************************************************/
pdf       : header objects xref trailer
          ;

header    : VERSION binary {
                      // I'm not sure what the binary is doing for me, 
                      // the version seems to have all of the header 
                      // information...
                      pdfprintf(yypdf, "%s", $1);
		      free($1);
                                                                             }
          ;

objects   : object objects
          |
          ;

object    : INT INT OBJ {
		      // Create the object in the new PDF we are building
		      yycurobj = newobject(yypdf, gNormal);

		      // Make the object actually look like the one we read
		      yycurobj->number = $1;
		      yycurobj->generation = $2;

		      // Make the object a child of the pages object
		      addchild(yypdf->dummyObj, yycurobj);
                                                                             }
                        DBLLT dict DBLGT stream ENDOBJ
          ;

dict      : NAME STRING dict {
                      adddictitem(yycurobj->dict, $1, gLiteralTextValue, $2);
		      free($1);
		      free($2);
                                                                             }
          | NAME NAME dict {
                      ((dictionary *) 
 		        adddictitem(yycurobj->dict, $1, gLiteralTextValue, $2))
                        ->valueType = gTextValue;
		      free($1);
		      free($2);
                                                                             }
          | NAME ARRAY arrayvals ENDARRAY dict {
                                                                             }
          | NAME OBJREF dict {
                      ((dictionary *) 
 		        adddictitem(yycurobj->dict, $1, gLiteralTextValue, $2))
                        ->valueType = gObjValue;
		      free($1);
		      free($2);
                                                                             }
          | NAME DBLLT dict DBLGT dict {
                                                                             }
          | NAME INT dict {
	              adddictitem(yycurobj->dict, $1, gIntValue, $2);
		      free($1);
                                                                             }
          |
          ;

arrayvals : OBJREF arrayvals {}
          | INT arrayvals {}
          |
          ;

stream    : STREAM { binaryMode = 1; } binary { binaryMode = 0; } ENDSTREAM 
          |
          ;

binary    : ANYTHING binary {
                        if($2 != NULL){
			  if(($$ = malloc(sizeof($1) + sizeof($2))) == NULL)
			    error("Could not build the binary stream.");

			  strcat($$, $1);
			  strcat($$, $2);
			  free($1);
			  free($2);
			}
			else $$ = $1;
                                                                             }
          | {
                        $$ = NULL;
                                                                             }
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
  initpanda();
  if((yypdf = pdfopen_suppress("lexical.pdf", "w")) == NULL)
    error("lexer: could not open lexical.pdf");

  yyparse();

  // Write the new pdf out
  pdfclose(yypdf);
}

yyerror(char *s){
  fprintf(stderr, "Error is %s\n", s);
  exit(42);
}
