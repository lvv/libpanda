#include <stdio.h>
#include <math.h>
#include <panda/functions.h>
#include <panda/constants.h>

int *drawcircle (panda_page *, int, int, int, int);

int
main (int argc, char *argv[])
{
  panda_pdf *document;
  panda_page *mypage;
  int *results, count;

  if ((document = panda_open ("circles.pdf", "w")) == NULL)
    panda_error (panda_true, "Could not create circles.pdf");

  mypage = panda_newpage (document, panda_pagesize_a4);

  // We are going to draw some circles to prove that I can...
  results = drawcircle (mypage, 200, 200, 100, 10);

  for (count = 0; count < 20; count += 2)
    {
      printf ("[%d, %d]\n", results[count], results[count + 1]);

      free (drawcircle (mypage, results[count], results[count + 1], 10, 30));
    }

  // Close the PDF
  panda_close (document);
}

int *
drawcircle (panda_page * mypage, int xc, int yc, int len, int lines)
{
  int angle, whichline = 0, xe, ye;
  int *endpoints;

  // Allocate memory for the endpoints
  endpoints = (int *) panda_xmalloc (lines * 2 * sizeof (int));

  for (angle = 0; angle < 360; angle += (360 / lines))
    {
      xe = xc + sin ((double) angle * 3.1415 / 180) * len;
      ye = yc - cos ((double) angle * 3.1415 / 180) * len;

      panda_setlinestart (mypage, xc, yc);
      panda_addlinesegment (mypage, xe, ye);
      panda_strokeline (mypage);
      panda_endline (mypage);

      endpoints[whichline] = xe;
      endpoints[whichline + 1] = ye;
      whichline += 2;
    }

  return endpoints;
}
