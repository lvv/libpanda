/******************************************************************************
  images.c

  Change Control:                                                      DDMMYYYY
    Michael Still    File created                                      29072000

  Purpose:
    Image insertion is all handled by one fuction, which then goes off and
    calls the functions that know about particular image types...

  Copyright (C) Michael Still 2000 - 2002
  
  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.
  
  You should have received a copy of the GNU General Public License
  along with this program; if not, write to the Free Software
  Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
******************************************************************************/

#if defined _WINDOWS
#include "stdafx.h"

#include "contrib/libtiff/tiffio.h"
#include "contrib/libjpeg/jpeglib.h"
#include "contrib/libpng/png.h"

#include <windows.h>

HANDLE winmutex;
#else
#include <panda/constants.h>
#include <panda/functions.h>

#if defined HAVE_LIBTIFF
#include <tiffio.h>
#endif

#if defined HAVE_LIBJPEG
#include <jpeglib.h>
#endif

#if defined HAVE_LIBPNG
#include <png.h>
#endif

#include <unistd.h>
#include <pthread.h>

pthread_mutex_t convMutex = PTHREAD_MUTEX_INITIALIZER;
#endif

#include <math.h>
#include <sys/stat.h>

static tsize_t libtiffDummyReadProc (thandle_t fd, tdata_t buf, tsize_t size);
static tsize_t libtiffDummyWriteProc (thandle_t fd, tdata_t buf,
				      tsize_t size);
static toff_t libtiffDummySeekProc (thandle_t fd, toff_t off, int i);
static int libtiffDummyCloseProc (thandle_t fd);

void libpngDummyWriteProc (png_structp png, png_bytep data, png_uint_32 len);
void libpngDummyFlushProc (png_structp png);

char *globalImageBuffer;
unsigned long globalImageBufferOffset;
char globalIsIDAT;

/******************************************************************************
DOCBOOK START

FUNCTION panda_imagebox
PURPOSE insert an image into the PDF document at the specified location

SYNOPSIS START
#include&lt;panda/constants.h&gt;
#include&lt;panda/functions.h&gt;
void panda_panda_imagebox (panda_pdf * output, panda_page * target, int top, int left, int bottom, int right, char *filename, int type);
SYNOPSIS END

DESCRIPTION This function call inserts an image into the PDF document at the specified location using a reasonable default for rotation (none). This call is included for backward compatability withprevious releases of the API and it is recommened that new code call <command>panda_imageboxrot</command>(). It is unlikely that this call will be retired however. The image types accepted by this call are: panda_image_tiff, panda_image_jpeg and panda_image_png.

RETURNS Nothing

EXAMPLE START
#include&lt;panda/constants.h&gt;
#include&lt;panda/functions.h&gt;

panda_pdf *demo;
panda_page *currPage;

panda_init ();

if ((demo = panda_open ("output.pdf", "w")) == NULL)
  panda_error (panda_true, "demo: could not open output.pdf to write to.");

currPage = panda_newpage (demo, panda_pagesize_a4);
  
panda_imagebox (demo, currPage, 0, 0, currPage->height / 2,
  currPage->width, "input.tif", panda_image_tiff);
EXAMPLE END
SEEALSO panda_imageboxrot
DOCBOOK END
******************************************************************************/

// Imagebox now just calls panda_imageboxrot with a default rotational value
// Based on patches submitted by Ceasar Miquel (miquel@df.uba.ar)
void
panda_imagebox (panda_pdf * output, panda_page * target, int top, int left,
		int bottom, int right, char *filename, int type)
{
  panda_imageboxrot (output, target, top, left, bottom, right, 0.0,
		     filename, type);
}

/******************************************************************************
DOCBOOK START

FUNCTION panda_imageboxrot
PURPOSE insert an image into the PDF document at the specified location

SYNOPSIS START
#include&lt;panda/constants.h&gt;
#include&lt;panda/functions.h&gt;
void panda_panda_imageboxrot (panda_pdf * output, panda_page * target, int top, int left, int bottom, int right, double angle, char *filename, int type);
SYNOPSIS END

DESCRIPTION This function call inserts an image into the PDF document at the specified location, including the ability to rotate the image on the page. It should be noted that xpdf will sometimes make the rotated image look quite sickly. This is in fact a bug in xpdf (which has beenr eported), and not a bug in <command>Panda</command>. The image types accepted by this call are: panda_image_tiff, panda_image_jpeg and panda_image_png.
</para>
<para>
<emphasis>Note that this function now holds your hand and will save you from inerting an image with the same name over and over. Instead of regrabbing the image, it will just put a pointer to it inside the PDF itself. If you really want to use the same filename over and over with different images inside it, then use the</emphasis> <command>panda_imageboxactual</command>() call.

RETURNS Nothing

EXAMPLE START
#include&lt;panda/constants.h&gt;
#include&lt;panda/functions.h&gt;

panda_pdf *demo;
panda_page *currPage;

panda_init ();

if ((demo = panda_open ("output.pdf", "w")) == NULL)
  panda_error (panda_true, "demo: could not open output.pdf to write to.");

currPage = panda_newpage (demo, panda_pagesize_a4);
  
panda_imagebox (demo, currPage, 0, 0, currPage->height / 2,
  currPage->width, 45.0, "input.tif", panda_image_tiff);
EXAMPLE END
SEEALSO panda_imagebox
DOCBOOK END
******************************************************************************/

