/******************************************************************************
  Windows.c

  Change Control:                                                      DDMMYYYY
    Michael Still    File created                                      22062001

  Purpose:
    I don't want to have to pass pointers to structures into VB land because
	this seems to be non-trivial and prone to breakage. This file therefore
	provides a series of abstractions to allow these pointers to be avoided.
******************************************************************************/

#if defined _WINDOWS
#include "panda/constants.h"
#include "panda/functions.h"
 windows_panda_abs abstractions[windows_panda_abs_max];

// Setup the windows stuff ready for use
  void __stdcall
windows_panda_init ()
{
  int count;
  for (count = 0; count < windows_panda_abs_max; count++)
    {
      abstractions[count].list = NULL;
      abstractions[count].highest = 0;
    }
  panda_init ();
}
int __stdcall
windows_panda_test (char *filename, char *mode)
{
  void *one, *two, *three;
  int numbers[3];
  one = panda_xmalloc (sizeof (int));
  two = panda_xmalloc (sizeof (int));
  three = panda_xmalloc (sizeof (int));
  numbers[0] = windows_panda_reabs (0, one);
  numbers[1] = windows_panda_reabs (0, two);
  numbers[2] = windows_panda_reabs (0, three);
  if (one != windows_panda_deabs (0, numbers[0]))
    return 1;
  if (two != windows_panda_deabs (0, numbers[1]))
    return 1;
  if (three != windows_panda_deabs (0, numbers[2]))
    return 1;
  return 0;
}


// -------------- curves.c
  void __stdcall
windows_panda_setlinestart (int page, int x, int y)
{
  panda_setlinestart (windows_panda_deabs (windows_panda_abs_page, page), x,
		       y);
}
void __stdcall
windows_panda_addlinesegment (int page, int x, int y)
{
  panda_addlinesegment (windows_panda_deabs (windows_panda_abs_page, page),
			 x, y);
}
void __stdcall
windows_panda_addcubiccurvesegment (int page, int x, int y, int cx1, int cy1,
				    int cx2, int cy2)
{
  panda_addcubiccurvesegment (windows_panda_deabs
			       (windows_panda_abs_page, page), x, y, cx1, cy1,
			       cx2, cy2);
}
void __stdcall
windows_panda_addquadraticcurvesegmentone (int page, int x, int y, int cx,
					   int cy)
{
  panda_addquadraticcurvesegmentone (windows_panda_deabs
				      (windows_panda_abs_page, page), x, y,
				      cx, cy);
}
void __stdcall
windows_panda_addquadraticcurvesegmenttwo (int page, int x, int y, int cx,
					   int cy)
{
  panda_addquadraticcurvesegmenttwo (windows_panda_deabs
				      (windows_panda_abs_page, page), x, y,
				      cx, cy);
}
void __stdcall
windows_panda_closeline (int page)
{
  panda_closeline (windows_panda_deabs (windows_panda_abs_page, page));
}
void __stdcall
windows_panda_rectangle (int page, int top, int left, int bottom, int right)
{
  panda_rectangle (windows_panda_deabs (windows_panda_abs_page, page), top,
		    left, bottom, right);
}
void __stdcall
windows_panda_endline (int page)
{
  panda_endline (windows_panda_deabs (windows_panda_abs_page, page));
}
void __stdcall
windows_panda_strokeline (int page)
{
  panda_strokeline (windows_panda_deabs (windows_panda_abs_page, page));
}
void __stdcall
windows_panda_fillline (int page)
{
  panda_fillline (windows_panda_deabs (windows_panda_abs_page, page));
}
void __stdcall
windows_panda_setlinewidth (int page, int width)
{
  panda_setlinewidth (windows_panda_deabs (windows_panda_abs_page, page),
		       width);
}
void __stdcall
windows_panda_setlinecap (int page, int cap)
{
  panda_setlinecap (windows_panda_deabs (windows_panda_abs_page, page), cap);
}
void __stdcall
windows_panda_setlinejoin (int page, int join)
{
  panda_setlinejoin (windows_panda_deabs (windows_panda_abs_page, page),
		      join);
}
void __stdcall
windows_panda_setlinedash (int page, int on, int off, int phase)
{
  panda_setlinedash (windows_panda_deabs (windows_panda_abs_page, page), on,
		      off, phase);
}
void __stdcall
windows_panda_setfillcolor (int page, int red, int green, int blue)
{
  panda_setfillcolor (windows_panda_deabs (windows_panda_abs_page, page),
		       red, green, blue);
}
void __stdcall
windows_panda_setlinecolor (int page, int red, int green, int blue)
{
  panda_setlinecolor (windows_panda_deabs (windows_panda_abs_page, page),
		       red, green, blue);
}


