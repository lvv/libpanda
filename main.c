/******************************************************************************
  Panda

  Change Control:                                                      DDMMYYYY
    Michael Still    File created                                      03062000
                     ... Code inserted as needed to test new functionality ...

  Purpose:
    The Panda codebase can be used in a variety of ways, one of them is as
    as stand alone application. This file supports that methodology.
******************************************************************************/

#include  <panda/functions.h>
#include  <panda/constants.h>

int
main (int argc, char *argv[])
{
  panda_pdf *demo;
  panda_page *currPage;
  int lineDepth;
  char tempString[1024], *tempPtr;

  printf ("Welcome to the Panda 0.4 sample application...\n");

  // Initialise the library
  panda_init ();

  // Open our demo PDF
  if ((demo = panda_open ("output.pdf", "w")) == NULL)
    panda_error ("demo: could not open output.pdf to write to.");

  // For every text mode in v 1.3
  for (lineDepth = 0; lineDepth < 8; lineDepth++)
    {
#if defined DEBUG
      printf ("Created page\n");
#endif

      // Create a page
      currPage = panda_newpage (demo, panda_pagesize_a4);

      // Put in the background images
      panda_imagebox (demo, currPage, 0, 0, currPage->height / 2,
		      currPage->width, "input.tif", panda_image_tiff);
      panda_imagebox (demo, currPage, currPage->height / 2, 0,
		      currPage->height, currPage->width, "input2.tif",
		      panda_image_tiff);

      // Put some text onto it
      panda_setfontmode (demo, lineDepth);
      panda_setcharacterspacing (demo, (double) lineDepth);
      panda_setwordspacing (demo, (double) lineDepth * 10);
      panda_sethorizontalscaling (demo, (double) 1 - (lineDepth * 0.1));
      panda_setleading (demo, 16.0);

      // I am not drawing a multiline string here because I am not sure how to 
      // represent this in the PDF at the moment
      sprintf (tempString,
	       "Hello %c5World! %cMy name %c5is Panda!\nAnd I am a PDF generator\nI handle multiple line text ok .once you have set a leading.",
	       4, 6, 5);
      panda_textbox (demo, currPage, lineDepth * 20 + 10, 10 + lineDepth, 100,
		     30, tempString);

      panda_setfont (demo, tempPtr = panda_createfont (demo, "Symbol", 1,
						       "MacRomanEncoding"));
      panda_textbox (demo, currPage, lineDepth * 20 + 50, 10 + lineDepth, 100,
		     30, "Symbol");
      free (tempPtr);

      panda_setfont (demo, tempPtr =
		     panda_createfont (demo, "Helvetica-Bold", 1,
				       "MacRomanEncoding"));
      panda_textbox (demo, currPage, lineDepth * 20 + 70, 30 + lineDepth, 100,
		     30, "A line in Helvetica-Bold");
      free (tempPtr);

      // Insert some images
      panda_imagebox (demo, currPage, 100, 100, 150, 150, "gnu-head.jpg",
		      panda_image_jpeg);
      panda_textbox (demo, currPage, 90, 110, 200, 200,
		     "INFRONTINFRONTINFRONT");

      panda_textbox (demo, currPage, 190, 210, 300, 300,
		     "BEHINDBEHINDBEHIND");
      panda_imagebox (demo, currPage, 200, 200, 317, 317, "gnu_box.jpg",
		      panda_image_jpeg);
      panda_imagebox (demo, currPage, 317, 317, 434, 434, "gnu_box.jpg",
		      panda_image_jpeg);
      panda_imagebox (demo, currPage, 434, 434, 551, 551, "gnu_box.jpg",
		      panda_image_jpeg);

      // Do an panda_imageboxrot or two to test the code included by Ceasar Miquel
      panda_imageboxrot (demo, currPage, 600, 0, 717, 117, 15.0,
			 "gnu_box.jpg", panda_image_jpeg);

      panda_imageboxrot (demo, currPage, 600, 200, 717, 317, 30.0,
			 "gnu_box.jpg", panda_image_jpeg);

      panda_imageboxrot (demo, currPage, 600, 400, 717, 517, 42.0,
			 "gnu_box.jpg", panda_image_jpeg);

      panda_imageboxrot (demo, currPage, 700, 0, 817, 117, 90.0,
			 "gnu_box.jpg", panda_image_jpeg);

      panda_imageboxrot (demo, currPage, 700, 200, 817, 317, 132.0,
			 "gnu_box.jpg", panda_image_jpeg);

#if defined DEBUG
      printf ("Created textbox and inserted first image\n");
#endif

      panda_textbox (demo, currPage, 300, 10, 400, 50,
		     "A second textbox on the page");

#if defined DEBUG
      printf ("Created second textbox\n");
#endif

      panda_setfont (demo, tempPtr = panda_createfont (demo, "Times-Roman", 1,
						       "MacRomanEncoding"));
      panda_textbox (demo, currPage, 600, 10, 700, 300,
		     "The background image on this page is Copyright 2000 Andrew Cagney");
      panda_textbox (demo, currPage, 620, 10, 720, 300,
		     "and is distributed under the terms of the GPL...");
      free (tempPtr);

      // We also put in some lines and curves so that we can test that
      panda_setlinestart (currPage, 100, 100);
      panda_addlinesegment (currPage, 150, 120);
      panda_addlinesegment (currPage, 200, 176);
      panda_addlinesegment (currPage, 86, 12);
      panda_endline (currPage);
    }

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
