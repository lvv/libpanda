#include <stdio.h>
#include <math.h>
#include <panda/functions.h>
#include <panda/constants.h>

// Good combinations (not too slow, and nice): [2, 3, 2]

void drawcircle (panda_page *, int, int, int, int);

int degree, reduce, minlen;

int
main (int argc, char *argv[])
{
  panda_pdf *document;
  panda_page *mypage;

  if ((document = panda_open ("morecircles.pdf", "w")) == NULL)
    panda_error (panda_true, "Could not create morecircles.pdf");

  mypage = panda_newpage (document, panda_pagesize_a4);

  // Get the arguements
  reduce = atoi (argv[1]);
  degree = atoi (argv[2]);
  minlen = atoi (argv[3]);

  // Draw a whole bunch of circles
  drawcircle (mypage, 200, 200, 100, degree);

  // Close the PDF
  panda_close (document);
}

void
drawcircle (panda_page * mypage, int xc, int yc, int len, int lines)
{
  int angle, xe, ye;

  if (len < minlen)
    return;

  for (angle = 0; angle < 360; angle += (360 / lines))
    {
      printf ("[%d %d %d] ", len, angle, degree);

      xe = xc + sin ((double) angle * 3.1415 / 180) * len;
      ye = yc - cos ((double) angle * 3.1415 / 180) * len;

      panda_setlinestart (mypage, xc, yc);
      panda_addlinesegment (mypage, xe, ye);
      panda_strokeline (mypage);
      panda_endline (mypage);

      // Now do the things that come off this line
      drawcircle (mypage, xe, ye, len / reduce, lines);
    }
}