// -------------- images.c
  void __stdcall
windows_panda_imagebox (int pdf, int page, int top, int left, int bottom,
			int right, char *filename, int imgtype)
{
  panda_imagebox (windows_panda_deabs (windows_panda_abs_pdf, pdf),
		   windows_panda_deabs (windows_panda_abs_page, page), top,
		   left, bottom, right, filename, imgtype);
}
void __stdcall
windows_panda_imageboxrot (int pdf, int page, int top, int left, int bottom,
			   int right, double angle, char *filename,
			   int imgtype)
{
  panda_imageboxrot (windows_panda_deabs (windows_panda_abs_pdf, pdf),
		      windows_panda_deabs (windows_panda_abs_page, page),
		      top, left, bottom, right, angle, filename, imgtype);
}


// -------------- info.c
  void __stdcall
windows_panda_setauthor (int pdf, char *string)
{
  panda_setauthor (windows_panda_deabs (windows_panda_abs_pdf, pdf), string);
}
void __stdcall
windows_panda_setcreator (int pdf, char *string)
{
  panda_setcreator (windows_panda_deabs (windows_panda_abs_pdf, pdf),
		     string);
}
void __stdcall
windows_panda_settitle (int pdf, char *string)
{
  panda_settitle (windows_panda_deabs (windows_panda_abs_pdf, pdf), string);
}
void __stdcall
windows_panda_setsubject (int pdf, char *string)
{
  panda_setsubject (windows_panda_deabs (windows_panda_abs_pdf, pdf),
		     string);
}
void __stdcall
windows_panda_setkeywords (int pdf, char *string)
{
  panda_setkeywords (windows_panda_deabs (windows_panda_abs_pdf, pdf),
		      string);
}


// -------------- font.c
  int __stdcall
windows_panda_createfont (int pdf, char *name, int fonttype, char *encoding)
{
  return windows_panda_reabs (windows_panda_abs_font,
			       panda_createfont (windows_panda_deabs
						  (windows_panda_abs_pdf,
						   pdf), name, fonttype,
						  encoding));
}
void __stdcall
windows_panda_setfont (int pdf, int ident)
{
  panda_setfont (windows_panda_deabs (windows_panda_abs_pdf, pdf),
		  windows_panda_deabs (windows_panda_abs_font, ident));
}
void __stdcall
windows_panda_setfontsize (int pdf, int size)
{
  panda_setfontsize (windows_panda_deabs (windows_panda_abs_pdf, pdf), size);
}
void __stdcall
windows_panda_setfontmode (int pdf, int mode)
{
  panda_setfontmode (windows_panda_deabs (windows_panda_abs_pdf, pdf), mode);
}
void __stdcall
windows_panda_setcharacterspacing (int pdf, double spacing)
{
  panda_setcharacterspacing (windows_panda_deabs
			      (windows_panda_abs_pdf, pdf), spacing);
}
void __stdcall
windows_panda_setwordspacing (int pdf, double spacing)
{
  panda_setwordspacing (windows_panda_deabs (windows_panda_abs_pdf, pdf),
			 spacing);
}
void __stdcall
windows_panda_sethorizontalscaling (int pdf, double scaling)
{
  panda_sethorizontalscaling (windows_panda_deabs
			       (windows_panda_abs_pdf, pdf), scaling);
}
void __stdcall
windows_panda_setleading (int pdf, double leading)
{
  panda_setleading (windows_panda_deabs (windows_panda_abs_pdf, pdf),
		     leading);
}


