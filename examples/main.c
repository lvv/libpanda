/******************************************************************************
  Panda

  Change Control:                                                      DDMMYYYY
    Michael Still    File created                                      03062000
                     ... Code inserted as needed to test new functionality ...

  Purpose:
    The Panda codebase can be used in a variety of ways, one of them is as
    as stand alone application. This file supports that methodology.

    Completely rearranged as of 20 Jan 2001 to make the tests clearer...
******************************************************************************/

#include  <panda/functions.h>
#include  <panda/constants.h>

int
main (int argc, char *argv[])
{
  panda_pdf *demo;
  panda_page *currPage, *templatePage;
  int lineDepth, trans;
  char tempString[1024], *tempPtr;

  printf ("Welcome to the Panda 0.5 sample application...\n");

  // Initialise the library
  panda_init ();

  // Open our demo PDF
  if ((demo = panda_open ("output.pdf", "w")) == NULL)
    panda_error (panda_true, "demo: could not open output.pdf to write to.");

  // These are normally commented out because they are annoying
  //panda_hidetoolbar (demo, panda_true);
  //panda_hidemenubar (demo, panda_true);
  //panda_hidewindowui (demo, panda_true);

  if ((argc > 1) && (strcmp (argv[1], "compressed") == 0))
    {
      printf ("Turn on compression\n");

      panda_setproperty (demo->pages, panda_scope_cascade,
			 panda_object_property_compress, panda_true);

      printf ("Just before compression level set\n");

      panda_setproperty (demo->pages, panda_scope_cascade,
			 panda_object_property_compress_level, 9);
    }
  else
    printf ("For compressed sample, use %s compressed\n", argv[0]);

  // Image functionality
  ///////////////////////////////////////////////////////////////////////////

  // Create a page
  currPage = panda_newpage (demo, panda_pagesize_a4);

  // Put in the background images
  panda_imagebox (demo, currPage, 0, 0, currPage->height / 2,
		  currPage->width, "images/input.tif", panda_image_tiff);
  panda_imagebox (demo, currPage, currPage->height / 2, 0,
		  currPage->height, currPage->width, "images/input2.tif",
		  panda_image_tiff);

  panda_imagebox (demo, currPage, 317, 317, 434, 434, "images/gnu_box.jpg",
		  panda_image_jpeg);
  panda_imagebox (demo, currPage, 434, 434, 551, 551, "images/gnu_box.jpg",
		  panda_image_jpeg);

  // Do an panda_imageboxrot or two to test the code included by Ceasar Miquel
  panda_imageboxrot (demo, currPage, 600, 0, 717, 117, 15.0,
		     "images/gnu_box.jpg", panda_image_jpeg);

  panda_imageboxrot (demo, currPage, 600, 200, 717, 317, 30.0,
		     "images/gnu_box.jpg", panda_image_jpeg);

  panda_imageboxrot (demo, currPage, 600, 400, 717, 517, 42.0,
		     "images/gnu_box.jpg", panda_image_jpeg);

  panda_imageboxrot (demo, currPage, 700, 0, 817, 117, 90.0,
		     "images/gnu_box.jpg", panda_image_jpeg);

  panda_imageboxrot (demo, currPage, 700, 200, 817, 317, 132.0,
		     "images/gnu_box.jpg", panda_image_jpeg);

  // Insert a PNG to show that I can
  panda_imageboxrot (demo, currPage, 100, 200, 200, 300, 0.0,
		     "images/libpng.png", panda_image_png);

  panda_imageboxrot (demo, currPage, 300, 200, 400, 300, 0.0,
		     "images/gnu.png", panda_image_png);

  panda_imageboxrot (demo, currPage, 100, 420, 310, 620, 36.0,
		     "images/RedbrushAlpha.png", panda_image_png);

  // (c) statement
  panda_setfont (demo, tempPtr = panda_createfont (demo, "Times-Roman", 1,
						   "MacRomanEncoding"));
  panda_textbox (demo, currPage, 600, 10, 700, 300,
		 "The background image on this page is Copyright 2000 "
		 "Andrew Cagney");
  panda_textbox (demo, currPage, 620, 10, 720, 300,
		 "and is distributed under the terms of the GPL...");
  panda_textbox (demo, currPage, 310, 320, 330, 800,
		 "Flower (c) 1999 Pieter S. van der Meulen");
  free (tempPtr);

  panda_setfont (demo, tempPtr =
		 panda_createfont (demo, "Helvetica-Bold", 1,
				   "MacRomanEncoding"));
  panda_textboxrot (demo, currPage, 200, 30, 230,
		 80, 45.0, "With new improved angled text!");
  free (tempPtr);

  ///////////////////////////////////////////////////////////////////////////
  // Text functionality (with a few images thrown in as well)
  ///////////////////////////////////////////////////////////////////////////

  currPage = panda_newpage (demo, panda_pagesize_a4);

  // I am not drawing a multiline string here because I am not sure how to 
  // represent this in the PDF at the moment
  sprintf (tempString,
	   "Hello %c5World! %cMy name %c5is Panda!\nAnd I am a PDF "
	   "generator\nI handle multiple line text ok. Once you have set a "
	   "leading.",
	   4, 6, 5);
  panda_setleading(demo, 15.0);
  panda_textbox (demo, currPage, 10, 10, 100, 500, tempString);

  panda_setfont (demo, tempPtr = panda_createfont (demo, "Symbol", 1,
						   "MacRomanEncoding"));
  panda_textbox (demo, currPage, 50, 10, 100, 200, "Symbol");
  free (tempPtr);

  panda_setfont (demo, tempPtr =
		 panda_createfont (demo, "Helvetica-Bold", 1,
				   "MacRomanEncoding"));
  panda_textbox (demo, currPage, 70, 30, 100, 80, "A line in Helvetica-Bold");
  free (tempPtr);

  panda_imagebox (demo, currPage, 100, 100, 150, 150, "images/gnu-head.jpg",
		  panda_image_jpeg);
  panda_textbox (demo, currPage, 90, 110, 200, 200, "INFRONTINFRONTINFRONT");

  panda_textbox (demo, currPage, 190, 210, 300, 300, "BEHINDBEHINDBEHIND");
  panda_imagebox (demo, currPage, 200, 200, 317, 317, "images/gnu_box.jpg",
		  panda_image_jpeg);

  panda_textbox (demo, currPage, 300, 10, 400, 50,
		 "A second textbox on the page");

  ///////////////////////////////////////////////////////////////////////////
  // Demonstrate the supported text modes
  ///////////////////////////////////////////////////////////////////////////

  currPage = panda_newpage (demo, panda_pagesize_a4);
  panda_setleading (demo, 16.0);

  for (lineDepth = 0; lineDepth < 8; lineDepth++)
    {
      panda_setfontmode (demo, panda_textmode_normal);

      switch (lineDepth)
	{
	case panda_textmode_normal:
	  panda_textbox (demo, currPage, 20 + (lineDepth * 20), 10,
			 40 + (lineDepth * 20), 400, "Normal");
	  break;

	case panda_textmode_outline:
	  panda_textbox (demo, currPage, 20 + (lineDepth * 20), 10,
			 40 + (lineDepth * 20), 400, "Outline");
	  break;

	case panda_textmode_filledoutline:
	  panda_textbox (demo, currPage, 20 + (lineDepth * 20), 10,
			 40 + (lineDepth * 20), 400, "FilledOutline");
	  break;

	case panda_textmode_invisible:
	  panda_textbox (demo, currPage, 20 + (lineDepth * 20), 10,
			 40 + (lineDepth * 20), 400, "Invisible");
	  break;

	case panda_textmode_filledclipped:
	  panda_textbox (demo, currPage, 20 + (lineDepth * 20), 10,
			 40 + (lineDepth * 20), 400, "FilledClipped");
	  break;

	case panda_textmode_strokedclipped:
	  panda_textbox (demo, currPage, 20 + (lineDepth * 20), 10,
			 40 + (lineDepth * 20), 400, "Stroked Clipped");
	  break;

	case panda_textmode_filledstrokedclipped:
	  panda_textbox (demo, currPage, 20 + (lineDepth * 20), 10,
			 40 + (lineDepth * 20), 400,
			 "Filled Stroked Clipped");
	  break;

	case panda_textmode_clipped:
	  panda_textbox (demo, currPage, 20 + (lineDepth * 20), 10,
			 40 + (lineDepth * 20), 400, "Clipped");
	  break;
	}

      panda_setcharacterspacing (demo, (double) lineDepth);
      panda_setwordspacing (demo, (double) lineDepth * 10);
      panda_sethorizontalscaling (demo, (double) 1 - (lineDepth * 0.1));

      panda_setfontmode (demo, lineDepth);
      panda_textbox (demo, currPage, 20 + (lineDepth * 20), 200,
		     40 + (lineDepth * 20), 400,
		     "Demonstration of a text mode");
    }

  ///////////////////////////////////////////////////////////////////////////
  // Demonstrate the supported lines and curve thingies -- note that no
  // graphics state is held from the previous set of lines, so you'll need
  // to rebuild it each time.
  ///////////////////////////////////////////////////////////////////////////

  currPage = panda_newpage (demo, panda_pagesize_a4);

  panda_setfontmode (demo, panda_textmode_normal);
  panda_textbox (demo, currPage, 40, 10, 55, 200,
		 "Please note that these shapes are lines, and there is no");
  panda_textbox (demo, currPage, 60, 10, 75, 200,
		 "requirement to have the shapes closed...");

  // Straight lines of all types -- stroked
  panda_setlinestart (currPage, 110, 110);
  panda_addlinesegment (currPage, 160, 130);
  panda_addlinesegment (currPage, 210, 186);
  panda_addlinesegment (currPage, 96, 22);
  panda_closeline (currPage);
  panda_strokeline (currPage);
  panda_endline (currPage);

  // Now some curves -- stroked
  panda_setlinestart (currPage, 210, 210);
  panda_addcubiccurvesegment (currPage, 310, 210, 225, 300, 275, 400);
  panda_addquadraticcurvesegmentone (currPage, 160, 160, 200, 225);
  panda_addquadraticcurvesegmenttwo (currPage, 210, 210, 250, 375);
  panda_closeline (currPage);
  panda_strokeline (currPage);
  panda_endline (currPage);

  // Rectangles -- stroked
  panda_rectangle (currPage, 210, 210, 310, 310);
  panda_strokeline (currPage);
  panda_endline (currPage);

  // Straight lines of all types -- stroked and filled
  panda_setlinecolor (currPage, 99, 33, 255);
  panda_setfillcolor (currPage, 112, 38, 300);
  panda_setlinestart (currPage, 110, 310);
  panda_setlinewidth (currPage, 5);
  panda_addlinesegment (currPage, 160, 330);
  panda_addlinesegment (currPage, 210, 386);
  panda_addlinesegment (currPage, 96, 222);
  panda_closeline (currPage);
  panda_fillline (currPage);
  panda_endline (currPage);

  // Now some curves -- stroked and filled
  panda_setfillcolor (currPage, 112, 138, 37);
  panda_setlinestart (currPage, 210, 410);
  panda_setlinewidth (currPage, 5);
  panda_addcubiccurvesegment (currPage, 310, 410, 225, 500, 275, 600);
  panda_addquadraticcurvesegmentone (currPage, 160, 360, 200, 425);
  panda_addquadraticcurvesegmenttwo (currPage, 210, 410, 250, 575);
  panda_closeline (currPage);
  //panda_strokeline (currPage);
  panda_fillline (currPage);
  panda_endline (currPage);

  // Rectangles -- stroked filled
  panda_setfillcolor (currPage, 38, 38, 38);
  panda_setlinewidth (currPage, 5);
  panda_rectangle (currPage, 410, 210, 510, 310);
  //panda_strokeline (currPage);
  panda_fillline (currPage);
  panda_endline (currPage);

  // Straight lines of all types -- stroked and capped
  panda_setlinewidth (currPage, 10);
  panda_setlinestart (currPage, 100, 600);
  panda_addlinesegment (currPage, 200, 600);
  panda_strokeline (currPage);
  panda_endline (currPage);

  panda_setlinewidth (currPage, 10);
  panda_setlinecap (currPage, panda_linecap_butt);
  panda_setlinestart (currPage, 100, 625);
  panda_addlinesegment (currPage, 200, 625);
  panda_strokeline (currPage);
  panda_endline (currPage);

  panda_setlinewidth (currPage, 10);
  panda_setlinecap (currPage, panda_linecap_round);
  panda_setlinestart (currPage, 100, 650);
  panda_addlinesegment (currPage, 200, 650);
  panda_strokeline (currPage);
  panda_endline (currPage);

  panda_setlinewidth (currPage, 10);
  panda_setlinecap (currPage, panda_linecap_projectedsquare);
  panda_setlinestart (currPage, 100, 675);
  panda_addlinesegment (currPage, 200, 675);
  panda_strokeline (currPage);
  panda_endline (currPage);

  // Mitre joints
  panda_setlinewidth (currPage, 10);
  panda_setlinecap (currPage, panda_linecap_butt);
  panda_setlinestart (currPage, 300, 600);
  panda_addlinesegment (currPage, 350, 650);
  panda_addlinesegment (currPage, 400, 600);
  panda_strokeline (currPage);
  panda_endline (currPage);

  panda_setlinewidth (currPage, 10);
  panda_setlinejoin (currPage, panda_linejoin_miter);
  panda_setlinestart (currPage, 300, 625);
  panda_addlinesegment (currPage, 350, 675);
  panda_addlinesegment (currPage, 400, 625);
  panda_strokeline (currPage);
  panda_endline (currPage);

  panda_setlinewidth (currPage, 10);
  panda_setlinejoin (currPage, panda_linejoin_round);
  panda_setlinestart (currPage, 300, 650);
  panda_addlinesegment (currPage, 350, 700);
  panda_addlinesegment (currPage, 400, 650);
  panda_strokeline (currPage);
  panda_endline (currPage);

  panda_setlinewidth (currPage, 10);
  panda_setlinejoin (currPage, panda_linejoin_bevel);
  panda_setlinestart (currPage, 300, 675);
  panda_addlinesegment (currPage, 350, 725);
  panda_addlinesegment (currPage, 400, 675);
  panda_strokeline (currPage);
  panda_endline (currPage);

  // Do some work with line dashing
  panda_setlinedash (currPage, 1, 0, 0);
  panda_setlinejoin (currPage, panda_linejoin_round);

  panda_setlinestart (currPage, 100, 800);
  panda_addlinesegment (currPage, 100, 750);
  panda_addlinesegment (currPage, 140, 800);
  panda_closeline (currPage);
  panda_strokeline (currPage);
  panda_endline (currPage);

  panda_setlinedash (currPage, 3, 3, 0);
  panda_setlinestart (currPage, 150, 800);
  panda_addlinesegment (currPage, 150, 750);
  panda_addlinesegment (currPage, 190, 800);
  panda_closeline (currPage);
  panda_strokeline (currPage);
  panda_endline (currPage);

  panda_setlinedash (currPage, 2, 1, 1);
  panda_setlinestart (currPage, 200, 800);
  panda_addlinesegment (currPage, 200, 750);
  panda_addlinesegment (currPage, 240, 800);
  panda_closeline (currPage);
  panda_strokeline (currPage);
  panda_endline (currPage);

  ///////////////////////////////////////////////////////////////////////////
  // Why not have some annotations as well?
  ///////////////////////////////////////////////////////////////////////////

  panda_textannotation(demo, currPage, panda_true, "Hello", "Text annotation",
		       10, 10, 50, 50, 1.0, 0.0, 0.0,
		       panda_icon_comment, 0);
  panda_lineannotation(demo, currPage, "Line annotation", "Line annotation",
		       50, 50, 150, 150, 
		       150, 150, 200, 200, 
		       0.0, 1.0, 0.0, 0);

  ///////////////////////////////////////////////////////////////////////////
  // We can also setup template pages to make life a little easier (and the
  // document a little smaller)
  ///////////////////////////////////////////////////////////////////////////

  templatePage = panda_newtemplate(demo, panda_pagesize_a4);
  panda_setlinestart (templatePage, 100, 800);
  panda_addlinesegment (templatePage, 100, 750);
  panda_addlinesegment (templatePage, 140, 800);
  panda_closeline (templatePage);
  panda_strokeline (templatePage);
  panda_endline (templatePage);

  panda_setlinedash (templatePage, 3, 3, 0);
  panda_setlinestart (templatePage, 150, 800);
  panda_addlinesegment (templatePage, 150, 750);
  panda_addlinesegment (templatePage, 190, 800);
  panda_closeline (templatePage);
  panda_strokeline (templatePage);
  panda_endline (templatePage);

  currPage = panda_newpage(demo, panda_pagesize_a4);
  panda_applytemplate(demo, currPage, templatePage);

  ///////////////////////////////////////////////////////////////////////////
  // Let's try some transitions
  ///////////////////////////////////////////////////////////////////////////
  
  for(trans = 0; trans < panda_pagetrans_none; trans++)
    {
      currPage = panda_newpage(demo, panda_pagesize_a4);
      panda_pageduration(demo, currPage, 5);
      panda_transduration(demo, currPage, 5.0);
      panda_transstyle(demo, currPage, trans);
      
      panda_setlinecolor (currPage, trans * 20, 0, trans * 10);
      panda_setfillcolor (currPage, trans * 20, 0, trans * 10);
      panda_setlinestart (currPage, 0, 0);
      panda_setlinewidth (currPage, 5);
      panda_addlinesegment (currPage, 1000, 0);
      panda_addlinesegment (currPage, 1000, 1000);
      panda_addlinesegment (currPage, 0, 1000);
      panda_closeline (currPage);
      panda_fillline (currPage);
      panda_endline (currPage);
    }

  currPage = panda_newpage(demo, panda_pagesize_a4);

  // Finished all the demoing, close the PDF document
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
