/******************************************************************************
  Panda

  Change Control:                                                      DDMMYYYY
    Michael Still    File created                                      03062000
                     ... Code inserted as needed to test new functionality ...

  Purpose:
    Empty documents used to crash Panda. This verifies that this is no longer
    a problem.
******************************************************************************/

#include  <panda/functions.h>
#include  <panda/constants.h>

int
main (int argc, char *argv[])
{
  panda_pdf *demo;
  panda_page *currPage, *templatePage;

  panda_init ();

  // Open our demo PDF
  if ((demo = panda_open ("empty.pdf", "w")) == NULL)
    panda_error (panda_true, "demo: could not open output.pdf to write to.");

  // Create a page
  currPage = panda_newpage (demo, panda_pagesize_a4);

  panda_close (demo);

  // We should return a value here
  return 0;
}

// Allow a callback to be setup to display a dialog box for an error or
// whatever before we terminate the application
void
panda_errorCallback (char *description)
{
  fprintf (stderr, "Callback: %s\n", description);
}