// -------------- panda.c
  int __stdcall
windows_panda_open (char *filename, char *mode)
{
  return windows_panda_reabs (windows_panda_abs_pdf,
			       panda_open (filename, mode));
}
int __stdcall
windows_panda_newpage (int pdf, char *size)
{
  return windows_panda_reabs (windows_panda_abs_page,
			       panda_newpage (windows_panda_deabs
					       (windows_panda_abs_pdf, pdf),
					       size));
}
void __stdcall
windows_panda_close (int pdf)
{
  panda_close (windows_panda_deabs (windows_panda_abs_pdf, pdf));
}


// -------------- template.c
  int __stdcall
windows_panda_newtemplate (int pdf, char *size)
{
  return windows_panda_reabs (windows_panda_abs_page,
			       panda_newtemplate (windows_panda_deabs
						   (windows_panda_abs_pdf,
						    pdf), size));
}
void __stdcall
windows_panda_applytemplate (int pdf, int page1, int page2)
{
  panda_applytemplate (windows_panda_deabs (windows_panda_abs_pdf, pdf),
			windows_panda_deabs (windows_panda_abs_page, page1),
			windows_panda_deabs (windows_panda_abs_page, page2));
}


// -------------- text.c
  void __stdcall
windows_panda_textbox (int pdf, int page, int top, int left, int bottom,
		       int right, char *text)
{
  panda_textbox (windows_panda_deabs (windows_panda_abs_pdf, pdf),
		  windows_panda_deabs (windows_panda_abs_page, page), top,
		  left, bottom, right, text);
}


/******************************************************************************
  INTERNAL STUFF BELOW THIS LINE
******************************************************************************/ 
  
// This function's role is to lookup the magic token you gave me (with a type),
// and use that to determine the pointer that should be used and then use it...
void *
windows_panda_deabs (int absarea, int number)
{
  windows_panda_ptrlist * current;
  
    // Is the area outside the allowable range?
    if (absarea >= windows_panda_abs_max)
    panda_error (panda_true, "Area out of reach");
  
    // Has the area had any data put into it yet?
    if (abstractions[absarea].list == NULL)
    panda_error (panda_true, "Area empty");
  
    // Now we need to go through looking for the number specified
    current = abstractions[absarea].list;
  while ((current != NULL) && (current->number != number))
    current = current->next;
  
    // current being null means we hit the end of the list without finding anything
    if (current == NULL)
    panda_error (panda_true, "Item not found in area");
  
    // We found it!
    return current->me;
}


// This function does the opposite of above, you give me a pointer, and I hive it
// away to be returned later
  int
windows_panda_reabs (int absarea, void *ptr)
{
  windows_panda_ptrlist * current;
  
    // Is the area outside the allowable range?
    if (absarea >= windows_panda_abs_max)
    panda_error (panda_true, "Area out of reach");
  
    // Has the area had any data put into it yet?
    if (abstractions[absarea].list == NULL)
    {
      
	// No, we better do some setup
	abstractions[absarea].list =
	panda_xmalloc (sizeof (windows_panda_ptrlist));
      abstractions[absarea].list->next = NULL;
    }
  
    // Find the end of the list
    current = abstractions[absarea].list;
  while (current->next != NULL)
    current = current->next;
  
    // Now we can store the pointer
    current->next = panda_xmalloc (sizeof (windows_panda_ptrlist));
  current = current->next;
  current->next = NULL;
  current->me = ptr;
  current->number = abstractions[absarea].highest++;
  
    // We found it!
    return current->number;
}
#endif	/*  */
