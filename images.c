/******************************************************************************
  images.c

  Change Control:                                                      DDMMYYYY
    Michael Still    File created                                      29072000

  Purpose:
    Image insertion is all handled by one fuction, which then goes off and
    calls the functions that know about particular image types...
******************************************************************************/

#include "constants.h"
#include "functions.h"

// A redistribution point for image insertions based on type of image
void imagebox(pdf *output, page *target, int top, int left, int bottom, 
  int right, char *filename, int type){
  // We simply call the right function for that image type
  switch(type){
  case gTiff:
    insertTiff(output, target, top, left, bottom, right, filename);
    break;
  }
}

