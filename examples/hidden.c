#include  <panda/functions.h>
#include  <panda/constants.h>

int main(int argc, char *argv[])    {
	panda_pdf *demo;

	panda_page *currPage, *templatePage;

	// Initialise the library
	panda_init();

	if ((demo = panda_open("hidden.pdf", "w")) == NULL)
		panda_error(panda_true, "could not open hidden.pdf");

	panda_setproperty(demo->pages, panda_scope_cascade, panda_object_property_compress, panda_true);
	panda_setproperty(demo->pages, panda_scope_cascade, panda_object_property_compress_level, 9);


	// Create a page
	currPage = panda_newpage(demo, panda_pagesize_a4);
	int h = currPage->height;
	int w = currPage->width;

	//int iw, ih;
	//panda_imagesize (&iw, &ih, "page.tif", panda_image_tiff);
	panda_imagebox(demo, currPage, 0, 0, h, w, "images/input.tif", panda_image_tiff);

	//printf("image WxH: %i x %i     page WxH: %i x %i   \n", iw, ih, w, h);
	
	panda_setfontmode(demo, panda_textmode_invisible);

	panda_textbox(demo, currPage,	100, 	w/2-100,	150, 	w/2+100, "top");
	panda_textbox(demo, currPage,	h/2, 	w/2-100,	h/2, 	w/2+100, "center");
	panda_textbox(demo, currPage,	h-100, 	w/2-100,	h-50, 	w/2+100, "bottom");
	panda_setfontmode(demo, panda_textmode_normal);

	panda_close(demo);
	return 0;
}

