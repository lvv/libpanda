/******************************************************************************
  images.c

  Change Control:                                                      DDMMYYYY
    Michael Still    File created                                      29072000

  Purpose:
    Image insertion is all handled by one fuction, which then goes off and
    calls the functions that know about particular image types...
******************************************************************************/

#include <panda/constants.h>
#include <panda/functions.h>
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

// Imagebox now just calls panda_imageboxrot with a default rotational value
// Based on patches submitted by Ceasar Miquel (miquel@df.uba.ar)
void
panda_imagebox (panda_pdf * output, panda_page * target, int top, int left,
	  int bottom, int right, char *filename, int type)
{
  return panda_imageboxrot(output, target, top, left, bottom, right, 0.0, 
		     filename, type);
}

// A redistribution point for image insertions based on type of image
void
panda_imageboxrot (panda_pdf * output, panda_page * target, int top, int left,
	  int bottom, int right, double angle, char *filename, int type)
{
  panda_object *imageObj, *xobjrefsubdict, *xobjrefsubsubdict;
  char   *panda_pdfFilename;
  int    i;

#if defined DEBUG
  printf ("Started inserting an image.\n");
#endif

  // Now we need an panda_object to contain the tiff
  imageObj = panda_newpanda_object (output, gNormal);
  panda_addpanda_child (target->obj, imageObj);

  // We cannot have some characters in the filename that we embed into the PDF,
  // so we fix them here
  panda_pdfFilename = (char *) panda_xmalloc((strlen(filename) + 1) * sizeof(char));
  strcpy(panda_pdfFilename, filename);

  for(i = 0; i < strlen(panda_pdfFilename) + 1; i++)
    if(panda_pdfFilename[i] == '/') panda_pdfFilename[i] = '-';

#if defined DEBUG
  printf("Filename for PDF was \"%s\" and is now \"%s\"\n",
	 filename, panda_pdfFilename);
#endif

  // We make an panda_object not just a panda_dictionary because this is what
  // panda_adddictitem needs
  xobjrefsubsubdict = panda_newpanda_object (output, gPlaceholder);
  panda_adddictitem (xobjrefsubsubdict->dict, panda_pdfFilename, gObjValue, imageObj);

  xobjrefsubdict = panda_newpanda_object (output, gPlaceholder);
  panda_adddictitem (xobjrefsubdict->dict, "XObject", gDictionaryValue,
	       xobjrefsubsubdict->dict);

  // And put this into the PDF
  panda_adddictitem (target->obj->dict, "Resources", gDictionaryValue,
	       xobjrefsubdict->dict);

  // We put some information based on a stat of the image file into the panda_object
  // This will allow us to determine if this file's image is included in the
  // document again later, which will allow us avoid repetition of the same
  // image data for no apparent reason -- note that this is not complete yet
  // and we still need to add the existance check before the panda_object is created
  // and some form of panda_object searching by a rational and efficient manner (a
  // binary search tree?)

  // We now add some panda_dictionary elements to the image panda_object to say that it is
  // a TIFF image
  panda_adddictitem (imageObj->dict, "Type", gTextValue, "XObject");
  panda_adddictitem (imageObj->dict, "Subtype", gTextValue, "Image");

  // This line will need to be changed to gaurantee that the internal name is
  // unique unless the actual image is the same
  panda_adddictitem (imageObj->dict, "Name", gTextValue, panda_pdfFilename);

  // Now we do the things that are image format specific... This is also
  // where we check if support has been compiled in for the libraries we need.
  switch (type)
    {
    case gImageTiff:
      if(HAVE_LIBTIFF)
	panda_insertTIFF (output, target, imageObj, filename);
      else
	{
	  fprintf(stderr, "%s %s\n",
		  "TIFF support not compiled into Panda because libtiff was",
		  "not found at compile time.");
	  panda_adddictitem(imageObj->dict, "TIFF_Support_Missing", gIntValue, 1);
	}
      break;

    case gImageJpeg:
      if(HAVE_LIBJPEG)
	panda_insertJPEG (output, target, imageObj, filename);
      else
	{
	  fprintf(stderr, "%s %s\n",
		  "JPEG support not compiled into Panda because libjpeg was",
		  "not found at compile time.");
	  panda_adddictitem(imageObj->dict, "JPEG_Support_Missing", gIntValue, 1);
	}
      break;

    case gImagePNG:
      if(HAVE_LIBPNG)
	panda_insertPNG (output, target, imageObj, filename);
      else
	{
	  fprintf(stderr, "%s %s\n",
		  "PNG support not compiled into Panda because libpng was not",
		  "found at compile time.");
	  panda_adddictitem(imageObj->dict, "PNG_Support_Missing", gIntValue, 1);
	}
      break;
    }
  // --------------------------------------------------------------------------

  // We also need to add some information to the layout stream for the contents
  // panda_object for the panda_page that the image is being displayed on. This information
  // consists of the following:
  //  - save the current graphics state (q operator, p 386 of spec)
  //  - setup the current transformation matrix (ctm, s 3.2 and p 323 of spec)
  //    such that the image is scaled correctly (cm operator)
  //  - modify the ctm to shift the image to where it is meant to be on the
  //    the panda_page
  //  - use the image xpanda_object we have created (Do operator, p 348 of spec)
  //  - restore the graphics state to the way it was previously (Q operator,
  //    p 386 of spec)
  if(target->contents->textmode == gTrue)
    {
      target->contents->layoutstream = 
	panda_streamprintf(target->contents->layoutstream, "ET\n");
      target->contents->textmode = gFalse;

#if defined DEBUG
      printf("Imageboxrot: Ended textmode for panda_object %d\n", target->contents->number);
#endif
    }

  target->contents->layoutstream =
    panda_streamprintf (target->contents->layoutstream,
		  "\nq\n%.2f %.2f %.2f %.2f %.2f %.2f cm\n",
		  // The first matrix -- this has been modified because of
		  // patches submitted by Ceasar Miquel (miquel@df.uba.ar)
		  cos(angle * M_PI / 180.0), // x scale
		  sin(angle * M_PI / 180.0), // rotate and scale
		  -sin(angle * M_PI / 180.0), // ???
		  cos(angle * M_PI / 180.0), // y scale
		  (double) left,	// x start
		  (double) target->height - bottom);	// y start

  target->contents->layoutstream =
    panda_streamprintf (target->contents->layoutstream,
		  "%.2f %.2f %.2f %.2f %.2f %.2f cm\n",
		  // The second matrix
		  (double) (right - left),	// x size
		  0.0,		// ???
		  0.0,		// ???
		  (double) (bottom - top),	// y size
		  0.0,		// ???
		  0.0);		// ???

  target->contents->layoutstream =
    panda_streamprintf (target->contents->layoutstream, "/%s Do\nQ\n\n", 
		  panda_pdfFilename);

#if defined DEBUG
  printf ("Finished inserting an image.\n");
#endif
}

