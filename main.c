/******************************************************************************
	GNUPDF

	Change Control:													DDMMYYYY
		Michael Still		File created							03062000

	Purpose:
		The GNUPDF codebase can be used in a variety of ways, one of them is as
		as stand alone application. This file supports that methodology.
******************************************************************************/

#define		GLOBALS			here
#include	"functions.h"
#include	"constants.h"

int main(int argc, char *argv[]){
	pdf		*demo;
	page	*firstpage;

	// Initialise the library
	initgnupdf();

	// Open our demo PDF
	if((demo = pdfopen("output.pdf", "w")) == NULL)
		error("demo: could not open output.pdf to write to.");

	// Add a page to the PDF
	firstpage = pdfpage(demo, gPageSizeA4);

	// Put some text onto it
	textbox(demo, firstpage, 10, 10, 100, 30, "Hello World!");

	pdfclose(demo);
}