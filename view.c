/******************************************************************************
  View.c

  Change Control:                                                      DDMMYYYY
    Michael Still    File created                                      08012002

  Purpose:
    Despite the name, this file has nothing to do with viewing PDF documents.
    If you are looking for a viewer, then I recommend gv...

    This file is responsible for the output of parts of the PDF which control
    the user's viewing experience.

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
#else
#include <panda/constants.h>
#include <panda/functions.h>
#endif

/******************************************************************************
DOCBOOK START

FUNCTION panda_hidetoolbar
PURPOSE ask the viewer to hide it's tool bar when this PDF is displayed

SYNOPSIS START
#include&lt;panda/constants.h&gt;
#include&lt;panda/functions.h&gt;
void panda_hidetoolbar (panda_pdf *document, int onoroff);
SYNOPSIS END

DESCRIPTION This function records information in the output PDF document requesting that the viewing application's tool bar not be displayed when this PDF is opened. This option is not supported by all viewers, and therefore should not be relied upon. The on or off arguement is a panda_true value, which does the obvious thing. The default is to show the tool bar.

RETURNS Nothing

EXAMPLE START
#include&lt;panda/constants.h&gt;
#include&lt;panda/functions.h&gt;

panda_pdf *document;

panda_init();

document = panda_open("filename.pdf", "w");
panda_hidetoolbar(document, panda_true);
EXAMPLE END
DOCBOOK END
******************************************************************************/

void
panda_hidetoolbar (panda_pdf * document, int onoff)
{
  panda_adddictitem (document, document->catalog,
		     "ViewerPreferences/HideToolbar",
		     panda_booleanvalue, onoff);
}

/******************************************************************************
DOCBOOK START

FUNCTION panda_hidemenubar
PURPOSE ask the viewer to hide it's menu bar when this PDF is displayed

SYNOPSIS START
#include&lt;panda/constants.h&gt;
#include&lt;panda/functions.h&gt;
void panda_hidemenubar (panda_pdf *document, int onoroff);
SYNOPSIS END

DESCRIPTION This function records information in the output PDF document requesting that the viewing application's menu bar not be displayed when this PDF is opened. This option is not supported by all viewers, and therefore should not be relied upon. The on or off arguement is a panda_true value, which does the obvious thing. The default is to show the menu bar.

RETURNS Nothing

EXAMPLE START
#include&lt;panda/constants.h&gt;
#include&lt;panda/functions.h&gt;

panda_pdf *document;

panda_init();

document = panda_open("filename.pdf", "w");
panda_hidemenubar(document, panda_true);
EXAMPLE END
DOCBOOK END
******************************************************************************/

void
panda_hidemenubar (panda_pdf * document, int onoff)
{
  panda_adddictitem (document, document->catalog,
		     "ViewerPreferences/HideMenubar",
		     panda_booleanvalue, onoff);
}

/******************************************************************************
DOCBOOK START

FUNCTION panda_hidewindowui
PURPOSE ask the viewer to hide it's display window user interface when this PDF is displayed

SYNOPSIS START
#include&lt;panda/constants.h&gt;
#include&lt;panda/functions.h&gt;
void panda_hidewindowui (panda_pdf *document, int onoroff);
SYNOPSIS END

DESCRIPTION This function records information in the output PDF document requesting that the viewing application's window user interface not be displayed when this PDF is opened. This option is not supported by all viewers, and therefore should not be relied upon. The on or off arguement is a panda_true value, which does the obvious thing. The default is to display the user interface.

RETURNS Nothing

EXAMPLE START
#include&lt;panda/constants.h&gt;
#include&lt;panda/functions.h&gt;

panda_pdf *document;

panda_init();

document = panda_open("filename.pdf", "w");
panda_hidewindowui(document, panda_true);
EXAMPLE END
DOCBOOK END
******************************************************************************/

void
panda_hidewindowui (panda_pdf * document, int onoff)
{
  panda_adddictitem (document, document->catalog,
		     "ViewerPreferences/HideWindowUI",
		     panda_booleanvalue, onoff);
}

