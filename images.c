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
#include <math.h>
#include <jpeglib.h>
#include <sys/stat.h>
#include <unistd.h>
#include <pthread.h>

static tsize_t libtiffDummyReadProc (thandle_t fd, tdata_t buf, tsize_t size);
static tsize_t libtiffDummyWriteProc (thandle_t fd, tdata_t buf,
				      tsize_t size);
static toff_t libtiffDummySeekProc (thandle_t fd, toff_t off, int i);
static int libtiffDummyCloseProc (thandle_t fd);

char *globalTiffBuffer;
unsigned long globalTiffBufferOffset;
pthread_mutex_t tiffConvMutex = PTHREAD_MUTEX_INITIALIZER;

// A redistribution point for image insertions based on type of image
void
imagebox (pdf * output, page * target, int top, int left,
	  int bottom, int right, char *filename, int type)
{
  object *imageObj, *xobjrefsubdict, *xobjrefsubsubdict;

#if defined DEBUG
  printf ("Started inserting an image.\n");
#endif

  // Now we need an object to contain the tiff
  imageObj = newobject (output, gNormal);
  addchild (target->obj, imageObj);

  // We make an object not just a dictionary because this is what
  // adddictitem needs
  xobjrefsubsubdict = newobject (output, gPlaceholder);
  adddictitem (xobjrefsubsubdict->dict, filename, gObjValue, imageObj);

  xobjrefsubdict = newobject (output, gPlaceholder);
  adddictitem (xobjrefsubdict->dict, "XObject", gDictionaryValue,
	       xobjrefsubsubdict->dict);

  // And put this into the PDF
  adddictitem (target->obj->dict, "Resources", gDictionaryValue,
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
  adddictitem (imageObj->dict, "Type", gTextValue, "XObject");
  adddictitem (imageObj->dict, "Subtype", gTextValue, "Image");

  // This line will need to be changed to gaurantee that the internal name is
  // unique unless the actual image is the same
  adddictitem (imageObj->dict, "Name", gTextValue, filename);

  // --------------------------------------------------------------------------
  // Now we do the things that are image format specific...
  switch (type)
    {
    case gImageTiff:
      insertTiff (output, target, imageObj, filename);
      break;

    case gImageJpeg:
      insertJpeg (output, target, imageObj, filename);
      break;

    case gImagePNG:
      insertPNG (output, target, imageObj, filename);
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
    streamprintf (target->contents->xobjectstream,
		  "\nq\n%.2f %.2f %.2f %.2f %.2f %.2f cm\n",
		  // The first matrix
		  1.0,		// xscale
		  0.0,		// rot and scale?
		  0.0,		// ???
		  1.0,		// yscale
		  (double) left,	// x start
		  (double) target->height - bottom);	// y start

  target->contents->xobjectstream =
    streamprintf (target->contents->xobjectstream,
		  "%.2f %.2f %.2f %.2f %.2f %.2f cm\n",
		  // The second matrix
		  (double) (right - left),	// x size
		  0.0,		// ???
		  0.0,		// ???
		  (double) (bottom - top),	// y size
		  0.0,		// ???
		  0.0);		// ???

  target->contents->xobjectstream =
    streamprintf (target->contents->xobjectstream, "/%s Do\nQ\n\n", filename);

#if defined DEBUG
  printf ("Finished inserting an image.\n");
#endif
}

// This function will insert a TIFF image into a PDF
void
insertTiff (pdf * output, page * target, object * imageObj, char *filename)
{
  /**************************************************************************
    Some notes about TIFF support inside PDF files.

     - MSB2LSB is the only byte fillorder that is supported.
     - The images must be converted to single strip
     - G3 and G4 are supported
  **************************************************************************/

  TIFF *image, *conv;
  object *subdict;
  int stripCount, stripMax;
  tsize_t stripSize;
  unsigned long imageOffset;
  char *tempstream, *stripBuffer;
  uint16 tiffResponse16, compression, fillorder;
  uint32 height, width;

  // Open the file and make sure that it exists and is a TIFF file
  if ((image = TIFFOpen (filename, "r")) == NULL)
    error ("Could not open the specified TIFF image.");

#if defined DEBUG
  printf ("Inserting a TIFF image on page with object number %d.\n",
	  target->obj->number);
#endif

  // This dictionary item is TIFF specific
  adddictitem (imageObj->dict, "Filter", gTextValue, "CCITTFaxDecode");

  // Bits per component is per colour component, not per sample. Does this
  // matter?
  if (TIFFGetField (image, TIFFTAG_BITSPERSAMPLE, &tiffResponse16) != 0)
    adddictitem (imageObj->dict, "BitsPerComponent", gIntValue,
		 tiffResponse16);
  else
    error ("Could not get the colour depth for the tiff image.");

  // The colour device will change based on this number as well
  switch (tiffResponse16)
    {
    case 1:
      adddictitem (imageObj->dict, "ColorSpace", gTextValue, "DeviceGray");
      break;

    default:
      adddictitem (imageObj->dict, "ColorSpace", gTextValue, "DeviceRGB");
      break;
    }

  /****************************************************************************
     We need to add a sub-dictionary with the parameters for the compression
     filter in it.
  ****************************************************************************/

  // We make an object not just a dictionary because this is what
  // adddictitem needs
  subdict = newobject (output, gPlaceholder);

  // K will be minus one for g4 fax, and zero for g3 fax
  TIFFGetField (image, TIFFTAG_COMPRESSION, &compression);
  switch (compression)
    {
    case COMPRESSION_CCITTFAX3:
      adddictitem (subdict->dict, "K", gIntValue, 0);
      break;

    case COMPRESSION_CCITTFAX4:
      adddictitem (subdict->dict, "K", gIntValue, -1);
      break;

    case COMPRESSION_LZW:
      error ("LZW is encumbered with patents and therefore not supported.");
      break;

    default:
      error ("Unsupported TIFF compression algorithm.");
      break;
    }

  // Width of the image
  if (TIFFGetField (image, TIFFTAG_IMAGEWIDTH, &width) != 0)
    {
      adddictitem (subdict->dict, "Columns", gIntValue, width);
      adddictitem (imageObj->dict, "Width", gIntValue, width);
    }
  else
    error ("Could not get the width of the TIFF image.");

  // Height of the image
  if (TIFFGetField (image, TIFFTAG_IMAGELENGTH, &height) != 0)
    {
      adddictitem (subdict->dict, "Rows", gIntValue, height);
      adddictitem (imageObj->dict, "Height", gIntValue, height);
    }
  else
    error ("Could not get the height of the TIFF image.");

  // And put this into the PDF
  adddictitem (imageObj->dict, "DecodeParms", gDictionaryValue,
	       subdict->dict);

  // Fillorder determines whether we convert on the fly or not
  TIFFGetField (image, TIFFTAG_FILLORDER, &fillorder);

  if (fillorder == FILLORDER_LSB2MSB)
    {
    /*************************************************************************
      Convert the image
    *************************************************************************/

#if defined DEBUG
      printf ("Conversion of the image in-memory will occur.\n");
#endif

      // Because of the way this is implemented to integrate with the tiff lib
      // we need to ensure that we are the only thread that is performing this
      // operation at the moment. This is not a well coded piece of the library,
      // but I am at a loss as to how to do it better... We don't check if we
      // have already used global tiff buffer, because we are still using it's
      // old contents...

      pthread_mutex_lock (&tiffConvMutex);
      globalTiffBuffer = NULL;
      globalTiffBufferOffset = 0;

      // Open the dummy document (which actually only exists in memory)
      conv =
	TIFFClientOpen ("dummy", "w", (thandle_t) - 1, libtiffDummyReadProc,
			libtiffDummyWriteProc, libtiffDummySeekProc,
			libtiffDummyCloseProc, NULL, NULL, NULL);

      // Copy the image information ready for conversion
      stripSize = TIFFStripSize (image);
      stripMax = TIFFNumberOfStrips (image);
      imageOffset = 0;

      if ((stripBuffer =
	   malloc (TIFFNumberOfStrips (image) * stripSize)) == NULL)
	error ("Insufficient memory for TIFF image insertion.");

      for (stripCount = 0; stripCount < stripMax; stripCount++)
	{
#if defined DEBUG
	  printf ("Read a strip of the input image with offset %l\n",
		  imageOffset);
#endif

	  imageOffset += TIFFReadEncodedStrip (image, stripCount,
					       stripBuffer + imageOffset,
					       stripSize);
	}

      // We also need to copy some of the attributes of the tiff image
      // Bits per sample has to be 1 because this is going to be a G4/G3 image
      // (and all other image formats were stripped out above).
      // I need to check all of these sometime with many images...
      TIFFSetField (conv, TIFFTAG_IMAGEWIDTH, width);
      TIFFSetField (conv, TIFFTAG_IMAGELENGTH, height);
      TIFFSetField (conv, TIFFTAG_BITSPERSAMPLE, 1);
      TIFFSetField (conv, TIFFTAG_COMPRESSION, compression);
      TIFFSetField (conv, TIFFTAG_PLANARCONFIG, PLANARCONFIG_CONTIG);
      TIFFSetField (conv, TIFFTAG_ROWSPERSTRIP, height + 1);
      TIFFSetField (conv, TIFFTAG_PHOTOMETRIC, PHOTOMETRIC_MINISWHITE);
      TIFFSetField (conv, TIFFTAG_FILLORDER, FILLORDER_MSB2LSB);
      TIFFSetField (conv, TIFFTAG_SAMPLESPERPIXEL, 1);
      TIFFSetField (conv, TIFFTAG_ORIENTATION, ORIENTATION_TOPLEFT);
      TIFFSetField (conv, TIFFTAG_RESOLUTIONUNIT, RESUNIT_INCH);
      TIFFSetField (conv, TIFFTAG_XRESOLUTION, 300);
      TIFFSetField (conv, TIFFTAG_YRESOLUTION, 300);

      if (compression == COMPRESSION_CCITTFAX4)
	TIFFSetField (conv, TIFFTAG_GROUP4OPTIONS, 0);

#if defined DEBUG
      printf ("The image buffer is ld bytes long\n", imageOffset);
#endif

      // Actually do the conversion
      TIFFWriteEncodedStrip (conv, 0, stripBuffer, imageOffset);

      // Finish up
      free (stripBuffer);

#if defined DEBUG
      printf ("The global tiff buffer became %l bytes long\n",
	      globalTiffBufferOffset);
#endif

      imageObj->binarystream = globalTiffBuffer;
      imageObj->binarystreamLength = globalTiffBufferOffset;
      pthread_mutex_unlock (&tiffConvMutex);
    }

  else
    {
    /**************************************************************************
       Insert the image
    **************************************************************************/

#if defined DEBUG
      printf ("Image is not being converted internally.\n");
#endif

      // We also need to add a binary stream to the object and put the image
      // data into this stream
      stripSize = TIFFStripSize (image);
      imageOffset = 0;

      if ((imageObj->binarystream =
	   malloc (TIFFNumberOfStrips (image) * stripSize)) == NULL)
	error ("Insufficient memory for TIFF image insertion.");

      for (stripCount = 0; stripCount < TIFFNumberOfStrips (image);
	   stripCount++)
	{
	  imageOffset += TIFFReadRawStrip (image, stripCount,
					   imageObj->binarystream +
					   imageOffset, stripSize);
	}

      // We might have too much memory, truncate to the size we have actually 
      // read
      if ((tempstream = realloc (imageObj->binarystream, imageOffset)) !=
	  NULL)
	imageObj->binarystream = tempstream;

      // The image offset is the total size of the binary stream as well
      imageObj->binarystreamLength = imageOffset;
    }
}

// This function will insert a JPEG image into a PDF
void
insertJpeg (pdf * output, page * target, object * imageObj, char *filename)
{
  struct jpeg_decompress_struct cinfo;
  struct jpeg_error_mgr jerr;
  FILE *image;
  int c;
  unsigned long imageBufSize;

#if defined DEBUG
  printf ("Inserting a JPEG image on page with object number %d.\n",
	  target->obj->number);
#endif

  // Open the file
  if ((image = fopen (filename, "rb")) == NULL)
    error ("Could not open the specified JPEG file.");

  // Setup the decompression options
  cinfo.err = jpeg_std_error (&jerr);

  // Start decompressing
  jpeg_create_decompress (&cinfo);
  jpeg_stdio_src (&cinfo, image);
  jpeg_read_header (&cinfo, TRUE);

  // This dictionary item is JPEG specific
  adddictitem (imageObj->dict, "Filter", gTextValue, "DCTDecode");

  // Bits per component -- I'm not sure exactly how this works with libjpeg.
  // Is it possible to have a black and white jpeg?
  adddictitem (imageObj->dict, "BitsPerComponent", gIntValue,
	       cinfo.data_precision);

  // The colour device will change based on this number as well
  switch (cinfo.jpeg_color_space)
    {
    case JCS_GRAYSCALE:
      adddictitem (imageObj->dict, "ColorSpace", gTextValue, "DeviceGray");
      break;

    default:
      adddictitem (imageObj->dict, "ColorSpace", gTextValue, "DeviceRGB");
      break;
    }

  /****************************************************************************
     Some details of the image
  ****************************************************************************/

  adddictitem (imageObj->dict, "Width", gIntValue, cinfo.image_width);
  adddictitem (imageObj->dict, "Height", gIntValue, cinfo.image_height);

  /****************************************************************************
    Read the image into it's memory buffer
  ****************************************************************************/

  imageBufSize = 0;
  imageObj->binarystreamLength = 0;

  if ((image = fopen (filename, "r")) == NULL)
    error ("Could not open the JPEG file.");

  while ((c = fgetc (image)) != EOF)
    {
      // We need to grow the buffer
      if (imageBufSize == imageObj->binarystreamLength)
	{
	  imageBufSize += 1024;

	  if ((imageObj->binarystream = realloc (imageObj->binarystream,
						 imageBufSize)) == NULL)
	    error ("Could not make enough space for the JPEG image.");
	}

      // Store the info
      imageObj->binarystream[imageObj->binarystreamLength++] = c;
    }

  imageObj->binarystream[imageObj->binarystreamLength++] = 0;
  fclose (image);

  // This cleans things up for us in the JPEG library
  jpeg_destroy_decompress (&cinfo);
}

void
insertPNG (pdf * output, page * target, object * imageObj, char *filename)
{

#if defined DEBUG
  printf ("Inserting a PNG / Flate image on page with object number %d.\n",
	  target->obj->number);
#endif

}

/*****************************************************************************
  The following are dummy functions for libtiff that will allow us to do on-
  the-fly conversion of tiff images to the small subset of tiff that the
  PDF specification allows for...
*****************************************************************************/

static tsize_t
libtiffDummyReadProc (thandle_t fd, tdata_t buf, tsize_t size)
{
  // Return the amount of data read, which we will always set as 0 because
  // we only need to be able to write to these in-memory tiffs
  return 0;
}

/* Yes, I know global variables are bad, and this will be changed in the near
   future. If you are reading this code and are disgusted, then feel free to
   send me a patch at mikal@stillhq.com */

static tsize_t
libtiffDummyWriteProc (thandle_t fd, tdata_t buf, tsize_t size)
{

#if defined DEBUG
  printf ("TIFF dummy write procedure called\n");
#endif

  // libtiff will try to write an 8 byte header into the tiff file. We need
  // to ignore this because PDF does not use it...
  if ((size == 8) && (((char *) buf)[0] == 'I') && (((char *) buf)[1] == 'I')
      && (((char *) buf)[2] == 42))
    {
      // Skip the header -- little endian
    }
  else if ((size == 8) && (((char *) buf)[0] == 'M') &&
	   (((char *) buf)[1] == 'M') && (((char *) buf)[2] == 42))
    {
      // Skip the header -- big endian
    }
  else
    {
      // Have we done anything yet?
      if (globalTiffBuffer == NULL)
	{
	  if ((globalTiffBuffer = (char *) malloc (size * sizeof (char))) ==
	      NULL)
	    error ("Could not start tiff conversion memory buffer.");
	}

      // Otherwise, we need to grow the memory buffer
      else
	{
	  if ((globalTiffBuffer = (char *) realloc (globalTiffBuffer,
						    (size * sizeof (char)) +
						    globalTiffBufferOffset))
	      == NULL)
	    error ("Could not grow the tiff conversion memory buffer.");
	}

      // Now move the image data into the buffer
      memcpy (globalTiffBuffer + globalTiffBufferOffset, buf, size);
      globalTiffBufferOffset += size;
    }

  return (size);
}

static toff_t
libtiffDummySeekProc (thandle_t fd, toff_t off, int i)
{
  // This appears to return the location that it went to
  return off;
}

static int
libtiffDummyCloseProc (thandle_t fd)
{
  // Return a zero meaning all is well
  return 0;
}
