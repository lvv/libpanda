/******************************************************************************
  Panda

  Change Control:                                                      DDMMYYYY
    Michael Still    File created                                      03062000
                     ... Code inserted as needed to test new functionality ...

  Purpose:
    The Panda codebase can be used in a variety of ways, one of them is as
    as stand alone application. This file supports that methodology.
******************************************************************************/

#include  "functions.h"
#include  "constants.h"

int main(int argc, char *argv[]){
  pdf    *demo;
  page   *currPage;
  int    lineDepth;

  // Initialise the library
  initpanda();

  // Open our demo PDF
  if((demo = pdfopen("output.pdf", "w")) == NULL)
    error("demo: could not open output.pdf to write to.");

  // For every text mode in v 1.3
  for(lineDepth = 0; lineDepth < 8; lineDepth++){
#if defined DEBUG
    printf("Created page\n");
#endif

    // Create a page
    currPage = pdfpage(demo, gPageSizeA4);

    // Put some text onto it
    setfontmode(demo, lineDepth);
    textbox(demo, currPage, lineDepth * 20 + 10, 10, 100, 30, "Hello World!");

#if defined DEBUG
    printf("Created textbox\n");
#endif
  }

  pdfclose(demo);
}