/******************************************************************************
DOCBOOK START

FUNCTION panda_fitwindow
PURPOSE ask the viewer to fit the viewer window to the first page of the PDF document when it is opened

SYNOPSIS START
#include&lt;panda/constants.h&gt;
#include&lt;panda/functions.h&gt;
void panda_fitwindow (panda_pdf *document, int onoroff);
SYNOPSIS END

DESCRIPTION This function records information in the output PDF document requesting that the viewing application fit the display window to the first page of the PDF document when it is opened. This option is not supported by all viewers, and therefore should not be relied upon. The on or off arguement is a panda_true value, which does the obvious thing. The default is to not fit the document to the window.

RETURNS Nothing

EXAMPLE START
#include&lt;panda/constants.h&gt;
#include&lt;panda/functions.h&gt;

panda_pdf *document;

panda_init();

document = panda_open("filename.pdf", "w");
panda_fitwindow(document, panda_true);
EXAMPLE END
DOCBOOK END
******************************************************************************/

void
panda_fitwindow (panda_pdf * document, int onoff)
{
  panda_adddictitem (document, document->catalog,
		     "ViewerPreferences/FitWindow",
		     panda_booleanvalue, onoff);
}

/******************************************************************************
DOCBOOK START

FUNCTION panda_centerwindow
PURPOSE ask the viewer to center the document's window on the screen when the PDF is displayed

SYNOPSIS START
#include&lt;panda/constants.h&gt;
#include&lt;panda/functions.h&gt;
void panda_centerwindow (panda_pdf *document, int onoroff);
SYNOPSIS END

DESCRIPTION This function records information in the output PDF document requesting that the viewing application center the displayed PDF document on the screen when it is opened. This option is not supported by all viewers, and therefore should not be relied upon. The on or off arguement is a panda_true value, which does the obvious thing. The default is to not center the window.

RETURNS Nothing

EXAMPLE START
#include&lt;panda/constants.h&gt;
#include&lt;panda/functions.h&gt;

panda_pdf *document;

panda_init();

document = panda_open("filename.pdf", "w");
panda_centerwindow(document, panda_true);
EXAMPLE END
DOCBOOK END
******************************************************************************/

void
panda_centerwindow (panda_pdf * document, int onoff)
{
  panda_adddictitem (document, document->catalog,
		     "ViewerPreferences/CenterWindow",
		     panda_booleanvalue, onoff);
}

/******************************************************************************
DOCBOOK START

FUNCTION panda_nfspagemodde
PURPOSE defines display characteristics for the PDF document if it is using non fullscreen mode after defaulting to fullscreen mode

SYNOPSIS START
#include&lt;panda/constants.h&gt;
#include&lt;panda/functions.h&gt;
void panda_nfspagemode (panda_pdf *document, int pagemode);
SYNOPSIS END

DESCRIPTION If the document in question is using fullscreen mode and then exits from fullscreen mode, then this function configures the behaviour of several of the 'eye candy' options available in some viewers. The possible values for pagemode are: <command>panda_window_usenone</command>, which displays neither the outline or thumbnails (if present); <command>panda_window_useoutlines</command>, which displays only the outline for the document; <command>panda_window_usethumbs</command>, which only displays thumbnails. <command>Please note that this function will only have an effect on the viewer if the page mode has been set to fullscreen with the panda_fullscreen() function call</command>

RETURNS Nothing

EXAMPLE START
#include&lt;panda/constants.h&gt;
#include&lt;panda/functions.h&gt;

panda_pdf *document;

panda_init();

document = panda_open("filename.pdf", "w");
panda_nfspagemode(document, panda_window_usenone);
EXAMPLE END
SEEALSO panda_fullscreen
DOCBOOK END
******************************************************************************/

