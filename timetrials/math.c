/*****************************************************************************
  Panda suppport code. I need a simple application that can add numbers which
  have decimal points. This application takes a string on the command line as
  as series of arguements that defines the mathematical operation to perform.

  e.g.
    0.001 + 4.2
*****************************************************************************/

#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]){
  double   a, b;

  if(argc < 3){
    printf("Usage: %s <num one> + <num two>\n", argv[0]);
    exit(42);
  }

  a = atof(argv[1]);
  b = atof(argv[3]);

  switch(argv[2][0]){
  case '+':
    printf("%f\n", a + b);
    break;

  case '/':
    printf("%f\n", a / b);
    break;

  default:
    printf("Undefined operation\n");
    exit(42);
    break;
  }

  exit(0);
}
