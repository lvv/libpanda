/******************************************************************************
  Panda

  Change Control:                                                      DDMMYYYY
    Michael Still    File created                                      03062000
                     ... Code inserted as needed to test new functionality ...

  Purpose:
    The Panda codebase can be used in a variety of ways, one of them is as
    as stand alone application. This file supports that methodology.

    This code tests the creation of many sequential pages in a pdf document.
    This will test the object handling within Panda quite extensively.
******************************************************************************/

#include  <panda/functions.h>
#include  <panda/constants.h>
#include  <sys/timeb.h>

int main(int argc, char *argv[]){
  pdf          *demo;
  page         *currPage;
  char         tempString[1024];
  int          count;

  // Initialise the library
  initpanda();

  // Open our demo PDF
  if((demo = pdfopen("output.pdf", "w")) == NULL)
    error("demo: could not open output.pdf to write to.");

  for(count = 0; count < 10000; count++){
    // Create a page
    currPage = pdfpage(demo, gPageSizeA4);

    textbox(demo, currPage, 10, 10, 20, 20, "x");
  }

  pdfclose(demo);

  exit(0);
}
