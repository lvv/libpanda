typedef union {
  int      intVal;
  char     *textVal;
} YYSTYPE;
#define	VERSION	257
#define	NAME	258
#define	STRING	259
#define	OBJREF	260
#define	OBJ	261
#define	ENDOBJ	262
#define	INT	263
#define	FP	264
#define	DBLLT	265
#define	DBLGT	266
#define	STREAM	267
#define	ENDSTREAM	268
#define	ARRAY	269
#define	ENDARRAY	270
#define	PDFEOF	271
#define	ANYTHING	272


extern YYSTYPE yylval;
