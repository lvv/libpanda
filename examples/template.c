/******************************************************************************
  Panda

  Change Control:                                                      DDMMYYYY
    Michael Still    File created                                      16062001

  Purpose:
    Test templating functionality
******************************************************************************/

#include  <panda/functions.h>
#include  <panda/constants.h>

int
main (int argc, char *argv[])
{
  panda_pdf *demo;
  panda_page *currPage, *templatePage;
  int lineDepth;
  char tempString[1024], *tempPtr;

  // Initialise the library
  panda_init ();

  // Open our demo PDF
  if ((demo = panda_open ("template.pdf", "w")) == NULL)
    panda_error (panda_true, "demo: could not open output.pdf to write to.");

  ///////////////////////////////////////////////////////////////////////////
  // Test template functionality
  ///////////////////////////////////////////////////////////////////////////

  templatePage = panda_newtemplate (demo, panda_pagesize_a4);
  panda_setfillcolor (templatePage, 112, 138, 37);
  panda_setlinewidth (templatePage, 5);
  panda_setlinestart (templatePage, 0, 0);
  panda_addlinesegment (templatePage, 594, 841);
  panda_addlinesegment (templatePage, 0, 841);
  panda_closeline (templatePage);
  panda_fillline (templatePage);
  panda_endline (templatePage);

  currPage = panda_newpage (demo, panda_pagesize_a4);
  panda_applytemplate (demo, currPage, templatePage);

  // Finished all the demoing, close the PDF document
  panda_close (demo);

  // We should return a value here
  return 0;
}
