/******************************************************************************
  Constants.h

  Change Control:                                                      DDMMYYYY
    Michael Still    File created                                      08062000

  Purpose:
    This file defines some constants that are used in various places
    throughout the code. If the value GLOBALS is defined, then we are in 
    the one source file which owns the variables
******************************************************************************/

#ifndef PANDA_CONSTANTS_H
#define PANDA_CONSTANTS_H 1

#ifdef __cplusplus
extern "C" {
#endif

// The text that we make binary to display in the first 100 bytes or so of the
// PDF (forces some FTP clients into binary mode). The function inipanda
// actually generates the gBinaryHeaderString string value.
#define  panda_magicheaderstring    "%PDF-1.3 "
#define  panda_headerstring         "Panda, 2000"
#define  panda_binarychar(x)        256 - x

#if defined GLOBALS
char panda_binaryheaderstring[14];
#else
extern char panda_binaryheaderstring[12];
#endif

// We sometimes need to specify what type of value we are passing to a 
// function. These are the supported types.
#define  panda_brackettedtextvalue 6
#define  panda_dictionaryvalue     4
#define  panda_integervalue        0
#define  panda_textvalue           1
#define  panda_literaltextvalue    5
#define  panda_objectvalue         2
#define  panda_objectarrayvalue    3

// True and false
#define  panda_true               0
#define  panda_false              1

// Up and down movement for the traversal functions
#define  panda_up                 7
#define  panda_down               8

// File modes for the PDF object
#define  panda_write              9
#define  panda_writelinear        10

// Standard page sizes -- these are expressed in inches, with 72 dots per inch
// with no fractional values shown. As many values as needed can be added here
#define  panda_pagesize_a4                "[0 0 594 841]"
#define  panda_pagesize_usletter          "[0 0 612 792]"
#define  panda_pagesize_template_a4       "TEMPLATE [0 0 594 841]"
#define  panda_pagesize_template_usletter "TEMPLATE [0 0 612 792]"

// We have the concept of placeholder objects, that do not get written out to
// the PDF
#define  panda_placeholder        0
#define  panda_normal             1

// The text drawing modes (p 343 v 1.3)
enum
{
  gTextModeNormal = 0,
  gTextModeOutline,
  gTextModeFilledOutline,
  gTextModeInvisible,
  gTextModeFillClipped,
  gTextModeStrokeClipped,
  gTextModeFillStrokeClipped,
  gTextModeClipped
};

// The image types supported
enum
{
  gImageTiff = 0,
  gImageJpeg,
  gImagePNG
};

#ifdef __cplusplus
}
#endif

#endif /* PANDA_CONSTANTS_H */
