/******************************************************************************
  View.c

  Change Control:                                                      DDMMYYYY
    Michael Still    File created                                      08012002

  Purpose:
    Despite the name, this file has nothing to do with viewing PDF documents.
    If you are looking for a viewer, then I recommend gv...

    This file is responsible for the output of parts of the PDF which control
    the user's viewing experience.
******************************************************************************/

#if defined _WINDOWS
  #include "panda/constants.h"
  #include "panda/functions.h"
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

void panda_hidetoolbar(panda_pdf *document, int onoff)
{
  panda_adddictitem(document->viewerPrefs->dict, "HideToolbar", 
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

void panda_hidemenubar(panda_pdf *document, int onoff)
{
  panda_adddictitem(document->viewerPrefs->dict, "HideMenubar", 
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

void panda_hidewindowui(panda_pdf *document, int onoff)
{
  panda_adddictitem(document->viewerPrefs->dict, "HideWindowUI", 
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

void panda_fitwindow(panda_pdf *document, int onoff)
{
  panda_adddictitem(document->viewerPrefs->dict, "FitWindow", 
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

void panda_centerwindow(panda_pdf *document, int onoff)
{
  panda_adddictitem(document->viewerPrefs->dict, "CenterWindow", 
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

void panda_nfspagemode(panda_pdf *document, int pagemode)
{
  switch(pagemode)
    {
    case panda_window_usenone:
      panda_adddictitem(document->viewerPrefs->dict, "NonFullScreenPageMode", 
			panda_textvalue, "UseNone");
      break;

    case panda_window_useoutlines:
      panda_adddictitem(document->viewerPrefs->dict, "NonFullScreenPageMode", 
			panda_textvalue, "UseOutlines");
      break;

    case panda_window_usethumbs:
      panda_adddictitem(document->viewerPrefs->dict, "NonFullScreenPageMode", 
			panda_textvalue, "UseThumbs");
      break;

    default:
      panda_error(panda_false, "Attempt to set a value for the non fullscreen page mode which is invalid");
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

void panda_fullscreen(panda_pdf *document, int onoff)
{
  // todo_mikal

  if(onoff == panda_true)
    panda_adddictitem(document->catalog->dict, "PageMode", 
		      panda_textvalue, "FullScreen");
  else
    panda_adddictitem(document->catalog->dict, "PageMode", 
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

void panda_textdirection(panda_pdf *document, int dir)
{
  switch(dir)
    {
    case panda_textdirection_l2r:
      panda_adddictitem(document->viewerPrefs->dict, "Direction", 
			panda_textvalue, "L2R");
      break;

    case panda_textdirection_r2l:
      panda_adddictitem(document->viewerPrefs->dict, "Direction", 
			panda_textvalue, "R2L");
      break;

    default:
      panda_error(panda_false, "Invalid text direction specified");
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
void panda_pageduration (panda_page *target, double seconds);
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

void panda_pageduration(panda_page *target, double duration)
{
  panda_adddictitem(target->obj->dict, "Dur", panda_doublevalue, duration);
}