// This function will insert a TIFF image into a PDF
void
panda_insertTIFF (panda_pdf * output, panda_page * target, panda_object * imageObj, char *filename)
{
  /**************************************************************************
    Some notes about TIFF support inside PDF files.

     - MSB2LSB is the only byte fillorder that is supported.
     - The images must be converted to single strip
     - G3 and G4 are supported
  **************************************************************************/

  TIFF *image, *conv;
  panda_object *subdict;
  int stripCount, stripMax;
  tsize_t stripSize;
  unsigned long imageOffset;
  char *tempstream, *stripBuffer, errMessage[1024];
  uint16 tiffResponse16, compression, fillorder;
  uint32 height, width;

  // Open the file and make sure that it exists and is a TIFF file
  if ((image = TIFFOpen (filename, "r")) == NULL){
    snprintf(errMessage, 1024, 
	     "Could not open the specified TIFF image \"%s\".", filename);
    panda_error(errMessage);
  }

#if defined DEBUG
  printf ("Inserting a TIFF image on panda_page with panda_object number %d.\n",
	  target->obj->number);
#endif

  // This panda_dictionary item is TIFF specific
  panda_adddictitem (imageObj->dict, "Filter", gTextValue, "CCITTFaxDecode");

  // Bits per component is per colour component, not per sample. Does this
  // matter?
  if (TIFFGetField (image, TIFFTAG_BITSPERSAMPLE, &tiffResponse16) != 0)
    panda_adddictitem (imageObj->dict, "BitsPerComponent", gIntValue,
		 tiffResponse16);
  else
    panda_error ("Could not get the colour depth for the tiff image.");

  // The colour device will change based on this number as well
  switch (tiffResponse16)
    {
    case 1:
      panda_adddictitem (imageObj->dict, "ColorSpace", gTextValue, "DeviceGray");
      break;

    default:
      panda_adddictitem (imageObj->dict, "ColorSpace", gTextValue, "DeviceRGB");
      break;
    }

  /****************************************************************************
     We need to add a sub-panda_dictionary with the parameters for the compression
     filter in it.
  ****************************************************************************/

  // We make an panda_object not just a panda_dictionary because this is what
  // panda_adddictitem needs
  subdict = panda_newpanda_object (output, gPlaceholder);

  // K will be minus one for g4 fax, and zero for g3 fax
  TIFFGetField (image, TIFFTAG_COMPRESSION, &compression);
  switch (compression)
    {
    case COMPRESSION_CCITTFAX3:
      panda_adddictitem (subdict->dict, "K", gIntValue, 0);
      break;

    case COMPRESSION_CCITTFAX4:
      panda_adddictitem (subdict->dict, "K", gIntValue, -1);
      break;

    case COMPRESSION_LZW:
      panda_error ("LZW is encumbered with patents and therefore not supported.");
      break;

    default:
      panda_error ("Unsupported TIFF compression algorithm.");
      break;
    }

  // Width of the image
  if (TIFFGetField (image, TIFFTAG_IMAGEWIDTH, &width) != 0)
    {
      panda_adddictitem (subdict->dict, "Columns", gIntValue, width);
      panda_adddictitem (imageObj->dict, "Width", gIntValue, width);
    }
  else
    panda_error ("Could not get the width of the TIFF image.");

  // Height of the image
  if (TIFFGetField (image, TIFFTAG_IMAGELENGTH, &height) != 0)
    {
      panda_adddictitem (subdict->dict, "Rows", gIntValue, height);
      panda_adddictitem (imageObj->dict, "Height", gIntValue, height);
    }
  else
    panda_error ("Could not get the height of the TIFF image.");

  // And put this into the PDF
  panda_adddictitem (imageObj->dict, "DecodeParms", gDictionaryValue,
	       subdict->dict);

  // Fillorder determines whether we convert on the fly or not, although
  // multistrip images also need to be converted
  TIFFGetField (image, TIFFTAG_FILLORDER, &fillorder);

  if ((fillorder == FILLORDER_LSB2MSB) || (TIFFNumberOfStrips(image) > 1))
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

      stripBuffer = panda_xmalloc(TIFFNumberOfStrips (image) * stripSize);

      for (stripCount = 0; stripCount < stripMax; stripCount++)
	{
#if defined DEBUG
	  printf ("Read a strip of the input image with offset %ld\n",
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
      printf ("The image buffer is %ld bytes long\n", imageOffset);
#endif

      // Actually do the conversion
      TIFFWriteEncodedStrip (conv, 0, stripBuffer, imageOffset);

      // Finish up
      free (stripBuffer);

#if defined DEBUG
      printf ("The global tiff buffer became %ld bytes long\n",
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

      // We also need to add a binary stream to the panda_object and put the image
      // data into this stream
      stripSize = TIFFStripSize (image);
      imageOffset = 0;

      imageObj->binarystream = panda_xmalloc(TIFFNumberOfStrips (image) * stripSize);

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
panda_insertJPEG (panda_pdf * output, panda_page * target, panda_object * imageObj, char *filename)
{
  struct jpeg_decompress_struct cinfo;
  struct jpeg_error_mgr jerr;
  FILE *image;
  int c;
  unsigned long imageBufSize;

#if defined DEBUG
  printf ("Inserting a JPEG image on panda_page with panda_object number %d.\n",
	  target->obj->number);
#endif

  // Open the file
  if ((image = fopen (filename, "rb")) == NULL)
    panda_error ("Could not open the specified JPEG file.");

  // Setup the decompression options
  cinfo.err = jpeg_std_error (&jerr);

  // Start decompressing
  jpeg_create_decompress (&cinfo);
  jpeg_stdio_src (&cinfo, image);
  jpeg_read_header (&cinfo, TRUE);

  // This panda_dictionary item is JPEG specific
  panda_adddictitem (imageObj->dict, "Filter", gTextValue, "DCTDecode");

  // Bits per component -- I'm not sure exactly how this works with libjpeg.
  // Is it possible to have a black and white jpeg? Ceasar Miquel
  // (miquel@df.uba.ar) has submitted patches suggesting that this should
  // always be 8, but this seems to work so I will leave it like this for now
  panda_adddictitem (imageObj->dict, "BitsPerComponent", gIntValue,
	       cinfo.data_precision);

  // The colour device will change based on this number as well
  switch (cinfo.jpeg_color_space)
    {
    case JCS_GRAYSCALE:
      panda_adddictitem (imageObj->dict, "ColorSpace", gTextValue, "DeviceGray");
      break;

    default:
      panda_adddictitem (imageObj->dict, "ColorSpace", gTextValue, "DeviceRGB");
      break;
    }

  /****************************************************************************
     Some details of the image
  ****************************************************************************/

  panda_adddictitem (imageObj->dict, "Width", gIntValue, cinfo.image_width);
  panda_adddictitem (imageObj->dict, "Height", gIntValue, cinfo.image_height);

  /****************************************************************************
    Read the image into it's memory buffer
  ****************************************************************************/

  imageBufSize = 0;
  imageObj->binarystreamLength = 0;

  if ((image = fopen (filename, "r")) == NULL)
    panda_error ("Could not open the JPEG file.");

  while ((c = fgetc (image)) != EOF)
    {
      // We need to grow the buffer
      if (imageBufSize == imageObj->binarystreamLength)
	{
	  imageBufSize += 1024;

	  if ((imageObj->binarystream = realloc (imageObj->binarystream,
						 imageBufSize)) == NULL)
	    panda_error ("Could not make enough space for the JPEG image.");
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
panda_insertPNG (panda_pdf * output, panda_page * target, panda_object * imageObj, char *filename)
{

#if defined DEBUG
  printf ("Inserting a PNG / Flate image on panda_page with panda_object number %d.\n",
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
	globalTiffBuffer = (char *) panda_xmalloc(size * sizeof (char));

      // Otherwise, we need to grow the memory buffer
      else
	{
	  if ((globalTiffBuffer = (char *) realloc (globalTiffBuffer,
						    (size * sizeof (char)) +
						    globalTiffBufferOffset))
	      == NULL)
	    panda_error ("Could not grow the tiff conversion memory buffer.");
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
