/******************************************************************************
  Constants.h

  Change Control:                                                      DDMMYYYY
    Michael Still    File created                                      08062000

  Purpose:
    This file defines some constants that are used in various places
    throughout the code. If the value GLOBALS is defined, then we are in 
    the one source file which owns the variables
******************************************************************************/

// This might cause problems with other people's applications... Email
// mikal@stillhq.com and let me know if you have pain...
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#ifndef PANDA_CONSTANTS_H
#define PANDA_CONSTANTS_H 1

#ifdef __cplusplus
extern "C"
{
#endif

// The text that we make binary to display in the first 100 bytes or so of the
// PDF (forces some FTP clients into binary mode). The function inipanda
// actually generates the gBinaryHeaderString string value.
#define  panda_magicheaderstring    "%PDF-1.3 "
#define  panda_headerstring         "Panda, 2000"
#define  panda_binarychar(x)        256 - x

#ifndef VERSION
#define VERSION "0.5.2d"
#endif

#if defined GLOBALS
  char panda_binaryheaderstring[14];
#else
  extern char panda_binaryheaderstring[12];
#endif

  // General values
  enum
  {
    // True and false
    panda_true = 0,
    panda_false,

    // We sometimes need to specify what type of value we are passing to a 
    // function. These are the supported types.
    panda_brackettedtextvalue,
    panda_dictionaryvalue,
    panda_integervalue,
    panda_textvalue,
    panda_literaltextvalue,
    panda_objectvalue,
    panda_objectarrayvalue,
    panda_booleanvalue,
    panda_doublevalue,

    // Up and down movement for the traversal functions
    panda_up,
    panda_down,

    // File modes for the PDF object
    panda_write,
    panda_writelinear,

    // We have the concept of placeholder objects, that do not get written out to
    // the PDF
    panda_placeholder,
    panda_normal
  };

// Standard page sizes -- these are expressed in inches, with 72 dots per inch
// with no fractional values shown. As many values as needed can be added here
#define  panda_pagesize_a4                "[0 0 594 841]"
#define  panda_pagesize_usletter          "[0 0 612 792]"

// The text drawing modes (p 343 v 1.3)
  enum
  {
    panda_textmode_normal = 0,
    panda_textmode_outline,
    panda_textmode_filledoutline,
    panda_textmode_invisible,
    panda_textmode_filledclipped,
    panda_textmode_strokedclipped,
    panda_textmode_filledstrokedclipped,
    panda_textmode_clipped
  };

// The line cap styles (p 139 v 1.3 II)
  enum
  {
    panda_linecap_butt = 0,
    panda_linecap_round,
    panda_linecap_projectedsquare,
    panda_linecap_max		// Not in the spec!
  };

// The line join styles (p 140 v 1.3 II)
  enum
  {
    panda_linejoin_miter = 0,
    panda_linejoin_round,
    panda_linejoin_bevel,
    panda_linejoin_max		// Not in the spec!
  };

// The image types supported
  enum
  {
    panda_image_tiff = 0,
    panda_image_jpeg,
    panda_image_png
  };

// The display modes that the viewer window can be asked to conform to
  enum
  {
    panda_window_usenone = 0,
    panda_window_useoutlines,
    panda_window_usethumbs
  };

// Possible text directions (for reading the document)
  enum
  {
    panda_textdirection_l2r = 0,
    panda_textdirection_r2l
  };

// Possible page transition effects
  enum
  {
    panda_pagetrans_split_yi = 0,
    panda_pagetrans_split_yo,
    panda_pagetrans_split_xi,
    panda_pagetrans_split_xo,
    panda_pagetrans_blinds_y,
    panda_pagetrans_blinds_x,
    panda_pagetrans_box_i,
    panda_pagetrans_box_o,
    panda_pagetrans_wipe_0,
    panda_pagetrans_wipe_90,
    panda_pagetrans_wipe_180,
    panda_pagetrans_wipe_270,
    panda_pagetrans_dissolve,
    panda_pagetrans_glitter_0,
    panda_pagetrans_glitter_270,
    panda_pagetrans_glitter_315,
    panda_pagetrans_none
  };

// Possible annotation icons
  enum
  {
    panda_icon_comment = 0,
    panda_icon_help,
    panda_icon_insert,
    panda_icon_key,
    panda_icon_newparagraph,
    panda_icon_note,
    panda_icon_paragraph
  };

// Flags on annotation objects (these are bit fields to be OR'ed)
  enum
  {
    panda_annotationflag_invisible = 1,
    panda_annotationflag_hidden = 2,
    panda_annotationflag_print = 4,
    panda_annotationflag_nozoom = 8,
    panda_annotationflag_norotate = 16,
    panda_annotationflag_noview = 32,
    panda_annotationflag_readonly = 64
  };

// Properties that might be held by objects -- panda_const_properties
  enum
  {
    panda_object_property_compress = 0,
    panda_object_property_compress_level,
    panda_object_property_ignorepageerrors,
    panda_object_property_binarystreamfile,
    panda_object_property_layoutstreamfile,
    panda_object_property_max
  };

// Scopes of properties
  enum
  {
    panda_scope_cascade = 0,
    panda_scope_local
  };

  // Justifcation/Horiz Alignment Types
  enum
  {
    panda_halign_left = 1,
    panda_halign_right,
    panda_halign_center,
    panda_halign_justify
  };

  // Vertical alignment types
  enum
  {
    panda_valign_top = 1,
    panda_valign_bottom,
    panda_valign_middle
  };

#if defined _WINDOWS
  // The abstraction types that we use
  enum
  {
    windows_panda_abs_pdf = 0,
    windows_panda_abs_page,
    windows_panda_abs_font,

    windows_panda_abs_max
  };

#endif				/* 
				 */
  // Sometimes we need PI
#define panda_pi 3.1415926535
// The default compression level
#define panda_default_compress_level 6
#ifdef __cplusplus
}
#endif

#endif				/* PANDA_CONSTANTS_H */
