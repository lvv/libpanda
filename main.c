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
  char   tempString[1024];

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
    setcharacterspacing(demo, (double) lineDepth);
    setwordspacing(demo, (double) lineDepth * 10);
    sethorizontalscaling(demo, (double) 1 - (lineDepth * 0.1));
    setleading(demo, 16.0);

    // I am not drawing a multiline string here because I am not sure how to 
    // represent this in the PDF at the moment
    sprintf(tempString, "Hello %c5World! %cMy name %c5is Panda!\nAnd I am a PDF generator\nI handle multiple line text ok (once you have set a leading)", 4, 6, 5);
    textbox(demo, currPage, lineDepth * 20 + 10, 10 + lineDepth, 100, 30, 
      tempString);

#if defined DEBUG
    printf("Created textbox\n");
#endif

    textbox(demo, currPage, 300, 10, 400, 50, "A second textbox on the page");

#if defined DEBUG
    printf("Created second textbox\n");
#endif
  }

  pdfclose(demo);

  // We should return a value here
  return 0;
}