void
panda_nfspagemode (panda_pdf * document, int pagemode)
{
  switch (pagemode)
    {
    case panda_window_usenone:
      panda_adddictitem (document, document->catalog,
			 "ViewerPreferences/NonFullScreenPageMode",
			 panda_textvalue, "UseNone");
      break;

    case panda_window_useoutlines:
      panda_adddictitem (document, document->catalog,
			 "ViewerPreferences/NonFullScreenPageMode",
			 panda_textvalue, "UseOutlines");
      break;

    case panda_window_usethumbs:
      panda_adddictitem (document, document->catalog,
			 "ViewerPreferences/NonFullScreenPageMode",
			 panda_textvalue, "UseThumbs");
      break;

    default:
      panda_error (panda_false,
		   "Attempt to set a value for the non fullscreen page mode which is invalid");
      break;
    }
}

/******************************************************************************
DOCBOOK START

FUNCTION panda_fullscreen
PURPOSE ask the viewer to display the PDF document in fullscreen mode

SYNOPSIS START
#include&lt;panda/constants.h&gt;
#include&lt;panda/functions.h&gt;
void panda_fullscreen (panda_pdf *document, int onoroff);
SYNOPSIS END

DESCRIPTION This function records information in the output PDF document requesting that the viewing application display the document in full screen mode. This option is not supported by all viewers, and therefore should not be relied upon. The on or off arguement is a panda_true value, which does the obvious thing. The default is to not display the PDF in fullscreen mode.

RETURNS Nothing

EXAMPLE START
#include&lt;panda/constants.h&gt;
#include&lt;panda/functions.h&gt;

panda_pdf *document;

panda_init();

document = panda_open("filename.pdf", "w");
panda_fullscreen(document, panda_true);
EXAMPLE END
DOCBOOK END
******************************************************************************/

void
panda_fullscreen (panda_pdf * document, int onoff)
{
  // todo_mikal

  if (onoff == panda_true)
    panda_adddictitem (document, document->catalog, "PageMode",
		       panda_textvalue, "FullScreen");
  else
    panda_adddictitem (document, document->catalog, "PageMode",
		       panda_textvalue, "TODO");
}

/******************************************************************************
DOCBOOK START

FUNCTION panda_textdirection
PURPOSE specify the direction that the text flows within the document

SYNOPSIS START
#include&lt;panda/constants.h&gt;
#include&lt;panda/functions.h&gt;
void panda_textdirection (panda_pdf *document, int dir);
SYNOPSIS END

DESCRIPTION This function records information within the PDF indicating the direction that the text in the document flows in. The possible values for the dir arguement are: <command>panda_textdirection_l2r</command>, text is read left to right; <command>panda_textdirection_r2l</command>, text is read right to left. The default for this value is <command>panda_textdirection_l2r</command>.

RETURNS Nothing

EXAMPLE START
#include&lt;panda/constants.h&gt;
#include&lt;panda/functions.h&gt;

panda_pdf *document;

panda_init();

document = panda_open("filename.pdf", "w");
panda_textdirection(document, panda_textdirection_r2l);
EXAMPLE END
DOCBOOK END
******************************************************************************/

void
panda_textdirection (panda_pdf * document, int dir)
{
  switch (dir)
    {
    case panda_textdirection_l2r:
      panda_adddictitem (document, document->catalog,
			 "ViewerPreferences/Direction",
			 panda_textvalue, "L2R");
      break;

    case panda_textdirection_r2l:
      panda_adddictitem (document, document->catalog,
			 "ViewerPreferences/Direction",
			 panda_textvalue, "R2L");
      break;

    default:
      panda_error (panda_false, "Invalid text direction specified");
      break;
    }
}

/******************************************************************************
DOCBOOK START

FUNCTION panda_pageduration
PURPOSE specify the maximum number of seconds that a page should be displayed by the viewer before moving on

SYNOPSIS START
#include&lt;panda/constants.h&gt;
#include&lt;panda/functions.h&gt;
void panda_pageduration (panda_page *target, int seconds);
SYNOPSIS END

DESCRIPTION This function records information within the PDF indicating the maximum number of seconds that the given page should be displayed within the viewer. This is useful for presentations and the like where you might like to automatically move onto the next page in the document at some point. The default value for this is to never move onto the next page automatically. If this value is changed from the default, there is currently no way to revert back to the default later. The feature may not be implemented by all viewers.

RETURNS Nothing

EXAMPLE START
#include&lt;panda/constants.h&gt;
#include&lt;panda/functions.h&gt;

panda_pdf *document;
panda_page *page;

panda_init();

document = panda_open("filename.pdf", "w");
page = panda_newpage (document, panda_pagesize_a4);

panda_pageduration (page, 30.5);
EXAMPLE END
DOCBOOK END
******************************************************************************/

