/******************************************************************************
  Panda

  Change Control:                                                      DDMMYYYY
    Michael Still    File created                                      03062000
                     ... Code inserted as needed to test new functionality ...

  Purpose:
    The Panda codebase can be used in a variety of ways, one of them is as
    as stand alone application. This file supports that methodology.

    Creating many textboxes on the one page has been found to be comparatively
    slow in panda compared with clibpdf and pdflib. We need to determine what
    is causing this and resolve it...
******************************************************************************/

#include  <panda/functions.h>
#include  <panda/constants.h>
#include  <sys/timeb.h>

int main(int argc, char *argv[]){
  pdf          *demo;
  page         *currPage;
  char         tempString[1024];
  int          count, x, y;

  // Initialise the library
  initpanda();

  // Open our demo PDF
  if((demo = pdfopen("output.pdf", "w")) == NULL)
    error("demo: could not open output.pdf to write to.");

  // Create a page
  currPage = pdfpage(demo, gPageSizeA4);

  x = 0;
  y = 16;

  for(count = 0; count < 10000; count++){
    textbox(demo, currPage, y, x, y + 10, x + 10, "x");

    x += 5;
    
    if(x > 594){
      x = 0;
      y += 16;
    }
  }

  pdfclose(demo);
}