// Check to see if we already have the image
void
panda_imageboxrot (panda_pdf * output, panda_page * target, int top, int left,
		   int bottom, int right, double angle, char *filename,
		   int type)
{
  char *dbkey, *dbdata, *imageref;
  int count;

#if defined DEBUG
  printf ("Check to see if we have used this image before (%s)\n", filename);
#endif

  count = 0;
  dbkey = panda_xsnprintf ("image-%d-name", count);
  while ((dbdata = panda_dbread (output, dbkey)) != NULL)
    {
      panda_xfree (dbkey);

      if (strcmp (dbdata, filename) == 0)
	{
#if defined DEBUG
	  printf
	    ("Found that this is a repeat ref to an image, and recycled\n");
#endif

	  dbkey = panda_xsnprintf ("image-%d-objectreference", count);
	  imageref = panda_dbread (output, dbkey);
	  panda_imageboxinternal (output, target, top, left, bottom, right,
				  angle, filename, type, panda_false,
				  imageref, -1);
	  panda_xfree (imageref);
	  panda_xfree (dbkey);
	  panda_xfree (dbdata);
	  return;
	}

      panda_xfree (dbdata);
      dbkey = panda_xsnprintf ("image-%d-name", ++count);
    }

  panda_imageboxinternal (output, target, top, left, bottom, right, angle,
			  filename, type, panda_true, NULL, count);

  panda_dbwrite (output, dbkey, filename);
  panda_xfree (dbkey);
}

// todo_mikal: doco
// People might want to force me to add an image
void
panda_imageboxactual (panda_pdf * output, panda_page * target, int top,
		      int left, int bottom, int right, double angle,
		      char *filename, int type)
{
  panda_imageboxinternal (output, target, top, left, bottom, right, angle,
			  filename, type, panda_true, NULL, -1);
}

// todo_mikal doco
// Redistribute the image types
void
panda_imageboxinternal (panda_pdf * output, panda_page * target, int top,
			int left, int bottom, int right, double angle,
			char *filename, int type, int addImage,
			char *objref, int databasecount)
{
  panda_object *imageObj;
  char *pdfFilename, *dictkey, *dbkey, *dbdata;
  int i;

#if defined DEBUG
  printf ("Started inserting an image (%d, %s, %d)\n", addImage, objref,
	  databasecount);
#endif

  if (addImage == panda_true)
    {
      // Now we need an object to contain the image
      imageObj = (panda_object *) panda_newobject (output, panda_normal);
      panda_addchild (target->obj, imageObj);

      if (databasecount != -1)
	{
	  dbkey = panda_xsnprintf ("image-%d-objectreference", databasecount);
	  dbdata = panda_xsnprintf ("%d %d R",
				    imageObj->number, imageObj->generation);
	  panda_dbwrite (output, dbkey, dbdata);
	  panda_xfree (dbkey);
	  panda_xfree (dbdata);
	}
    }
  else if (objref == NULL)
    panda_error (panda_true, "Invalid image processing state\n");
#if defined DEBUG
  else
    printf ("Code is now recycling image... Good for the environment\n");
#endif

  // We cannot have some characters in the filename that we embed into the PDF,
  // so we fix them here
  pdfFilename =
    (char *) panda_xmalloc ((strlen (filename) + 1) * sizeof (char));
  strcpy (pdfFilename, filename);

  for (i = 0; i < strlen (pdfFilename) + 1; i++)
    if (pdfFilename[i] == '/')
      pdfFilename[i] = '-';

#if defined DEBUG
  printf ("Filename for PDF was \"%s\" and is now \"%s\"\n",
	  filename, pdfFilename);
#endif

  dictkey = panda_xsnprintf ("Resources/XObject/%s", pdfFilename);
  if (objref == NULL)
    panda_adddictitem (output, target->obj, dictkey, panda_objectvalue,
		       imageObj);
  else
    panda_adddictitem (output, target->obj, dictkey, panda_literaltextvalue,
		       objref);
  panda_xfree (dictkey);

  // We put some information based on a stat of the image file into the object
  // This will allow us to determine if this file's image is included in the
  // document again later, which will allow us avoid repetition of the same
  // image data for no apparent reason -- note that this is not complete yet
  // and we still need to add the existance check before the object is created
  // and some form of object searching by a rational and efficient manner (a
  // binary search tree?)

  if (addImage == panda_true)
    {
      // We now add some dictionary elements to the image object to say that 
      // it is a TIFF image
      panda_adddictitem (output, imageObj, "Type", panda_textvalue,
			 "XObject");
      panda_adddictitem (output, imageObj, "Subtype", panda_textvalue,
			 "Image");

      // This line will need to be changed to gaurantee that the internal 
      // name is unique unless the actual image is the same
      panda_adddictitem (output, imageObj, "Name", panda_textvalue,
			 pdfFilename);
    }

  // Now we do the things that are image format specific... This is also
  // where we check if support has been compiled in for the libraries we need.
  switch (type)
    {
    case panda_image_tiff:
#if defined HAVE_LIBTIFF
      if (addImage == panda_true)
	panda_insertTIFF (output, target, imageObj, filename);
#else
      fprintf (stderr, "%s %s\n",
	       "TIFF support not compiled into Panda because libtiff was",
	       "not found at compile time.");
      panda_adddictitem (output, imageObj, "TIFF_Support_Missing",
			 panda_integervalue, 1);
#endif
      break;

    case panda_image_jpeg:
#if defined HAVE_LIBJPEG
      if (addImage == panda_true)
	panda_insertJPEG (output, target, imageObj, filename);
#else
      fprintf (stderr, "%s %s\n",
	       "JPEG support not compiled into Panda because libjpeg was",
	       "not found at compile time.");
      panda_adddictitem (output, imageObj, "JPEG_Support_Missing",
			 panda_integervalue, 1);
#endif
      break;

    case panda_image_png:
#if defined HAVE_LIBPNG
      if (addImage == panda_true)
	panda_insertPNG (output, target, imageObj, filename);
#else
      fprintf (stderr, "%s %s\n",
	       "PNG support not compiled into Panda because libpng was not",
	       "found at compile time.");
      panda_adddictitem (output, imageObj, "PNG_Support_Missing",
			 panda_integervalue, 1);
#endif
      break;
    }
  // --------------------------------------------------------------------------

  // We also need to add some information to the layout stream for the contents
  // object for the page that the image is being displayed on. This information
  // consists of the following [moved to internal.c]
  panda_entergraphicsmode (target);

  target->contents->layoutstream =
    panda_streamprintf (target->contents->layoutstream,
			"\n%.2f %.2f %.2f %.2f %.2f %.2f cm\n",
			// The first matrix -- this has been modified 
			// because of patches submitted by Ceasar Miquel 
			// (miquel@df.uba.ar)
			cos (angle * panda_pi / 180.0),	// x scale
			sin (angle * panda_pi / 180.0),	// rotate and scale
			-sin (angle * panda_pi / 180.0),	// ???
			cos (angle * panda_pi / 180.0),	// y scale
			(double) left,	// x start
			(double) target->height - bottom);	// y start

  target->contents->layoutstream =
    panda_streamprintf (target->contents->layoutstream,
			"%.2f %.2f %.2f %.2f %.2f %.2f cm\n",
			// The second matrix
			(double) (right - left),	// x size
			0.0,	// ???
			0.0,	// ???
			(double) (bottom - top),	// y size
			0.0,	// ???
			0.0);	// ???

  target->contents->layoutstream =
    panda_streamprintf (target->contents->layoutstream, "/%s Do\n",
			pdfFilename);

  free (pdfFilename);
  panda_exitgraphicsmode (target);

#if defined DEBUG
  printf ("Finished inserting an image.\n");
#endif
}

