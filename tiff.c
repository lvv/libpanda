/******************************************************************************
  tiff.c

  Change Control:                                                      DDMMYYYY
    Michael Still    File created                                      29072000

  Purpose:
    TIFF handling functions for Panda
******************************************************************************/

#include "constants.h"
#include "functions.h"
#include <tiffio.h>

// Insert a tiff image
void insertTiff(pdf *output, page *target, int top, int left, int bottom,
		int right, char *filename){
  TIFF   *input;

  // Can we open the named input image?
  if((input = TIFFOpen(filename, "r")) == NULL)
    error("Could not open the TIFF image named.");

}