void
panda_pageduration (panda_pdf * document, panda_page * target, int duration)
{
  panda_adddictitem (document, target->obj, "Dur", panda_integervalue,
		     duration);
}

/******************************************************************************
DOCBOOK START

FUNCTION panda_transduration
PURPOSE specify the number of seconds that a page transition effect should take to occur

SYNOPSIS START
#include&lt;panda/constants.h&gt;
#include&lt;panda/functions.h&gt;
void panda_transduration (panda_page *target, double seconds);
SYNOPSIS END

DESCRIPTION This function records information within the PDF indicating the maximum number of seconds that the given page transition effect should be displayed within the viewer. This is useful for presentations and the like when you realise that you are addicted to Microsoft Powerpoint...

RETURNS Nothing

EXAMPLE START
#include&lt;panda/constants.h&gt;
#include&lt;panda/functions.h&gt;

panda_pdf *document;
panda_page *page;

panda_init();

document = panda_open("filename.pdf", "w");
page = panda_newpage (document, panda_pagesize_a4);

panda_transduration (page, 30.5);
EXAMPLE END
DOCBOOK END
******************************************************************************/

void
panda_transduration (panda_pdf * document, panda_page * target,
		     double duration)
{
  panda_adddictitem (document, target->obj, "D", panda_doublevalue, duration);
}

/******************************************************************************
DOCBOOK START

FUNCTION panda_transstyle
PURPOSE specify the type of page change transition that should occur

SYNOPSIS START
#include&lt;panda/constants.h&gt;
#include&lt;panda/functions.h&gt;
void panda_transstyle (panda_page *target, int style);
SYNOPSIS END

DESCRIPTION START
This function records information within the PDF indicating the preferred page transition style to use. The following are valid styles to use:
<orderedlist>
<listitem>panda_pagetrans_split_yi -- vertical split from the inside of the page</listitem>
<listitem>panda_pagetrans_split_yo -- vertical split from the outside of the page</listitem>
<listitem>panda_pagetrans_split_xi -- horizontal split from the inside of the page</listitem>
<listitem>panda_pagetrans_split_xo -- horizontal split from the outside of the page</listitem>
<listitem>panda_pagetrans_blinds_y -- vertical blinds effect</listitem>
<listitem>panda_pagetrans_blinds_x -- horizontal blinds effect</listitem>
<listitem>panda_pagetrans_box_i -- box expanding from the inside of the page</listitem>
<listitem>panda_pagetrans_box_o -- box contracting from the outside of the page</listitem>
<listitem>panda_pagetrans_wipe_0 -- a single line wipes the page away from the left to the right</listitem>
<listitem>panda_pagetrans_wipe_90 -- a single line wipes the page away from the bottom to the top</listitem>
<listitem>panda_pagetrans_wipe_180 -- a single line wipes the page away from the right to the left</listitem>
<listitem>panda_pagetrans_wipe_270 -- a single line wipes the page away from the top to the bottom</listitem>
<listitem>panda_pagetrans_dissolve -- the old page dissolves slowly into the new page</listitem>
<listitem>panda_pagetrans_glitter_0 -- a glitter effect that moves from the left to the right of the page</listitem>
<listitem>panda_pagetrans_glitter_270 -- a glitter effect that moves from the top to the bottom of the page</listitem>
<listitem>panda_pagetrans_glitter_315 -- a glitter effect that moves from the top left to the bottom right of the page</listitem>
<listitem>panda_pagetrans_none -- no transition effect</listitem>
</orderedlist>

</para>
<para>
The default transition is to have no transition at all. It should be noted that not all viewers support these transition effects.
DESCRIPTION END

RETURNS Nothing

EXAMPLE START
#include&lt;panda/constants.h&gt;
#include&lt;panda/functions.h&gt;

panda_pdf *document;
panda_page *page;

panda_init();

document = panda_open("filename.pdf", "w");
page = panda_newpage (document, panda_pagesize_a4);

panda_transduration (page, 30.5);
EXAMPLE END
DOCBOOK END
******************************************************************************/

