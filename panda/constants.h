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
#define  gMagicHeaderString    "%PDF-1.3 "
#define  gHeaderString         "Panda, 2000"
#define  gBinaryChar(x)        256 - x

#if defined GLOBALS
char gBinaryHeaderString[14];
#else
extern char gBinaryHeaderString[12];
#endif

// We sometimes need to specify what type of value we are passing to a 
// function. These are the supported types.
#define  gBracketedTextValue 6
#define  gDictionaryValue    4
#define  gIntValue           0
#define  gTextValue          1
#define  gLiteralTextValue   5
#define  gObjValue           2
#define  gObjArrayValue      3

// True and false
#define  gTrue               0
#define  gFalse              1

// Up and down movement for the traversal functions
#define  gUp                 7
#define  gDown               8

// File modes for the PDF object
#define  gWrite              9
#define  gWriteLinear        10

// Standard page sizes -- these are expressed in inches, with 72 dots per inch
// with no fractional values shown. As many values as needed can be added here
#define  gPageSizeA4                "[0 0 594 841]"
#define  gPageSizeUsLetter          "[0 0 612 792]"
#define  gPageSizeTemplateA4        "TEMPLATE [0 0 594 841]"
#define  gPageSizeTemplayeUsLetter  "TEMPLATE [0 0 612 792]"

// We have the concept of placeholder objects, that do not get written out to
// the PDF
#define  gPlaceholder        0
#define  gNormal             1

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
