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

#if defined DEBUG
  printf("Started inserting an image.\n");
#endif

  // We simply call the right function for that image type
  switch(type){
  case gImageTiff:
    insertTiff(output, target, top, left, bottom, right, filename);
    break;
  }

#if defined DEBUG
  printf("Finished inserting an image.\n");
#endif
}

// This function will insert a TIFF image into a PDF
void insertTiff(pdf *output, page *target, int top, int left, int bottom,
  int right, char *filename){
  TIFF          *image;
  object        *imageObj, *subdict, *xobjrefsubdict;
  int           stripCount;
  tsize_t       stripSize;
  unsigned long imageOffset;
  char          *tempstream, xobjstr[255];
  uint16        tiffResponse16;
  uint32        tiffResponse32;

  // Open the file and make sure that it exists and is a TIFF file
  if((image = TIFFOpen(filename, "r")) == NULL)
    error("Could not open the specified TIFF image.");

#if defined DEBUG
  printf("Inserting a TIFF image on page with object number %d.\n",
    target->obj->number);
#endif

  // Now we need an object to contain the tiff
  imageObj = newobject(output, gNormal);
  addchild(target->obj, imageObj);

  // We do not use the xobject stream here because the image is not inline (it
  // is external to the stream that the page is described in)

  // We make an object not just a dictionary because this is what
  // adddictitem needs
  xobjrefsubdict = newobject(output, gPlaceholder);
  adddictitem(xobjrefsubdict->dict, filename, gObjValue, imageObj);
  adddictitem(target->obj->dict, "XObject", gDictionaryValue, xobjrefsubdict->dict);

  // We put some information based on a stat of the image file into the object
  // This will allow us to determine if this file's image is included in the
  // document again later, which will allow us avoid repetition of the same
  // image data for no apparent reason -- note that this is not complete yet
  // and we still need to add the existance check before the object is created
  // and some form of object searching by a rational and efficient manner (a
  // binary search tree?)

  // We now add some dictionary elements to the image object to say that it is
  // a TIFF image
  adddictitem(imageObj->dict, "Type", gTextValue, "XObject");
  adddictitem(imageObj->dict, "Subtype", gTextValue, "Image");
  
  // This line will need to be changed to gaurantee that the internal name is
  // unique unless the actual image is the same
  adddictitem(imageObj->dict, "Name", gTextValue, filename);

  adddictitem(imageObj->dict, "Filter", gTextValue, "CCITTFaxDecode");
  
  // Bits per component is per colour component, not per sample. Does this
  // matter?
  if(TIFFGetField(image, TIFFTAG_BITSPERSAMPLE, &tiffResponse16) != 0)
    adddictitem(imageObj->dict, "BitsPerComponent", gIntValue, tiffResponse16);
  else error("Could not get the colour depth for the tiff image.");

  // The colour device will change based on this number as well
  switch(tiffResponse16){
  case 1:
    adddictitem(imageObj->dict, "ColorSpace", gTextValue, "DeviceGray");
    break;

  default:
    adddictitem(imageObj->dict, "ColorSpace", gTextValue, "DeviceRGB");
    break;
  }

  /****************************************************************************
     We need to add a sub-dictionary with the parameters for the compression
     filter in it.
  ****************************************************************************/

  // We make an object not just a dictionary because this is what
  // adddictitem needs
  subdict = newobject(output, gPlaceholder);

  // There are several parameters that we need to set for the compression to be
  // able to do its thing
  adddictitem(subdict->dict, "K", gIntValue, -1);

  // Width of the image
  if(TIFFGetField(image, TIFFTAG_IMAGEWIDTH, &tiffResponse32) != 0){
    adddictitem(subdict->dict, "Columns", gIntValue, tiffResponse32);
    adddictitem(imageObj->dict, "Width", gIntValue, tiffResponse32);
  }
  else error("Could not get the width of the TIFF image.");

  // Height of the image
  if(TIFFGetField(image, TIFFTAG_IMAGELENGTH, &tiffResponse32) != 0){
    adddictitem(subdict->dict, "Rows", gIntValue, tiffResponse32);
    adddictitem(imageObj->dict, "Height", gIntValue, tiffResponse32);
  }
  else error("Could not get the height of the TIFF image.");

  // And put this into the PDF
  adddictitem(imageObj->dict, "Resources", gDictionaryValue, subdict->dict);

  /****************************************************************************
     Insert the image
  ****************************************************************************/

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