/******************************************************************************
DOCBOOK START

FUNCTION panda_insertTIFF
PURPOSE insert a TIFF image into the PDF

SYNOPSIS START
#include&lt;panda/constants.h&gt;
#include&lt;panda/functions.h&gt;
void panda_insertTIFF (panda_pdf * output, panda_page * target, panda_object * imageObj, char *filename);
SYNOPSIS END

DESCRIPTION <command>PANDA INTERNAL</command>. Do the actual insertion of the TIFF image into the PDF document. This routine handles the conversion of the TIFF image into the form supported by the PDF specification, and similar operations. It is an internal <command>Panda</command> call and should not be needed by users of the API.

RETURNS Nothing

EXAMPLE START
See panda_imageboxrot for an example usage
EXAMPLE END
SEEALSO panda_imagebox, panda_imageboxrot, panda_insertJPEG, panda_insertPNG
DOCBOOK END
******************************************************************************/

// This function will insert a TIFF image into a PDF
void
panda_insertTIFF (panda_pdf * output, panda_page * target,
		  panda_object * imageObj, char *filename)
{
  /**************************************************************************
    Some notes about TIFF support inside PDF files.

     - MSB2LSB is the only byte fillorder that is supported.
     - The images must be converted to single strip
     - G3 and G4 are supported
  **************************************************************************/

  TIFF *image, *conv;
  int stripCount, stripMax;
  tsize_t stripSize;
  unsigned long imageOffset;
  char *tempstream, *stripBuffer, *errMessage;
  uint16 tiffResponse16, compression, fillorder;
  int height, width;
  uint32 theight, twidth;

  // Open the file and make sure that it exists and is a TIFF file
  if ((image = TIFFOpen (filename, "r")) == NULL)
    {
      errMessage =
	panda_xsnprintf ("Could not open the specified TIFF image \"%s\".",
			 filename);
      panda_error (panda_true, errMessage);
    }

#if defined DEBUG
  printf ("Inserting a TIFF image on page with object number %d.\n",
	  target->obj->number);
#endif

  // Bits per component is per colour component, not per sample. Does this
  // matter?
  if (TIFFGetField (image, TIFFTAG_BITSPERSAMPLE, &tiffResponse16) != 0)
    panda_adddictitem (output, imageObj, "BitsPerComponent",
		       panda_integervalue, tiffResponse16);
  else
    panda_error (panda_true,
		 "Could not get the colour depth for the tiff image.");

  // The colour device will change based on the number of samples per pixel
  if (TIFFGetField (image, TIFFTAG_SAMPLESPERPIXEL, &tiffResponse16) == 0)
    panda_error
      (panda_true,
       "Could not get number of samples per pixel for a tiff image.");

  switch (tiffResponse16)
    {
    case 1:
      panda_adddictitem (output, imageObj, "ColorSpace", panda_textvalue,
			 "DeviceGray");
      break;

    default:
      panda_adddictitem (output, imageObj, "ColorSpace", panda_textvalue,
			 "DeviceRGB");
      break;
    }

  /****************************************************************************
     We need to add a sub-dictionary with the parameters for the compression
     filter in it.
  ****************************************************************************/

  // K will be minus one for g4 fax, and zero for g3 fax
  TIFFGetField (image, TIFFTAG_COMPRESSION, &compression);
  switch (compression)
    {
    case COMPRESSION_CCITTFAX3:
      panda_adddictitem (output, imageObj, "Filter", panda_textvalue,
			 "CCITTFaxDecode");
      panda_adddictitem (output, imageObj, "DecodeParms/K",
			 panda_integervalue, 0);
      break;

    case COMPRESSION_CCITTFAX4:
      panda_adddictitem (output, imageObj, "Filter", panda_textvalue,
			 "CCITTFaxDecode");
      panda_adddictitem (output, imageObj, "DecodeParms/K",
			 panda_integervalue, -1);
      break;

    case COMPRESSION_NONE:
      // We put nothing here because it is not compressed
      break;

    case COMPRESSION_LZW:
      panda_error (panda_true,
		   "LZW is encumbered with patents and therefore not supported.");
      break;

    default:
      panda_error (panda_true, "Unsupported TIFF compression algorithm.");
      break;
    }
  
    // Get the width and height of the image
    panda_imagesizeTIFF (&width, &height, filename);
  
    // Width of the image
    panda_adddictitem (output, imageObj, "DecodeParms/Columns",
		       panda_integervalue, width);
  panda_adddictitem (output, imageObj, "Width", panda_integervalue, width);

  // Height of the image
  panda_adddictitem (output, imageObj, "DecodeParms/Rows",
		     panda_integervalue, height);
  panda_adddictitem (output, imageObj, "Height", panda_integervalue, height);

  // Fillorder determines whether we convert on the fly or not, although
  // multistrip images also need to be converted
  TIFFGetField (image, TIFFTAG_FILLORDER, &fillorder);

  if ((fillorder == FILLORDER_LSB2MSB) || (TIFFNumberOfStrips (image) > 1))
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
#if defined _WINDOWS
      winmutex = CreateMutex (NULL, TRUE, "Panda");
      WaitForSingleObject (winmutex, INFINITE);
#else
      pthread_mutex_lock (&convMutex);
#endif

      globalImageBuffer = NULL;
      globalImageBufferOffset = 0;

      // Open the dummy document (which actually only exists in memory)
      conv =
	TIFFClientOpen ("dummy", "w", (thandle_t) - 1, libtiffDummyReadProc,
			libtiffDummyWriteProc, libtiffDummySeekProc,
			libtiffDummyCloseProc, NULL, NULL, NULL);

      // Copy the image information ready for conversion
      stripSize = TIFFStripSize (image);
      stripMax = TIFFNumberOfStrips (image);
      imageOffset = 0;

      stripBuffer = panda_xmalloc (TIFFNumberOfStrips (image) * stripSize);

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
      twidth = width;
      theight = height;
      TIFFSetField (conv, TIFFTAG_IMAGEWIDTH, twidth);
      TIFFSetField (conv, TIFFTAG_IMAGELENGTH, theight);
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
	      globalImageBufferOffset);
