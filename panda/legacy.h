/******************************************************************************
  Legacy.h

  Change Control:                                                      DDMMYYYY
    Michael Still    File created                                      10012001

  Purpose:
    This file provides a mapping between the legacy function call names and
    the new names...

******************************************************************************/

#include "objects.h"

#ifndef PANDA_LEGACY_H
#define PANDA_LEGACY_H 1

#ifdef __cplusplus
extern "C"
{
#endif

#define makedate panda_makedate
#define nowdate panda_nowdate
#define error panda_error
#define imagebox panda_imagebox
#define imageboxrot panda_imageboxrot
#define insertTiff panda_insertTIFF
#define insertJpeg panda_insertJPEG
#define insertPNG panda_insertPNG
#define checkInfo panda_checkinfo
#define setauthor panda_setauthor
#define setcreator panda_setcreator
#define settitle panda_settitle
#define setsubject panda_setsubject
#define setkeywords panda_setkeywords
#define createfont panda_createfont
#define setfont panda_setfont
#define setfontsize panda_setfontsize
#define getfontobj panda_getfontobj
#define setfontmode panda_setfontmode
#define setcharacterspacing panda_setcharacterspacing
#define setwordspacing panda_setwordspacing
#define sethorizontalscaling panda_sethorizontalscaling
#define setleading panda_setleading
#define xmalloc panda_xmalloc
#define initpanda panda_init
#define pdfopen panda_open
#define pdfopen_suppress panda_opensuppress
#define pdfopen_actual panda_openactual
#define pdfclose panda_close
#define pdfpage panda_newpage
#define closeText panda_closetext
#define newobject panda_newobject
#define adddictitem panda_adddictitem
#define getdictvalue panda_getdictvalue
#define getdict panda_getdict
#define freeObject panda_freeobject
#define freeDictionary panda_freedictionary
#define writeObject panda_writeobject
#define writeDictionary panda_writedictionary
#define addchild panda_addchild
#define traverseObjects panda_traverseobjects
#define textbox panda_textbox
#define writeTrailer panda_writetrailer
#define pdfprintf panda_printf
#define streamprintf panda_streamprintf
#define pdfputc panda_putc
#define pdfprint panda_print
#define writeXref panda_writexref

#define objectArray panda_objectarray
#define dictionary panda_dictionary
#define object panda_object
#define page panda_page
#define child panda_child
#define xref panda_xref
#define pdf panda_pdf

#define gBracketedTextValue panda_brackettedtextvalue
#define gDictionaryValue panda_dictionaryvalue
#define gIntValue panda_integervalue
#define gTextValue panda_textvalue
#define gLiteralTextValue panda_literaltextvalue
#define gObjValue panda_objectvalue
#define gObjArrayValue panda_objectarrayvalue
#define gTrue panda_true
#define gFalse panda_false
#define gUp panda_up
#define gDown panda_down
#define gWrite panda_write
#define gWriteLinear panda_writelinear
#define gPageSizeA4 panda_pagesize_a4
#define gPageSizeUsLetter panda_pagesize_usletter
#define gPlaceholder panda_placeholder
#define gNormal panda_normal
#define gTextModeNormal panda_textmode_normal
#define gTextModeOutline panda_textmode_outline
#define gTextModeFilledOutline panda_textmode_filledoutline
#define gTextModeInvisible panda_textmode_invisible
#define gTextModeFilledClipped panda_textmode_filledclipped
#define gTextModeStrokedClipped panda_textmode_strokedclipped
#define gTextModeFilledStrokedClipped panda_textmode_filledstrokedclipped
#define gTextModeClipped panda_textmode_clipped
#define gImageTiff panda_image_tiff
#define gImageJpeg panda_image_jpeg
#define gImagePNG panda_image_png

#ifdef __cplusplus
}
#endif

#endif				/* PANDA_LEGACY_H */
