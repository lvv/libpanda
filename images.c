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
#include <jpeglib.h>
#include <sys/stat.h>
#include <unistd.h>

// A redistribution point for image insertions based on type of image
void imagebox(pdf *output, page *target, int top, int left,
  int bottom, int right, char *filename, int type){
  object        *imageObj, *xobjrefsubdict, *xobjrefsubsubdict;

#if defined DEBUG
  printf("Started inserting an image.\n");
#endif

  // Now we need an object to contain the tiff
  imageObj = newobject(output, gNormal);
  addchild(target->obj, imageObj);
  
  // We make an object not just a dictionary because this is what
  // adddictitem needs
  xobjrefsubsubdict = newobject(output, gPlaceholder);
  adddictitem(xobjrefsubsubdict->dict, filename, gObjValue, imageObj);
  
  xobjrefsubdict = newobject(output, gPlaceholder);
  adddictitem(xobjrefsubdict->dict, "XObject", gDictionaryValue, 
    xobjrefsubsubdict->dict);
  
  // And put this into the PDF
  adddictitem(target->obj->dict, "Resources", gDictionaryValue, 
    xobjrefsubdict->dict);

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

  // --------------------------------------------------------------------------
  // Now we do the things that are image format specific...
  switch(type){
  case gImageTiff:
    insertTiff(output, target, imageObj, filename);
    break;

  case gImageJpeg:
    insertJpeg(output, target, imageObj, filename);
    break;

  case gImagePNG:
    insertPNG(output, target, imageObj, filename);
    break;
  }
  // --------------------------------------------------------------------------
  
  // We also need to add some information to the text stream for the contents
  // object for the page that the image is being displayed on. This information
  // consists of the following:
  //  - save the current graphics state (q operator, p 386 of spec)
  //  - setup the current transformation matrix (ctm, s 3.2 and p 323 of spec)
  //    such that the image is scaled correctly (cm operator)
  //  - modify the ctm to shift the image to where it is meant to be on the
  //    the page
  //  - use the image xobject we have created (Do operator, p 348 of spec)
  //  - restore the graphics state to the way it was previously (Q operator,
  //    p 386 of spec)
  target->contents->xobjectstream = 
    streamprintf(target->contents->xobjectstream, 
    "\nq\n%.2f %.2f %.2f %.2f %.2f %.2f cm\n",
    
    // The first matrix
    1.0, // xscale
    0.0, // rot and scale?
    0.0, // ???
    1.0, // yscale
    (double) left,                     // x start
    (double) target->height - bottom); // y start

  target->contents->xobjectstream =
    streamprintf(target->contents->xobjectstream,
    "%.2f %.2f %.2f %.2f %.2f %.2f cm\n",

    // The second matrix
    (double) (right - left), // x size
    0.0, // ???
    0.0, // ???
    (double) (bottom - top), // y size
    0.0, // ???
    0.0); // ???

  target->contents->xobjectstream = 
    streamprintf(target->contents->xobjectstream,
    "/%s Do\nQ\n\n", filename);
  
#if defined DEBUG
  printf("Finished inserting an image.\n");
#endif
}

// This function will insert a TIFF image into a PDF
void insertTiff(pdf *output, page *target, object *imageObj, char *filename){
  TIFF          *image;
  object        *subdict;
  int           stripCount;
  tsize_t       stripSize;
  unsigned long imageOffset;
  char          *tempstream;
  uint16        tiffResponse16;
  uint32        tiffResponse32;

  // Open the file and make sure that it exists and is a TIFF file
  if((image = TIFFOpen(filename, "r")) == NULL)
    error("Could not open the specified TIFF image.");

#if defined DEBUG
  printf("Inserting a TIFF image on page with object number %d.\n",
    target->obj->number);
#endif

  // This dictionary item is TIFF specific
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
  adddictitem(imageObj->dict, "DecodeParms", gDictionaryValue, subdict->dict);

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

// This function will insert a JPEG image into a PDF
void insertJpeg(pdf *output, page *target, object *imageObj, char *filename){
  struct jpeg_decompress_struct   cinfo;
  struct jpeg_error_mgr           jerr;
  FILE                            *image;
  int                             c;
  unsigned long                   imageBufSize;

#if defined DEBUG
  printf("Inserting a JPEG image on page with object number %d.\n",
    target->obj->number);
#endif

  // Open the file
  if((image = fopen(filename, "rb")) == NULL)
    error("Could not open the specified JPEG file.");
    
  // Setup the decompression options
  cinfo.err = jpeg_std_error(&jerr);
  jpeg_stdio_src(&cinfo, image);

  // Start decompressing
  jpeg_create_decompress(&cinfo);
  jpeg_stdio_src(&cinfo, image);
  jpeg_read_header(&cinfo, TRUE);

  // This dictionary item is JPEG specific
  adddictitem(imageObj->dict, "Filter", gTextValue, "DCTDecode");
  
  // Bits per component -- I'm not sure exactly how this works with libjpeg.
  // Is it possible to have a black and white jpeg?
  adddictitem(imageObj->dict, "BitsPerComponent", gIntValue, 
    cinfo.data_precision);

  // The colour device will change based on this number as well
  switch(cinfo.jpeg_color_space){
  case JCS_GRAYSCALE:
    adddictitem(imageObj->dict, "ColorSpace", gTextValue, "DeviceGray");
    break;

  default:
    adddictitem(imageObj->dict, "ColorSpace", gTextValue, "DeviceRGB");
    break;
  }

  /****************************************************************************
     Some details of the image
  ****************************************************************************/

  adddictitem(imageObj->dict, "Width", gIntValue, cinfo.image_width);
  adddictitem(imageObj->dict, "Height", gIntValue, cinfo.image_height);

  /****************************************************************************
    Read the image into it's memory buffer
  ****************************************************************************/

  imageBufSize = 0;
  imageObj->binarystreamLength = 0;
  
  if((image = fopen(filename , "r")) == NULL)
    error("Could not open the JPEG file.");

  while((c = fgetc(image)) != EOF){
    // We need to grow the buffer
    if(imageBufSize == imageObj->binarystreamLength){
      imageBufSize += 1024;
      
      if((imageObj->binarystream = realloc(imageObj->binarystream,
	imageBufSize)) == NULL)
	error("Could not make enough space for the JPEG image.");
    }

    // Store the info
    imageObj->binarystream[imageObj->binarystreamLength++] = c;
  }

  imageObj->binarystream[imageObj->binarystreamLength++] = 0;
  fclose(image);

  // This cleans things up for us in the JPEG library
  jpeg_destroy_decompress(&cinfo);
}

void insertPNG(pdf *output, page *target, object *imageObj, char *filename){
  
#if defined DEBUG
  printf("Inserting a PNG / Flate image on page with object number %d.\n",
    target->obj->number);
#endif

}
