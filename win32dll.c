/******************************************************************************
  Win32dll.c

  Change Control:                                                      DDMMYYYY
    Michael Still    File created                                      24072000

  Purpose:
    This file is used to export the relevant symbols for a win32 dll of
    the Panda library. It simply exports the names of the functions that
    I think should be accessible to the calling application. This file,
    therefore, does not include any exports for the internal functions
    of the library such as pdfputc(...)

    If this is a major problem, then feel free to add these function calls
    to this file, but I cannot think of a good reason to do so.
******************************************************************************/

#include <windows.h>
#include "constants.h"
#include "functions.h"

// We should have a callback for errors setup -- this is not currently
// implemented in the library, but we will put just enough here to make it
// work later...
typedef void (__stdcall *pandaErrorCallback) (BSTR errorString);
pandaErrorCallback errorCB = NULL;

// Function exports
_declspec(dllexport) void __stdcall wininitpanda(void){
  initpanda();
}

_declspec(dllexport) pdf * __stdcall winpdfopen(char *filename, char *mode){
  return pdfopen(filename, mode);
}

_declspec(dllexport) void __stdcall winpdfclose(pdf *document){
  pdfclose(document);
}

_declspec(dllexport) page * __stdcall winpdfpage(pdf *document, char *size){
  return pdfpage(document, size);
}

_declspec(dllexport) void __stdcall winsetfontsize(pdf *document, int size){
  setfontsize(document, size);
}

_declspec(dllexport) void __stdcall winsetfontmode(pdf *document, int size){
  setfontmode(document, size);
}

_declspec(dllexport) void __stdcall winsetcharacterspacing(pdf *document, double amount){
  setcharacterspacing(document, amount);
}

_declspec(dllexport) void __stdcall winsetwordspacing(pdf *document, double amount){
  setwordspacing(document, amount);
}

_declspec(dllexport) void __stdcall winsethorizontalscaling(pdf *document, double scaling){
  sethorizontalscaling(document, scaling);
}

_declspec(dllexport) void __stdcall winsetleading(pdf *document, double amount){
  setleading(document, amount);
}