#endif

      imageObj->binarystream = globalImageBuffer;
      imageObj->binarystreamLength = globalImageBufferOffset;

#if defined _WINDOWS
      ReleaseMutex (winmutex);
#else
      pthread_mutex_unlock (&convMutex);
#endif
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

      imageObj->binarystream =
	panda_xmalloc (TIFFNumberOfStrips (image) * stripSize);

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

  TIFFClose (image);
}

/******************************************************************************
DOCBOOK START

FUNCTION panda_insertJPEG
PURPOSE insert a JPEG image into the PDF

SYNOPSIS START
#include&lt;panda/constants.h&gt;
#include&lt;panda/functions.h&gt;
void panda_insertJPEG (panda_pdf * output, panda_page * target, panda_object * imageObj, char *filename);
SYNOPSIS END

DESCRIPTION <command>PANDA INTERNAL</command>. Do the actual insertion of the JPEG image into the PDF document. This routine handles the conversion of the JPEG image into the form supported by the PDF specification, and similar operations. It is an internal <command>Panda</command> call and should not be needed by users of the API.

RETURNS Nothing

EXAMPLE START
See panda_imageboxrot for an example usage
EXAMPLE END
SEEALSO panda_imagebox, panda_imageboxrot, panda_insertTIFF, panda_insertPNG
DOCBOOK END
******************************************************************************/

// This function will insert a JPEG image into a PDF
void
panda_insertJPEG (panda_pdf * output, panda_page * target,
		  panda_object * imageObj, char *filename)
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

  // Open the file -- why is this a memory leak?
  if ((image = fopen (filename, "rb")) == NULL)
    panda_error (panda_true, "Could not open the specified JPEG file.");

  // Setup the decompression options
  cinfo.err = jpeg_std_error (&jerr);

  // Start decompressing
  jpeg_create_decompress (&cinfo);
  jpeg_stdio_src (&cinfo, image);
  jpeg_read_header (&cinfo, TRUE);

  // This dictionary item is JPEG specific
  panda_adddictitem (output, imageObj, "Filter", panda_textvalue,
		     "DCTDecode");

  // Bits per component -- I'm not sure exactly how this works with libjpeg.
  // Is it possible to have a black and white jpeg? Ceasar Miquel
  // (miquel@df.uba.ar) has submitted patches suggesting that this should
  // always be 8, but this seems to work so I will leave it like this for now
  panda_adddictitem (output, imageObj, "BitsPerComponent", panda_integervalue,
		     cinfo.data_precision);

  // The colour device will change based on this number as well
  switch (cinfo.jpeg_color_space)
    {
    case JCS_GRAYSCALE:
      panda_adddictitem (output, imageObj, "ColorSpace", panda_textvalue,
			 "DeviceGray");
      break;

    default:
      panda_adddictitem (output, imageObj, "ColorSpace", panda_textvalue,
			 "DeviceRGB");
      break;
    }

  /****************************************************************************
     Some details of the image
  ****************************************************************************/

  panda_adddictitem (output, imageObj, "Width", panda_integervalue,
		     cinfo.image_width);
  panda_adddictitem (output, imageObj, "Height", panda_integervalue,
		     cinfo.image_height);

  // This cleans things up for us in the JPEG library
  jpeg_destroy_decompress (&cinfo);
  fclose (image);

  /****************************************************************************
    Read the image into it's memory buffer
  ****************************************************************************/

  imageBufSize = 0;
  imageObj->binarystreamLength = 0;

  if ((image = fopen (filename, "rb")) == NULL)
    panda_error (panda_true, "Could not open the JPEG file.");

  while ((c = fgetc (image)) != EOF)
    {
      // We need to grow the buffer
      if (imageBufSize == imageObj->binarystreamLength)
	{
	  imageBufSize += 1024;

	  if ((imageObj->binarystream = realloc (imageObj->binarystream,
						 imageBufSize)) == NULL)
	    panda_error (panda_true,
			 "Could not make enough space for the JPEG image.");
	}

      // Store the info
      imageObj->binarystream[imageObj->binarystreamLength++] = c;
    }

  imageObj->binarystream[imageObj->binarystreamLength++] = 0;
  fclose (image);

