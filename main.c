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

int
main (int argc, char *argv[])
{
  pdf *demo;
  page *currPage;
  int lineDepth;
  char tempString[1024];

  printf ("Welcome to the Panda 0.2 sample application...\n");

  // Initialise the library
  initpanda ();

  // Open our demo PDF
  if ((demo = pdfopen ("output.pdf", "w")) == NULL)
    error ("demo: could not open output.pdf to write to.");

  // For every text mode in v 1.3
  for (lineDepth = 0; lineDepth < 1; lineDepth++)
    {
#if defined DEBUG
      printf ("Created page\n");
#endif

      // Create a page
      currPage = pdfpage (demo, gPageSizeA4);

      // Put some text onto it
      setfontmode (demo, lineDepth);
      setcharacterspacing (demo, (double) lineDepth);
      setwordspacing (demo, (double) lineDepth * 10);
      sethorizontalscaling (demo, (double) 1 - (lineDepth * 0.1));
      setleading (demo, 16.0);

      // I am not drawing a multiline string here because I am not sure how to 
      // represent this in the PDF at the moment
      sprintf (tempString,
	       "Hello %c5World! %cMy name %c5is Panda!\nAnd I am a PDF generator\nI handle multiple line text ok .once you have set a leading.",
	       4, 6, 5);
      textbox (demo, currPage, lineDepth * 20 + 10, 10 + lineDepth, 100, 30,
	       tempString);

      setfont (demo, createfont(demo, "Helvetica-Bold", 1, "MacRomanEncoding"));
      textbox (demo, currPage, lineDepth * 20 + 30, 30 + lineDepth, 100, 30,
	       "A line in Helvetica-Bold");

      setfont (demo, createfont(demo, "Symbol", 1, "MacRomanEncoding"));
      textbox (demo, currPage, lineDepth * 20 + 50, 10 + lineDepth, 100, 30,
	       "Symbol");

      // Insert some images
      imagebox (demo, currPage, 0, 0, currPage->height / 2, currPage->width,
		"input.tif", gImageTiff);
      imagebox (demo, currPage, currPage->height / 2, 0, currPage->height,
		currPage->width, "input2.tif", gImageTiff);

      imagebox (demo, currPage, 100, 100, 150, 150, "gnu-head.jpg",
		gImageJpeg);
      imagebox (demo, currPage, 200, 200, 317, 317, "gnu_box.jpg",
		gImageJpeg);
      imagebox (demo, currPage, 317, 317, 434, 434, "gnu_box.jpg",
		gImageJpeg);
      imagebox (demo, currPage, 434, 434, 551, 551, "gnu_box.jpg",
		gImageJpeg);

#if defined DEBUG
      printf ("Created textbox and inserted first image\n");
#endif

      textbox (demo, currPage, 300, 10, 400, 50,
	       "A second textbox on the page");

#if defined DEBUG
      printf ("Created second textbox\n");
#endif

      setfont (demo, createfont (demo, "Times-Roman", 1, "MacRomanEncoding"));
      textbox (demo, currPage, 600, 10, 700, 300,
	       "The background image on this page is Copyright 2000 Andrew Cagney");
      textbox (demo, currPage, 620, 10, 720, 300,
	       "and is distributed under the terms of the GPL...");
    }

  pdfclose (demo);

  // ---------------------------- LINEAR -----------------------------------

  // Open our demo PDF
  if ((demo = pdfopen ("output-linear.pdf", "wl")) == NULL)
    error ("demo: could not open output.pdf to write to.");

  // For every text mode in v 1.3
  for (lineDepth = 0; lineDepth < 1; lineDepth++)
    {
#if defined DEBUG
      printf ("Created page\n");
#endif

      // Create a page
      currPage = pdfpage (demo, gPageSizeA4);

      // Put some text onto it
      setfontmode (demo, lineDepth);
      setcharacterspacing (demo, (double) lineDepth);
      setwordspacing (demo, (double) lineDepth * 10);
      sethorizontalscaling (demo, (double) 1 - (lineDepth * 0.1));
      setleading (demo, 16.0);

      // I am not drawing a multiline string here because I am not sure how to 
      // represent this in the PDF at the moment
      sprintf (tempString,
	       "Hello %c5World! %cMy name %c5is Panda!\nAnd I am a PDF generator\nI handle multiple line text ok .once you have set a leading.",
	       4, 6, 5);
      textbox (demo, currPage, lineDepth * 20 + 10, 10 + lineDepth, 100, 30,
	       tempString);

      // Insert some images
      imagebox (demo, currPage, 10, 10, 20, 20, "input.tif", gImageTiff);
      imagebox (demo, currPage, 200, 200, 250, 250, "gnu-head.jpg",
		gImageJpeg);
      //    imagebox(demo, currPage, 300, 300, 310, 310, "gnu_box.jpg", gImageJpeg);

#if defined DEBUG
      printf ("Created textbox and inserted first image\n");
#endif

      textbox (demo, currPage, 300, 10, 400, 50,
	       "A second textbox on the page");

#if defined DEBUG
      printf ("Created second textbox\n");
#endif

      setfont (demo, createfont (demo, "Times-Roman", 1, "MacRomanEncoding"));
      textbox (demo, currPage, 600, 10, 700, 300,
	       "The background image on this page is Copyright 2000 Andrew Cagney");
      textbox (demo, currPage, 620, 10, 720, 300,
	       "and is distributed under the terms of the GPL...");
    }

  pdfclose (demo);

  // We should return a value here
  return 0;
}

// Allow a callback to be setup to display a dialog box for an error or
// whatever before we terminate the application
void
errorCallback (char *description)
{
  fprintf (stderr, "Callback: %s\n", description);
}