void
panda_transstyle (panda_pdf * document, panda_page * target, int style)
{
#if defined DEBUG
  printf ("Defined transition style for object number %d\n",
	  target->obj->number);
#endif

  switch (style)
    {
    case panda_pagetrans_split_yi:
    case panda_pagetrans_split_yo:
    case panda_pagetrans_split_xi:
    case panda_pagetrans_split_xo:
      panda_adddictitem (document, target->obj, "S", panda_textvalue,
			 "Split");
      panda_adddictitem (document, target->obj, "Dm", panda_textvalue,
			 style < panda_pagetrans_split_xi ? "V" : "H");

      if ((style == panda_pagetrans_split_yi) ||
	  (style == panda_pagetrans_split_xi))
	{
	  panda_adddictitem (document, target->obj, "M", panda_textvalue,
			     "I");
	}
      else
	{
	  panda_adddictitem (document, target->obj, "M", panda_textvalue,
			     "O");
	}
      break;

    case panda_pagetrans_blinds_y:
    case panda_pagetrans_blinds_x:
      panda_adddictitem (document, target->obj, "S", panda_textvalue,
			 "Blinds");
      panda_adddictitem (document, target->obj, "Dm", panda_textvalue,
			 style == panda_pagetrans_blinds_y ? "V" : "H");
      break;

    case panda_pagetrans_box_i:
    case panda_pagetrans_box_o:
      panda_adddictitem (document, target->obj, "S", panda_textvalue, "Box");

      if (style == panda_pagetrans_box_i)
	{
	  panda_adddictitem (document, target->obj, "M", panda_textvalue,
			     "I");
	}
      else
	{
	  panda_adddictitem (document, target->obj, "M", panda_textvalue,
			     "O");
	}

      break;

    case panda_pagetrans_wipe_0:
    case panda_pagetrans_wipe_90:
    case panda_pagetrans_wipe_180:
    case panda_pagetrans_wipe_270:
      panda_adddictitem (document, target->obj, "S", panda_textvalue, "Wipe");

      switch (style)
	{
	case panda_pagetrans_wipe_0:
	  panda_adddictitem (document, target->obj, "Di", panda_integervalue,
			     0);
	  break;

	case panda_pagetrans_wipe_90:
	  panda_adddictitem (document, target->obj, "Di", panda_integervalue,
			     90);
	  break;

	case panda_pagetrans_wipe_180:
	  panda_adddictitem (document, target->obj, "Di", panda_integervalue,
			     180);
	  break;

	case panda_pagetrans_wipe_270:
	  panda_adddictitem (document, target->obj, "Di", panda_integervalue,
			     270);
	  break;
	}
      break;

    case panda_pagetrans_dissolve:
      panda_adddictitem (document, target->obj, "S", panda_textvalue,
			 "Dissolve");
      break;

    case panda_pagetrans_glitter_0:
    case panda_pagetrans_glitter_270:
    case panda_pagetrans_glitter_315:
      panda_adddictitem (document, target->obj, "S", panda_textvalue,
			 "Glitter");

      switch (style)
	{
	case panda_pagetrans_glitter_0:
	  panda_adddictitem (document, target->obj, "Di", panda_integervalue,
			     0);
	  break;

	case panda_pagetrans_glitter_270:
	  panda_adddictitem (document, target->obj, "Di", panda_integervalue,
			     270);
	  break;

	case panda_pagetrans_glitter_315:
	  panda_adddictitem (document, target->obj, "Di", panda_integervalue,
			     315);
	  break;
	}
      break;

    case panda_pagetrans_none:
      panda_adddictitem (document, target->obj, "S", panda_textvalue, "R");
      break;

    default:
      panda_error (panda_false, "Unknown page transition requested");
    }
}