#if defined DEBUG
  printf ("Finished inserting the JPEG\n");
#endif
}

/******************************************************************************
DOCBOOK START

FUNCTION panda_insertPNG
PURPOSE insert a PNG image into the PDF

SYNOPSIS START
#include&lt;panda/constants.h&gt;
#include&lt;panda/functions.h&gt;
void panda_insertPNG (panda_pdf * output, panda_page * target, panda_object * imageObj, char *filename);
SYNOPSIS END

DESCRIPTION START
<command>PANDA INTERNAL</command>. Do the actual insertion of the PNG image into the PDF document. This routine handles the conversion of the PNG image into the form supported by the PDF specification, and similar operations. It is an internal <command>Panda</command> call and should not be needed by users of the API.

While developing this function call, some reference was made to the source code for iText (http://www.lowagie.com/iText), which is a GPL'ed PDF generator written in Java. The code in Panda is original though...
DESCRIPTION END

RETURNS Nothing

EXAMPLE START
See panda_imageboxrot for an example usage
EXAMPLE END
SEEALSO panda_imagebox, panda_imageboxrot, panda_insertJPEG, panda_insertTIFF
DOCBOOK END

-----------------------------------------------------------------------------
This code is loosely based on examples from the libpng package, as well as
some GPL'ed work from Paulo Soares for PNG insertion into PDFs using iText,
a Java PDF generator (http://www.lowagie.com/iText). The code below was,
however, written by mikal@stillhq.com... iText was merely used for hints on
how to do it.

The PNG specification is also a good read...
******************************************************************************/

