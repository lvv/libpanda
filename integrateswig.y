%{

%}

          /* Define the possible yylval values */
%union {
  char     *textVal;
}

%token <textVal> COMMENTSTART
%token <textVal> COMMENTEND

%token <textVal> HASH
%token <textVal> LT
%token <textVal> GT
%token <textVal> QUOTE

%token <textVal> WORD

%%

header:  COMMENTSTART words COMMENTEND {} header
      |  includeline header
      |  
      ;

words :  WORD words {}
      |
      ;

includeline
      :  HASH LT WORD GT {}
      |  HASH QUOTE WORD QUOTE {}
      ;

%%

main(){
  yyparse();
}

yyerror(char *s){
  fprintf(stderr, "Error is %s\n", s);
  exit(42);
}
