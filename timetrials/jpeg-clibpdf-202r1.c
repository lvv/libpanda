/* Minimal.c -- A simple drawing and one-line text example.
		It can't be really simpler than this without being silly.

 * Copyright (C) 1998, 1999 FastIO Systems, All Rights Reserved.
 * For conditions of use, license, and distribution, see LICENSE.pdf
 * included in the distribution or http://www.fastio.com/LICENSE.pdf

cc -Wall -o Minimal -I/usr/local/include Minimal.c -lcpdfpm

HP-UX B.11.00 (assuming header and lib are installed, or in examples directory below):
cc -O -Aa -Ae +Z +DA1.0 -DHPUX  -I /usr/local/include -I.. -L.. -o Minimal Minimal.c -lcpdfp -lm

1999-08-24  [io] for v2.00
1998-07-18  [IO]
*/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "cpdflib.h"

int main(int argc, char *argv[])
{
CPDFdoc *pdf;
 float   width = 100.0, height = 100.0, xscale = 0.0, yscale = 0.0;

    /* == Initialization == */
    pdf = cpdf_open(0, NULL);
    cpdf_init(pdf);
    cpdf_pageInit(pdf, 1, PORTRAIT, A4, A4);		/* page orientation */


    cpdf_importImage(pdf, "jello.jpg", JPEG_IMG, 0.8, 5.6, 0.0,
		     &width, &height, &xscale, &yscale, 1);

    cpdf_finalizeAll(pdf);			/* PDF file/memstream is actually written here */
    cpdf_savePDFmemoryStreamToFile(pdf, "output.pdf");

    /* == Clean up == */
    cpdf_close(pdf);			/* shut down */
    return(0);
}

