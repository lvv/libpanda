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

int main(int argc, char *argv[]){
  CPDFdoc *pdf;
  CPDFdocLimits *limits;
  int    count;

    /* == Initialization == */
  if((limits = malloc(sizeof(CPDFdocLimits))) == NULL) exit(42);
  limits->nMaxPages = 10001;
  limits->nMaxFonts = -1;
  limits->nMaxImages = -1;
  limits->nMaxAnnots = -1;
  limits->nMaxObjects = 30000;

    pdf = cpdf_open(0, limits);
    cpdf_init(pdf);

    for(count = 0; count < 10000; count++){
      cpdf_pageInit(pdf, count + 1, PORTRAIT, A4, A4);
      cpdf_setFont(pdf, "Times-Italic", "MacRomanEncoding", 16.0);
      cpdf_beginText(pdf, 0);
      cpdf_rawText(pdf, 10.0, 10.0, 0.0, "x");
      cpdf_endText(pdf);
    }

    cpdf_finalizeAll(pdf);			/* PDF file/memstream is actually written here */
    cpdf_savePDFmemoryStreamToFile(pdf, "output.pdf");

    /* == Clean up == */
    cpdf_close(pdf);			/* shut down */
    return(0);
}

