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
#define VERSION "0.6.0"
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

  /* Standard page sizes -- these are expressed in inches, with 72
     dots per inch with no fractional values shown. As many values as
     needed can be added here.

     Conversions:
     o There are approximately 0.0393700787 mm per in

     Most of these sizes are from
     http://www.math.wayne.edu/~rrb/A4.html
   */

// ISO 215 sizes
// A series
#define  panda_pagesize_4a0        "[0 0 4768 6735]" // 1682 × 2378 mm
#define  panda_pagesize_2a0        "[0 0 3370 4768]" // 1189 × 1682 mm
#define  panda_pagesize_a0         "[0 0 2384 3370]" // 841 × 1189 mm
#define  panda_pagesize_a1         "[0 0 1684 2384]" // 594 × 841 mm
#define  panda_pagesize_a2         "[0 0 1191 1684]" // 420 × 594 mm
#define  panda_pagesize_a3         "[0 0 842 1191]"  // 297 × 420 mm
#define  panda_pagesize_a4         "[0 0 595 842]"   // 210 × 297 mm
#define  panda_pagesize_a5         "[0 0 420 595]"   // 148 × 210 mm
#define  panda_pagesize_a6         "[0 0 298 420]"   // 105 × 148 mm
#define  panda_pagesize_a7         "[0 0 210 298]"   // 74 × 105 mm
#define  panda_pagesize_a8         "[0 0 147 210]"   // 52 × 74 mm
#define  panda_pagesize_a9         "[0 0 105 147]"   // 37 × 52 mm
#define  panda_pagesize_a10        "[0 0 74 105]"    // 26 × 37 mm
// B series
#define  panda_pagesize_b0         "[0 0 2835 4008]" // 1000 × 1414 mm
#define  panda_pagesize_b1         "[0 0 2004 2835]" // 707 × 1000 mm
#define  panda_pagesize_b2         "[0 0 1417 2004]" // 500 × 707 mm
#define  panda_pagesize_b3         "[0 0 1001 1417]" // 353 × 500 mm
#define  panda_pagesize_b4         "[0 0 709 1001]"  // 250 × 353 mm
#define  panda_pagesize_b5         "[0 0 499 709]"   // 176 × 250 mm
#define  panda_pagesize_b6         "[0 0 354 499]"   // 125 × 176 mm
#define  panda_pagesize_b7         "[0 0 249 354]"   // 88 × 125 mm
#define  panda_pagesize_b8         "[0 0 176 249]"   // 62 × 88 mm
#define  panda_pagesize_b9         "[0 0 125 176]"   // 44 × 62 mm
#define  panda_pagesize_b10        "[0 0 88 125]"    // 31 × 44 mm
// C series
#define  panda_pagesize_c0         "[0 0 2599 3677]" // 917 × 1297 mm
#define  panda_pagesize_c1         "[0 0 1837 2599]" // 648 × 917 mm
#define  panda_pagesize_c2         "[0 0 1298 1837]" // 458 × 648 mm
#define  panda_pagesize_c3         "[0 0 918 1298]"  // 324 × 458 mm
#define  panda_pagesize_c4         "[0 0 649 918]"   // 229 × 324 mm
#define  panda_pagesize_c5         "[0 0 459 649]"   // 162 × 229 mm
#define  panda_pagesize_c6         "[0 0 323 459]"   // 114 × 162 mm
#define  panda_pagesize_c7         "[0 0 230 323]"   // 81 × 114 mm
#define  panda_pagesize_c8         "[0 0 162 230]"   // 57 × 81 mm
#define  panda_pagesize_c9         "[0 0 113 162]"   // 40 × 57 mm
#define  panda_pagesize_c10        "[0 0 79 113]"    // 28 × 40 mm
// Raw format A
#define panda_pagesize_ra0         "[0 0 2438 3458]" // 860 × 1220 mm
#define panda_pagesize_ra1         "[0 0 1729 2438]" // 610 × 860 mm
#define panda_pagesize_ra2         "[0 0 1219 1729]" // 430 × 610 mm
#define panda_pagesize_ra3         "[0 0 865 1219]"  // 305 × 430 mm
#define panda_pagesize_ra4         "[0 0 609 865]"   // 215 × 305 mm
// Supplementary raw format A
#define panda_pagesize_sra0        "[0 0 2551 3628]" // 900 × 1280 mm
#define panda_pagesize_sra1        "[0 0 1814 2551]" // 640 × 900 mm
#define panda_pagesize_sra2        "[0 0 1276 1814]" // 450 × 640 mm
#define panda_pagesize_sra3        "[0 0 907 1276]"  // 320 × 450 mm
#define panda_pagesize_sra4        "[0 0 225 907]"   // 225 × 320 mm
// US specific formats.
#define panda_pagesize_usletter    "[0 0 612 792]"   // 8.5 x 11 in
#define panda_pagesize_uslegal     "[0 0 612 1008]"  // 8.5 x 14 in
#define panda_pagesize_usexecutive "[0 0 522 756]"   // 7.25 x 10.5 in
#define panda_pagesize_usledger    "[0 0 792 1224]"  // 11 x 17 in
// ANSI/ASME Y14.1 technical drawings
#define panda_pagesize_usarch1     "[0 0 648 864]"   // 9 × 12 in
#define panda_pagesize_usarch2     "[0 0 864 1298]"  // 12 × 18 in
#define panda_pagesize_usarch3     "[0 0 1298 1728]" // 18 × 24 in
#define panda_pagesize_usarch4     "[0 0 1728 2592]" // 24 × 36 in
#define panda_pagesize_usarch5     "[0 0 2160 3024]" // 30 × 42 in
#define panda_pagesize_usarch6     "[0 0 2592 3456]" // 36 × 48 in
// Overhead projectors.
#define panda_pagesize_overheada   "[0 0 709 709]" // 250 x 250 mm
#define panda_pagesize_overheadb   "[0 0 808 808]" // 285 x 285 mm
// Identification cards
#define panda_pagesize_id1         "[0 0 343 153]" // 3.370 × 2.125 in
#define panda_pagesize_id2         "[0 0 298 210]" // 105 × 74 mm (A7)
#define panda_pagesize_id3         "[0 0 354 249]" // 125 × 88 mm (B7)


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
