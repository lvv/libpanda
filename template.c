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

// Internal call to create a template once requested via panda_newpage
panda_page *panda_newtemplate(panda_pdf *output, char *pageSize){
  panda_object *template;

  // Make a new page and object
  template = (page *) panda_xmalloc(sizeof(panda_page));
  template->obj = (object *) panda_xmalloc(sizeof(panda_object));

  // Add the required dictionary elements for a template page
  panda_adddictitem(template->obj->dict, "Name", gTextValue, "XObject");
  panda_adddictitem(template->obj->dict, "Subtype", gTextValue, "Form");
  panda_adddictitem(template->obj->dict, "FormType", gIntValue, 1);
  panda_adddictitem(template->obj->dict, "BBox", gTextValue, pageSize);
  
  // Make sure it is written out at some point
  panda_addchild(output->contents, template->obj);