void
panda_insertPNG (panda_pdf * output, panda_page * target,
		 panda_object * imageObj, char *filename)
{
  FILE *image;
  unsigned long width, height;
  int bitdepth, colourtype, outColourType;
  png_uint_32 i, rowbytes;
  png_structp png;
  png_infop info;
  unsigned char sig[8];
  png_bytepp row_pointers = NULL;

#if defined DEBUG
  printf ("Inserting a PNG image on page with object number %d.\n",
	  target->obj->number);
#endif

  // Open the file
  if ((image = fopen (filename, "rb")) == NULL)
    panda_error (panda_true, "Could not open the specified PNG file.");

  // Check that it really is a PNG file
  fread (sig, 1, 8, image);
  if (!png_check_sig (sig, 8))
    panda_error (panda_true, "PNG file was invalid");

  // Start decompressing
  if ((png = png_create_read_struct (PNG_LIBPNG_VER_STRING, NULL,
				     NULL, NULL)) == NULL)
    panda_error (panda_true,
		 "Could not create a PNG read structure (out of memory?)");

  if ((info = png_create_info_struct (png)) == NULL)
    panda_error (panda_true,
		 "Could not create PNG info structure (out of memory?)");

  // If panda_error did not exit, we would have to call png_destroy_read_struct

  if (setjmp (png_jmpbuf (png)))
    panda_error (panda_true, "Could not set PNG jump value");

  // Get ready for IO and tell the API we have already read the image signature
  // The IHDR chunk inside the PNG defines some info we need about the picture
  // (see PNG specification 1.2, page 15).
  png_init_io (png, image);
  png_set_sig_bytes (png, 8);
  png_read_info (png, info);
  png_get_IHDR (png, info, &width, &height, &bitdepth, &colourtype, NULL,
		NULL, NULL);

  // This dictionary item is PNG specific, but until we put uncompressed 
  // data into the PDF
  //panda_adddictitem (imageObj->dict, "Filter", panda_textvalue, "DCTDecode");

  // Get the pixel depth for the image from the PNG
  panda_adddictitem (output, imageObj, "BitsPerComponent", panda_integervalue,
		     bitdepth);

  // I can't find any documentation for why the predictor should always be
  // 15. If I ever do, then I will update this...
  panda_adddictitem (output, imageObj, "DecodeParms/Predictor",
		     panda_integervalue, 15);
  panda_adddictitem (output, imageObj, "DecodeParms/Columns",
		     panda_integervalue, width);
  panda_adddictitem (output, imageObj, "DecodeParms/BitsPerComponent",
		     panda_integervalue, bitdepth);

  // The colour device will change based on this number as well
  switch (colourtype)
    {
    case PNG_COLOR_TYPE_GRAY:
    case PNG_COLOR_TYPE_GRAY_ALPHA:
      panda_adddictitem (output, imageObj, "ColorSpace", panda_textvalue,
			 "DeviceGray");
      panda_adddictitem (output, imageObj, "DecodeParms/Colors",
			 panda_integervalue, 1);
      outColourType = PNG_COLOR_TYPE_GRAY;
      break;

    default:
      panda_adddictitem (output, imageObj, "ColorSpace", panda_textvalue,
			 "DeviceRGB");
      panda_adddictitem (output, imageObj, "DecodeParms/Colors",
			 panda_integervalue, 3);
      outColourType = PNG_COLOR_TYPE_RGB;
      break;
    }

  /****************************************************************************
     Some details of the image
  ****************************************************************************/

  panda_adddictitem (output, imageObj, "Width", panda_integervalue, width);
  panda_adddictitem (output, imageObj, "Height", panda_integervalue, height);
  panda_adddictitem (output, imageObj, "Filter", panda_textvalue,
		     "FlateDecode");

  /****************************************************************************
     Now actually insert the image. libpng lets us do some cool stuff with
     the data before it is handed to us like expanding it to our expectations.
     I don't use this at the moment, but reserve the right to one day...

     We read the image into a memory buffer...
  ****************************************************************************/

  /****************************************************************************
    Read the image into it's memory buffer
  ****************************************************************************/

  if (colourtype == PNG_COLOR_TYPE_PALETTE)
    png_set_expand (png);
  //  if(colourtype & PNG_COLOR_MASK_ALPHA)
  png_set_strip_alpha (png);
  png_read_update_info (png, info);

  rowbytes = png_get_rowbytes (png, info);
  imageObj->binarystream =
    (unsigned char *) panda_xmalloc ((rowbytes * height) + 1);
  imageObj->binarystreamLength = rowbytes * height;
  row_pointers = panda_xmalloc (height * sizeof (png_bytep));

  // Get the image bitmap
  for (i = 0; i < height; ++i)
    row_pointers[i] = imageObj->binarystream + (i * rowbytes);
  png_read_image (png, row_pointers);
  // free(row_pointers);
  png_read_end (png, NULL);

  imageObj->binarystream[imageObj->binarystreamLength++] = 0;
  fclose (image);

  // This cleans things up for us in the PNG library
  png_destroy_read_struct (&png, &info, NULL);

  /****************************************************************************
     ... And now we write that image out into another memory buffer (this one
     compressed) via some funky callbacks to libpng...
  ****************************************************************************/

  if ((png =
       png_create_write_struct (PNG_LIBPNG_VER_STRING, NULL, NULL,
				NULL)) == NULL)
    panda_error (panda_true,
		 "Could not create write structure for PNG (out of memory?)");

  if ((info = png_create_info_struct (png)) == NULL)
    panda_error
      (panda_true,
       "Could not create PNG info structure for writing (out of memory?)");

  if (setjmp (png_jmpbuf (png)))
    panda_error (panda_true, "Could not set the PNG jump value for writing");

  // If this call is done before png_create_write_struct, then everything seg faults...
#if defined _WINDOWS
  winmutex = CreateMutex (NULL, TRUE, "Panda");
  WaitForSingleObject (winmutex, INFINITE);
#else
  pthread_mutex_lock (&convMutex);
#endif

  png_set_write_fn (png, NULL, (png_rw_ptr) libpngDummyWriteProc,
		    (png_flush_ptr) libpngDummyFlushProc);
  globalIsIDAT = panda_false;
  globalImageBuffer = NULL;
  globalImageBufferOffset = 0;

  png_set_IHDR (png, info, width, height, bitdepth, outColourType,
		PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_DEFAULT,
		PNG_FILTER_TYPE_DEFAULT);
  png_write_info (png, info);

  png_write_image (png, row_pointers);
  png_write_end (png, info);
  png_destroy_write_struct (&png, &info);

  /****************************************************************************
     ... Finally, we have something that we can insert into the PDF stream
  ****************************************************************************/

  // Sometimes this might clobber and existing binary stream
  if (imageObj->binarystream != NULL)
    free (imageObj->binarystream);

  // We don't need row_pointers any more
  if (row_pointers != NULL)
    free (row_pointers);

  imageObj->binarystream = globalImageBuffer;
  imageObj->binarystreamLength = globalImageBufferOffset;

#if defined _WINDOWS
  ReleaseMutex (winmutex);
#else
  pthread_mutex_unlock (&convMutex);
#endif
}

/*****************************************************************************
  The following are dummy functions for libtiff that will allow us to do on-
  the-fly conversion of tiff images to the small subset of tiff that the
  PDF specification allows for...
*****************************************************************************/

/******************************************************************************
DOCBOOK START

FUNCTION libtiffDummyReadProc
PURPOSE mangle libtiff to do image conversion in memory without temportary files

SYNOPSIS START
static tsize_t libtiffDummyReadProc (thandle_t fd, tdata_t buf, tsize_t size);
SYNOPSIS END

DESCRIPTION <command>PANDA INTERNAL</command>. This call implements a dummy read procedure for libtiff to allow for the conversion of TIFF images on the fly in memory without the use of temporary files. It is an internal call and should NEVER be called by users of the API.

RETURNS The amount of data read

EXAMPLE START
See panda_insertTIFF for an example usage
EXAMPLE END
SEEALSO panda_insertTIFF, libtiffDummWriteProc
DOCBOOK END
******************************************************************************/

static tsize_t
libtiffDummyReadProc (thandle_t fd, tdata_t buf, tsize_t size)
{
  // Return the amount of data read, which we will always set as 0 because
  // we only need to be able to write to these in-memory tiffs
  return 0;
}

