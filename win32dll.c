/******************************************************************************
  Win32dll.c

  Change Control:                                                      DDMMYYYY
    Michael Still    File created                                      24072000

  Purpose:
    This file is used to export the relevant symbols for a win32 dll of
    the Panda library. It simply exports the names of the functions that
    I think should be accessible to the calling application. This file,
    therefore, does not include any exports for the internal functions
    of the library such as panda_putc(...)

    If this is a major problem, then feel free to add these function calls
    to this file, but I cannot think of a good reason to do so.
******************************************************************************/

#include <windows.h>
#include <panda/constants.h>
#include <panda/functions.h>

// We should have a callback for panda_errors setup -- this is not currently
// implemented in the library, but we will put just enough here to make it
// work later...
typedef void (__stdcall * pandaErrorCallback) (BSTR errorString);
pandaErrorCallback panda_errorCB = NULL;

// Function exports
_declspec (dllexport)
     void __stdcall winpanda_init (void)
{
  panda_init ();
}

_declspec (dllexport)
     panda_pdf *__stdcall winpanda_open (char *filename, char *mode)
{
  return panda_open (filename, mode);
}

_declspec (dllexport)
     void __stdcall winpanda_close (panda_pdf * document)
{
  panda_close (document);
}

_declspec (dllexport)
     panda_page *__stdcall winpanda_page (panda_pdf * document, char *size)
{
  return panda_page (document, size);
}

_declspec (dllexport)
     void __stdcall winpanda_setfontsize (panda_pdf * document, int size)
{
  panda_setfontsize (document, size);
}

_declspec (dllexport)
     void __stdcall winpanda_setfontmode (panda_pdf * document, int size)
{
  panda_setfontmode (document, size);
}

_declspec (dllexport)
     void __stdcall winpanda_setcharacterspacing (panda_pdf * document,
						  double amount)
{
  panda_setcharacterspacing (document, amount);
}

_declspec (dllexport)
     void __stdcall winpanda_setwordspacing (panda_pdf * document,
					     double amount)
{
  panda_setwordspacing (document, amount);
}

_declspec (dllexport)
     void __stdcall winpanda_sethorizontalscaling (panda_pdf * document,
						   double scaling)
{
  panda_sethorizontalscaling (document, scaling);
}

_declspec (dllexport)
     void __stdcall winpanda_setleading (panda_pdf * document, double amount)
{
  panda_setleading (document, amount);
}
