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
#include <tiffio.h>

// A redistribution point for image insertions based on type of image
void imagebox(pdf *output, page *target, int top, int left, int bottom, 
  int right, char *filename, int type){
  // We simply call the right function for that image type
  switch(type){
  case gImageTiff:
    insertTiff(output, target, top, left, bottom, right, filename);
    break;
  }
}

// This function will insert a TIFF image into a PDF
void insertTiff(pdf *output, page *target, int top, int left, int bottom,
  int right, char *filename){
  TIFF          *image;
  object        *imageObj;
  int           stripCount;
  tsize_t       stripSize;
  unsigned long imageOffset;
  char          *tempstream;

  // Open the file and make sure that it exists and is a TIFF file
  if((image = TIFFOpen(filename, "r")) == NULL)
    error("Could not open the specified TIFF image.");

  // Now we need an object to contain the tiff
  imageObj = newobject(output, gNormal);
  addchild(target->obj, imageObj);

  // We put some information based on a stat of the image file into the object
  // This will allow us to determine if this file's image is included in the
  // document again later, which will allow us avoid repetition of the same
  // image data for no apparent reason -- note that this is not complete yet
  // and we still need to add the existance check before the object is created
  // and some form of object searching by a rational and efficient manner (a
  // binary search tree?)


  // We now add some dictionary elements to the image object to say that it is
  // a TIFF image
  adddictitem(imageObj, "Filter", gTextValue, "CCITTFaxDecode");
  
  // We also need to add a binary stream to the object and put the image
  // data into this stream
  stripSize = TIFFStripSize(image);
  imageOffset = 0;

  if((imageObj->binarystream = 
      malloc(TIFFNumberOfStrips(image) * stripSize)) == NULL)
    error("Insufficient memory for TIFF image insertion.");

  for(stripCount = 0; stripCount < TIFFNumberOfStrips(image); stripCount++){
    imageOffset += TIFFReadRawStrip(image, stripCount, 
      imageObj->binarystream + imageOffset, stripSize);
  }

  // We might have too much memory, truncate to the size we have actually read
  if((tempstream = realloc(imageObj->binarystream, imageOffset)) != NULL)
    imageObj->binarystream = tempstream;

  // The image offset is the total size of the binary stream as well
  imageObj->binarystreamLength = imageOffset;
}