/******************************************************************************
DOCBOOK START

FUNCTION libtiffDummyWriteProc
PURPOSE mangle libtiff to do image conversion in memory without temportary files

SYNOPSIS START
static tsize_t libtiffDummyWriteProc (thandle_t fd, tdata_t buf, tsize_t size);
SYNOPSIS END

DESCRIPTION <command>PANDA INTERNAL</command>. This call implements a dummy write procedure for libtiff to allow for the conversion of TIFF images on the fly in memory without the use of temporary files. It is an internal call and should NEVER be called by users of the API.

RETURNS The amount of daat written

EXAMPLE START
See panda_insertTIFF for an example usage
EXAMPLE END
SEEALSO panda_insertTIFF, libtiffDummWriteProc
DOCBOOK END
******************************************************************************/

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
      if (globalImageBuffer == NULL)
	globalImageBuffer = (char *) panda_xmalloc (size * sizeof (char));

      // Otherwise, we need to grow the memory buffer
      else
	{
	  if ((globalImageBuffer = (char *) realloc (globalImageBuffer,
						     (size * sizeof (char)) +
						     globalImageBufferOffset))
	      == NULL)
	    panda_error (panda_true,
			 "Could not grow the tiff conversion memory buffer.");
	}

      // Now move the image data into the buffer
      memcpy (globalImageBuffer + globalImageBufferOffset, buf, size);
      globalImageBufferOffset += size;
    }

  return (size);
}

/******************************************************************************
DOCBOOK START

FUNCTION libtiffDummySeekProc
PURPOSE mangle libtiff to doc image conversion in memory without temportary files

SYNOPSIS START
static toff_t libtiffDummySeekProc (thandle_t, toff_t off, int i);
SYNOPSIS END

DESCRIPTION <command>PANDA INTERNAL</command>. This call implements a dummy seek procedure for libtiff to allow for the conversion of TIFF images on the fly in memory without the use of temporary files. It is an internal call and should NEVER be called by users of the API.

RETURNS Where the seek took us to

EXAMPLE START
See panda_insertTIFF for an example usage
EXAMPLE END
SEEALSO panda_insertTIFF, libtiffDummWriteProc
DOCBOOK END
******************************************************************************/

static toff_t
libtiffDummySeekProc (thandle_t fd, toff_t off, int i)
{
  // This appears to return the location that it went to
  return off;
}

/******************************************************************************
DOCBOOK START

FUNCTION libtiffDummyCloseProc
PURPOSE mangle libtiff to do image conversion in memory without temportary files

SYNOPSIS START
static int libtiffDummyCloseProc (thandle_t);
SYNOPSIS END

DESCRIPTION <command>PANDA INTERNAL</command>. This call implements a dummy close procedure for libtiff to allow for the conversion of TIFF images on the fly in memory without the use of temporary files. It is an internal call and should NEVER be called by users of the API.

RETURNS Zero

EXAMPLE START
See panda_insertTIFF for an example usage
EXAMPLE END
SEEALSO panda_insertTIFF, libtiffDummWriteProc
DOCBOOK END
******************************************************************************/

static int
libtiffDummyCloseProc (thandle_t fd)
{
  // Return a zero meaning all is well
  return 0;
}

/******************************************************************************
DOCBOOK START

FUNCTION libpngDummyWriteProc
PURPOSE mangle libpng to do image conversion in memory without temportary files

SYNOPSIS START
void libpngDummyWriteProc(png_structp png, png_bytep data, png_uint_32 len);
SYNOPSIS END

DESCRIPTION <command>PANDA INTERNAL</command>. This call implements a dummy write proceedure for libpng. This is needed so that Panda can get at the compressed PNG data, whilst converting it on the fly from unsupported PNG variants, and while not having to deal with temporary files. PDF documents only need the content of the IDAT chunks within the PNG file, and this method eases gaining access to those chunks.

RETURNS Zero

EXAMPLE START
See panda_insertPNG for an example usage
EXAMPLE END
SEEALSO panda_insertPNG, libpngDummyReadProc, libpngDummyFlushProc
DOCBOOK END
******************************************************************************/

void
libpngDummyWriteProc (png_structp png, png_bytep data, png_uint_32 len)
{
  char tempString[5];

  // Copy the first 4 bytes into a string
  tempString[0] = data[0];
  tempString[1] = data[1];
  tempString[2] = data[2];
  tempString[3] = data[3];
  tempString[4] = '\0';

  // If we know this is an IDAT, then copy the compressed image information
  if (globalIsIDAT == panda_true)
    {
#if defined DEBUG
      printf ("Inserted %d bytes into the PNG\n", len);
#endif

      // Have we done anything yet?
      if (globalImageBuffer == NULL)
	globalImageBuffer = (char *) panda_xmalloc (len * sizeof (char));

      // Otherwise, we need to grow the memory buffer
      else
	{
	  if ((globalImageBuffer = (char *) realloc (globalImageBuffer,
						     (len * sizeof (char)) +
						     globalImageBufferOffset))
	      == NULL)
	    panda_error (panda_true,
			 "Could not grow the png conversion memory buffer.");
	}

      // Now move the image data into the buffer
      memcpy (globalImageBuffer + globalImageBufferOffset, data, len);
      globalImageBufferOffset += len;

      globalIsIDAT = panda_false;
    }
  else if ((len == 4) && (strcmp (tempString, "IDAT") == 0))
    globalIsIDAT = panda_true;
  else
    globalIsIDAT = panda_false;
}

