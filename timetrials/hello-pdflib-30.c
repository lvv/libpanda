/* hello.c
 * Copyright (C) 1997-2000 Thomas Merz. All rights reserved.
 * PDFlib client: hello example in C
 *
 */

#include <stdio.h>
#include <stdlib.h>

#include "pdflib.h"

int
main(void)
{
    PDF *p;
    int font;

    p = PDF_new();

    /* open new PDF file */
    if (PDF_open_file(p, "output.pdf") == -1) {
	fprintf(stderr, "Error: cannot open PDF file hello_c.pdf.\n");
	exit(2);
    }

    PDF_set_info(p, "Creator", "hello.c");
    PDF_set_info(p, "Author", "Thomas Merz");
    PDF_set_info(p, "Title", "Hello, world (C)!");

    PDF_begin_page(p, a4_width, a4_height);	/* start a new page	*/

    font = PDF_findfont(p, "Helvetica-Bold", "host", 0);

    PDF_setfont(p, font, 24);
    PDF_set_text_pos(p, 50, 700);
    PDF_show(p, "Hello, world!");
    PDF_continue_text(p, "(says C)");
    PDF_end_page(p);				/* close page		*/

    PDF_close(p);				/* close PDF document	*/

    exit(0);
}
