/******************************************************************************
  Template.c

  Change Control:                                                      DDMMYYYY
    Michael Still    File created                                      10012001

  Purpose:
    PDF allows you to define a template for a set of pages, which contains all
    the elements you can have on a normal page. This makes storage of many,
    mainly identical, pages much more efficient. This is all discussed on page
    264 of the PDF Spec 1.3, second edition.
******************************************************************************/

#include <panda/constants.h>
#include <panda/functions.h>
#include <math.h>

// Here the user creates a dummy page which they can make look nice and then
// use as a template all over the place. Templates are a bit different from
// pages in that the content stream is owned by the page as well...
panda_page *
panda_newtemplate (panda_pdf * output, char *pageSize)
{
  panda_page *template;
  panda_object *xobjrefsubdict;

  // Make some space for the object
  template = (panda_page *) panda_xmalloc (sizeof (panda_page));
  template->obj = (panda_object *) panda_newobject (output, panda_normal);
  template->obj->isTemplate = panda_true;

  // Add it to the object tree
  panda_addchild (output->pages, template->obj);

  // We also need to do the same sort of thing for the contents object
  // that each page owns
  template->contents = template->obj;  
  template->contents->isContents = panda_true;

  // Setup some stuff in the contents object that we need later
  template->contents->textmode = panda_false;
  template->contents->insidegraphicsblock = panda_false;

  // Add the required dictionary elements for a template page
  panda_adddictitem (template->obj->dict, "Type", panda_textvalue, "XObject");
  panda_adddictitem (template->obj->dict, "Subtype", panda_textvalue, "Form");
  panda_adddictitem (template->obj->dict, "FormType", panda_integervalue, 1);
  panda_adddictitem (template->obj->dict, "BBox", panda_literaltextvalue,
		     pageSize);
  panda_adddictitem (template->obj->dict, "Matrix", panda_literaltextvalue,
		     "[1 0 0 1 0 0]");

  // We make an object not just a dictionary because this is what
  // adddictitem needs
  xobjrefsubdict = (panda_object *) panda_newobject (output,
                                                     panda_placeholder);
  panda_adddictitem (xobjrefsubdict->dict, "ProcSet", panda_literaltextvalue,
		     "[/PDF]");

  // And put this into the PDF
  panda_adddictitem (template->obj->dict, "Resources", panda_dictionaryvalue,
                     xobjrefsubdict->dict);

  // There is a pdf wide name that we use in resources sections to refer to
  // this template
  template->templatename = panda_xsnprintf("template%d%d", 
					   template->obj->number,
					   template->obj->generation);

  return template;
}

// Use the template on this page
void
panda_applytemplate(panda_pdf *output, panda_page *target, 
		    panda_page *template)
{
  panda_object *xobjrefsubdict, *xobjrefsubsubdict;
  double angle = 0;
  int left = 0, right = 200, top = 0, bottom = 200;

  // We also need to add some information to the layout stream for the contents
  // object for the page that the image is being displayed on. This information
  // consists of the following [moved to internal.c]
  panda_entergraphicsmode (target);
  
/*    target->contents->layoutstream = */
/*      panda_streamprintf (target->contents->layoutstream, */
/*                          "\n%.2f %.2f %.2f %.2f %.2f %.2f cm\n", */
/*  			cos (angle * panda_pi / 180.0), // x scale */
/*                          sin (angle * panda_pi / 180.0), // rotate and scale */
/*                          -sin (angle * panda_pi / 180.0),        // ??? */
/*                          cos (angle * panda_pi / 180.0), // y scale */
/*                          (double) left,  // x start */
/*                          (double) target->height - bottom);      // y start */

/*    target->contents->layoutstream = */
/*      panda_streamprintf (target->contents->layoutstream, */
/*                          "%.2f %.2f %.2f %.2f %.2f %.2f cm\n", */
/*                          // The second matrix */
/*                          (double) (right - left),        // x size */
/*                          0.0,    // ??? */
/*                          0.0,    // ??? */
/*                          (double) (bottom - top),        // y size */
/*                          0.0,    // ??? */
/*                          0.0);   // ??? */

  target->contents->layoutstream =
    panda_streamprintf (target->contents->layoutstream, "/%s Do\n",
                        template->templatename);

  panda_exitgraphicsmode (target);

  // We make an object not just a dictionary because this is what
  // adddictitem needs
  xobjrefsubsubdict = (panda_object *) panda_newobject (output,
                                                        panda_placeholder);
  panda_adddictitem (xobjrefsubsubdict->dict, template->templatename, 
		     panda_objectvalue, template->obj);

  xobjrefsubdict = (panda_object *) panda_newobject (output,
                                                     panda_placeholder);
  panda_adddictitem (xobjrefsubdict->dict, "XObject", panda_dictionaryvalue,
                     xobjrefsubsubdict->dict);

  // And put this into the PDF
  panda_adddictitem (target->obj->dict, "Resources", panda_dictionaryvalue,
                     xobjrefsubdict->dict);
}