/******************************************************************************
DOCBOOK START

FUNCTION libpngDummyFlushProc
PURPOSE mangle libpng to do image conversion in memory without temportary files

SYNOPSIS START
void libpngDummyFlushProc(png_structp png);
SYNOPSIS END

DESCRIPTION <command>PANDA INTERNAL</command>. This call implements a dummy flush proceedure for libpng. This is needed so that Panda can get at the compressed PNG data, whilst converting it on the fly from unsupported PNG variants, and while not having to deal with temporary files. PDF documents only need the content of the IDAT chunks within the PNG file, and this method eases gaining access to those chunks.

RETURNS Zero

EXAMPLE START
See panda_insertPNG for an example usage
EXAMPLE END
SEEALSO panda_insertPNG, libpngDummyReadProc, libpngDummyWriteProc
DOCBOOK END
******************************************************************************/

void
libpngDummyFlushProc (png_structp png)
{
}


/******************************************************************************
DOCBOOK START

FUNCTION panda_imagesize
PURPOSE determine the size of an image

SYNOPSIS START
#include&lt;panda/constants.h&gt;
#include&lt;panda/functions.h&gt;
void panda_panda_imagesize (int *width, int *height, char *filename, int type);
SYNOPSIS END

DESCRIPTION This function can be used to determine the size of an image before a <command>panda_imagebox</command>() function call.
RETURNS Nothing

EXAMPLE START
#include&lt;panda/constants.h&gt;
#include&lt;panda/functions.h&gt;

int height, width;

panda_init ();
  
panda_imagesize (&width, &height, "input.tif", panda_image_tiff);
EXAMPLE END
SEEALSO panda_imagebox, panda_imageboxrot
DOCBOOK END
******************************************************************************/ 
void 
panda_imagesize (int *width, int *height, char *filename, int type) 
{
  *width = 0;
  *height = 0;
  switch (type)
    
    {
    case panda_image_tiff:
      
#if defined HAVE_LIBTIFF
	panda_imagesizeTIFF (width, height, filename);
      
#else /*  */
	fprintf (stderr, "%s %s\n",
		 "TIFF support not compiled into Panda because libtiff was",
		 "not found at compile time.");
      
#endif /*  */
	break;
    case panda_image_jpeg:
      
#if defined HAVE_LIBJPEG
	panda_imagesizeJPEG (width, height, filename);
      
#else /*  */
	fprintf (stderr, "%s %s\n",
		 "JPEG support not compiled into Panda because libjpeg was",
		 "not found at compile time.");
      
#endif /*  */
	break;
    case panda_image_png:
      
#if defined HAVE_LIBPNG
	panda_imagesizePNG (width, height, filename);
      
#else /*  */
	fprintf (stderr, "%s %s\n",
		 "PNG support not compiled into Panda because libpng was not",
		 "found at compile time.");
      
#endif /*  */
	break;
    }
}
void
panda_imagesizeTIFF (int *width, int *height, char *filename) 
{
  uint32 twidth, theight;
  TIFF * image;
  if ((image = TIFFOpen (filename, "r")) == NULL)
    panda_error (panda_true, "Could not open TIFF image to determine size");
  TIFFGetField (image, TIFFTAG_IMAGEWIDTH, &twidth);
  TIFFGetField (image, TIFFTAG_IMAGELENGTH, &theight);
  *width = twidth;
  *height = theight;
}
void
panda_imagesizeJPEG (int *width, int *height, char *filename) 
{
  struct jpeg_decompress_struct cinfo;
  struct jpeg_error_mgr jerr;
  FILE * image;
  
    // Open the file -- why is this a memory leak?
    if ((image = fopen (filename, "rb")) == NULL)
    panda_error (panda_true, "Could not open the specified JPEG file.");
  
    // Setup the decompression options
    cinfo.err = jpeg_std_error (&jerr);
  
    // Start decompressing
    jpeg_create_decompress (&cinfo);
  jpeg_stdio_src (&cinfo, image);
  jpeg_read_header (&cinfo, TRUE);
  *width = cinfo.image_width;
  *height = cinfo.image_height;
  
    // This cleans things up for us in the JPEG library
    jpeg_destroy_decompress (&cinfo);
  fclose (image);
}
void
panda_imagesizePNG (int *width, int *height, char *filename) 
{
  FILE * image;
  unsigned long pwidth, pheight;
  int bitdepth, colourtype;
  png_structp png;
  png_infop info;
  unsigned char sig[8];
  
    // Open the file
    if ((image = fopen (filename, "rb")) == NULL)
    panda_error (panda_true, "Could not open the specified PNG file.");
  
    // Check that it really is a PNG file
    fread (sig, 1, 8, image);
  if (!png_check_sig (sig, 8))
    panda_error (panda_true, "PNG file was invalid");
  
    // Start decompressing
    if ((png =
	 png_create_read_struct (PNG_LIBPNG_VER_STRING, NULL, NULL,
				 NULL)) == NULL)
    panda_error (panda_true,
		  "Could not create a PNG read structure (out of memory?)");
  if ((info = png_create_info_struct (png)) == NULL)
    panda_error (panda_true,
		  "Could not create PNG info structure (out of memory?)");
  
    // If panda_error did not exit, we would have to call png_destroy_read_struct
    if (setjmp (png_jmpbuf (png)))
    panda_error (panda_true, "Could not set PNG jump value");
  
    // Get ready for IO and tell the API we have already read the image signature
    // The IHDR chunk inside the PNG defines some info we need about the picture
    // (see PNG specification 1.2, page 15).
    png_init_io (png, image);
  png_set_sig_bytes (png, 8);
  png_read_info (png, info);
  png_get_IHDR (png, info, &pwidth, &pheight, &bitdepth, &colourtype, NULL,
		 NULL, NULL);
  *width = pwidth;
  *height = pheight;
}
