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
panda_page *page;

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
panda_page *page;

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
panda_page *page;

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
