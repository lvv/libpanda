#include  <panda/functions.h>
#include  <panda/constants.h>

int main(int argc, char *argv[]){
  pdf    *demo;
  page   *currPage;
  char   filename[1024];

  // Initialise the library
  initpanda();

  // Open our demo PDF
  if((demo = pdfopen("output.pdf", "w")) == NULL)
    error("demo: could not open output.pdf to write to.");

    // Create a page
    currPage = pdfpage(demo, gPageSizeA4);

    // Insert some images
    sprintf(filename, "%s",





"a9746950-1.ss.tif"
    );

    imagebox(demo, currPage, 0, 0, currPage->height / 2, currPage->width, 
      filename, gImageTiff);

    textbox(demo, currPage, 20, 20, 100, 30, filename);


    // Create a page
    currPage = pdfpage(demo, gPageSizeA4);

    // Insert some images
    sprintf(filename, "%s",





"a9746950-1.tif"
    );

    imagebox(demo, currPage, 0, 0, currPage->height / 2, currPage->width, 
      filename, gImageTiff);

    textbox(demo, currPage, 20, 20, 100, 30, filename);


    // Create a page
    currPage = pdfpage(demo, gPageSizeA4);

    // Insert some images
    sprintf(filename, "%s",





"a9746974-1.tif"
    );

    imagebox(demo, currPage, 0, 0, currPage->height / 2, currPage->width, 
      filename, gImageTiff);

    textbox(demo, currPage, 20, 20, 100, 30, filename);


    // Create a page
    currPage = pdfpage(demo, gPageSizeA4);

    // Insert some images
    sprintf(filename, "%s",





"a9747190-1.tif"
    );

    imagebox(demo, currPage, 0, 0, currPage->height / 2, currPage->width, 
      filename, gImageTiff);

    textbox(demo, currPage, 20, 20, 100, 30, filename);


    // Create a page
    currPage = pdfpage(demo, gPageSizeA4);

    // Insert some images
    sprintf(filename, "%s",





"a9747196-1.tif"
    );

    imagebox(demo, currPage, 0, 0, currPage->height / 2, currPage->width, 
      filename, gImageTiff);

    textbox(demo, currPage, 20, 20, 100, 30, filename);


    // Create a page
    currPage = pdfpage(demo, gPageSizeA4);

    // Insert some images
    sprintf(filename, "%s",





"a9747263-1.tif"
    );

    imagebox(demo, currPage, 0, 0, currPage->height / 2, currPage->width, 
      filename, gImageTiff);

    textbox(demo, currPage, 20, 20, 100, 30, filename);


    // Create a page
    currPage = pdfpage(demo, gPageSizeA4);

    // Insert some images
    sprintf(filename, "%s",





"a9747356-1.tif"
    );

    imagebox(demo, currPage, 0, 0, currPage->height / 2, currPage->width, 
      filename, gImageTiff);

    textbox(demo, currPage, 20, 20, 100, 30, filename);


    // Create a page
    currPage = pdfpage(demo, gPageSizeA4);

    // Insert some images
    sprintf(filename, "%s",





"a9747676-1.tif"
    );

    imagebox(demo, currPage, 0, 0, currPage->height / 2, currPage->width, 
      filename, gImageTiff);

    textbox(demo, currPage, 20, 20, 100, 30, filename);


    // Create a page
    currPage = pdfpage(demo, gPageSizeA4);

    // Insert some images
    sprintf(filename, "%s",





"a9747696-1.tif"
    );

    imagebox(demo, currPage, 0, 0, currPage->height / 2, currPage->width, 
      filename, gImageTiff);

    textbox(demo, currPage, 20, 20, 100, 30, filename);


    // Create a page
    currPage = pdfpage(demo, gPageSizeA4);

    // Insert some images
    sprintf(filename, "%s",





"a9747697-1.tif"
    );

    imagebox(demo, currPage, 0, 0, currPage->height / 2, currPage->width, 
      filename, gImageTiff);

    textbox(demo, currPage, 20, 20, 100, 30, filename);


    // Create a page
    currPage = pdfpage(demo, gPageSizeA4);

    // Insert some images
    sprintf(filename, "%s",





"a9747698-1.tif"
    );

    imagebox(demo, currPage, 0, 0, currPage->height / 2, currPage->width, 
      filename, gImageTiff);

    textbox(demo, currPage, 20, 20, 100, 30, filename);


    // Create a page
    currPage = pdfpage(demo, gPageSizeA4);

    // Insert some images
    sprintf(filename, "%s",





"a9747828-1.tif"
    );

    imagebox(demo, currPage, 0, 0, currPage->height / 2, currPage->width, 
      filename, gImageTiff);

    textbox(demo, currPage, 20, 20, 100, 30, filename);


    // Create a page
    currPage = pdfpage(demo, gPageSizeA4);

    // Insert some images
    sprintf(filename, "%s",





"a9747902-1.tif"
    );

    imagebox(demo, currPage, 0, 0, currPage->height / 2, currPage->width, 
      filename, gImageTiff);

    textbox(demo, currPage, 20, 20, 100, 30, filename);


    // Create a page
    currPage = pdfpage(demo, gPageSizeA4);

    // Insert some images
    sprintf(filename, "%s",





"a9747913-1.tif"
    );

    imagebox(demo, currPage, 0, 0, currPage->height / 2, currPage->width, 
      filename, gImageTiff);

    textbox(demo, currPage, 20, 20, 100, 30, filename);


    // Create a page
    currPage = pdfpage(demo, gPageSizeA4);

    // Insert some images
    sprintf(filename, "%s",





"a9747936-1.tif"
    );

    imagebox(demo, currPage, 0, 0, currPage->height / 2, currPage->width, 
      filename, gImageTiff);

    textbox(demo, currPage, 20, 20, 100, 30, filename);


    // Create a page
    currPage = pdfpage(demo, gPageSizeA4);

    // Insert some images
    sprintf(filename, "%s",





"a9747937-1.tif"
    );

    imagebox(demo, currPage, 0, 0, currPage->height / 2, currPage->width, 
      filename, gImageTiff);

    textbox(demo, currPage, 20, 20, 100, 30, filename);


    // Create a page
    currPage = pdfpage(demo, gPageSizeA4);

    // Insert some images
    sprintf(filename, "%s",





"a9747943-1.tif"
    );

    imagebox(demo, currPage, 0, 0, currPage->height / 2, currPage->width, 
      filename, gImageTiff);

    textbox(demo, currPage, 20, 20, 100, 30, filename);


    // Create a page
    currPage = pdfpage(demo, gPageSizeA4);

    // Insert some images
    sprintf(filename, "%s",





"a9748604-1.tif"
    );

    imagebox(demo, currPage, 0, 0, currPage->height / 2, currPage->width, 
      filename, gImageTiff);

    textbox(demo, currPage, 20, 20, 100, 30, filename);


    // Create a page
    currPage = pdfpage(demo, gPageSizeA4);

    // Insert some images
    sprintf(filename, "%s",





"a9748687-1.tif"
    );

    imagebox(demo, currPage, 0, 0, currPage->height / 2, currPage->width, 
      filename, gImageTiff);

    textbox(demo, currPage, 20, 20, 100, 30, filename);


    // Create a page
    currPage = pdfpage(demo, gPageSizeA4);

    // Insert some images
    sprintf(filename, "%s",





"a9748688-1.tif"
    );

    imagebox(demo, currPage, 0, 0, currPage->height / 2, currPage->width, 
      filename, gImageTiff);

    textbox(demo, currPage, 20, 20, 100, 30, filename);


    // Create a page
    currPage = pdfpage(demo, gPageSizeA4);

    // Insert some images
    sprintf(filename, "%s",





"a9748704-1.tif"
    );

    imagebox(demo, currPage, 0, 0, currPage->height / 2, currPage->width, 
      filename, gImageTiff);

    textbox(demo, currPage, 20, 20, 100, 30, filename);


    // Create a page
    currPage = pdfpage(demo, gPageSizeA4);

    // Insert some images
    sprintf(filename, "%s",





"a9748743-1.tif"
    );

    imagebox(demo, currPage, 0, 0, currPage->height / 2, currPage->width, 
      filename, gImageTiff);

    textbox(demo, currPage, 20, 20, 100, 30, filename);


    // Create a page
    currPage = pdfpage(demo, gPageSizeA4);

    // Insert some images
    sprintf(filename, "%s",





"a9748824-1.tif"
    );

    imagebox(demo, currPage, 0, 0, currPage->height / 2, currPage->width, 
      filename, gImageTiff);

    textbox(demo, currPage, 20, 20, 100, 30, filename);


    // Create a page
    currPage = pdfpage(demo, gPageSizeA4);

    // Insert some images
    sprintf(filename, "%s",





"a9748827-1.tif"
    );

    imagebox(demo, currPage, 0, 0, currPage->height / 2, currPage->width, 
      filename, gImageTiff);

    textbox(demo, currPage, 20, 20, 100, 30, filename);


    // Create a page
    currPage = pdfpage(demo, gPageSizeA4);

    // Insert some images
    sprintf(filename, "%s",





"a9748837-1.tif"
    );

    imagebox(demo, currPage, 0, 0, currPage->height / 2, currPage->width, 
      filename, gImageTiff);

    textbox(demo, currPage, 20, 20, 100, 30, filename);


    // Create a page
    currPage = pdfpage(demo, gPageSizeA4);

    // Insert some images
    sprintf(filename, "%s",





"a9748851-1.tif"
    );

    imagebox(demo, currPage, 0, 0, currPage->height / 2, currPage->width, 
      filename, gImageTiff);

    textbox(demo, currPage, 20, 20, 100, 30, filename);


    // Create a page
    currPage = pdfpage(demo, gPageSizeA4);

    // Insert some images
    sprintf(filename, "%s",





"a9749198-1.tif"
    );

    imagebox(demo, currPage, 0, 0, currPage->height / 2, currPage->width, 
      filename, gImageTiff);

    textbox(demo, currPage, 20, 20, 100, 30, filename);


    // Create a page
    currPage = pdfpage(demo, gPageSizeA4);

    // Insert some images
    sprintf(filename, "%s",





"a9749347-1.tif"
    );

    imagebox(demo, currPage, 0, 0, currPage->height / 2, currPage->width, 
      filename, gImageTiff);

    textbox(demo, currPage, 20, 20, 100, 30, filename);


    // Create a page
    currPage = pdfpage(demo, gPageSizeA4);

    // Insert some images
    sprintf(filename, "%s",





"a9749362-1.tif"
    );

    imagebox(demo, currPage, 0, 0, currPage->height / 2, currPage->width, 
      filename, gImageTiff);

    textbox(demo, currPage, 20, 20, 100, 30, filename);


    // Create a page
    currPage = pdfpage(demo, gPageSizeA4);

    // Insert some images
    sprintf(filename, "%s",





"a9749371-1.tif"
    );

    imagebox(demo, currPage, 0, 0, currPage->height / 2, currPage->width, 
      filename, gImageTiff);

    textbox(demo, currPage, 20, 20, 100, 30, filename);


    // Create a page
    currPage = pdfpage(demo, gPageSizeA4);

    // Insert some images
    sprintf(filename, "%s",





"a9749378-1.tif"
    );

    imagebox(demo, currPage, 0, 0, currPage->height / 2, currPage->width, 
      filename, gImageTiff);

    textbox(demo, currPage, 20, 20, 100, 30, filename);


    // Create a page
    currPage = pdfpage(demo, gPageSizeA4);

    // Insert some images
    sprintf(filename, "%s",





"a9749379-1.tif"
    );

    imagebox(demo, currPage, 0, 0, currPage->height / 2, currPage->width, 
      filename, gImageTiff);

    textbox(demo, currPage, 20, 20, 100, 30, filename);


    // Create a page
    currPage = pdfpage(demo, gPageSizeA4);

    // Insert some images
    sprintf(filename, "%s",





"a9749398-1.tif"
    );

    imagebox(demo, currPage, 0, 0, currPage->height / 2, currPage->width, 
      filename, gImageTiff);

    textbox(demo, currPage, 20, 20, 100, 30, filename);


    // Create a page
    currPage = pdfpage(demo, gPageSizeA4);

    // Insert some images
    sprintf(filename, "%s",





"a9749535-1.tif"
    );

    imagebox(demo, currPage, 0, 0, currPage->height / 2, currPage->width, 
      filename, gImageTiff);

    textbox(demo, currPage, 20, 20, 100, 30, filename);


    // Create a page
    currPage = pdfpage(demo, gPageSizeA4);

    // Insert some images
    sprintf(filename, "%s",





"a9749584-1.tif"
    );

    imagebox(demo, currPage, 0, 0, currPage->height / 2, currPage->width, 
      filename, gImageTiff);

    textbox(demo, currPage, 20, 20, 100, 30, filename);


    // Create a page
    currPage = pdfpage(demo, gPageSizeA4);

    // Insert some images
    sprintf(filename, "%s",





"a9749608-1.tif"
    );

    imagebox(demo, currPage, 0, 0, currPage->height / 2, currPage->width, 
      filename, gImageTiff);

    textbox(demo, currPage, 20, 20, 100, 30, filename);


    // Create a page
    currPage = pdfpage(demo, gPageSizeA4);

    // Insert some images
    sprintf(filename, "%s",





"a9749628-1.tif"
    );

    imagebox(demo, currPage, 0, 0, currPage->height / 2, currPage->width, 
      filename, gImageTiff);

    textbox(demo, currPage, 20, 20, 100, 30, filename);


    // Create a page
    currPage = pdfpage(demo, gPageSizeA4);

    // Insert some images
    sprintf(filename, "%s",





"a9749632-1.tif"
    );

    imagebox(demo, currPage, 0, 0, currPage->height / 2, currPage->width, 
      filename, gImageTiff);

    textbox(demo, currPage, 20, 20, 100, 30, filename);


    // Create a page
    currPage = pdfpage(demo, gPageSizeA4);

    // Insert some images
    sprintf(filename, "%s",





"a9749633-1.tif"
    );

    imagebox(demo, currPage, 0, 0, currPage->height / 2, currPage->width, 
      filename, gImageTiff);

    textbox(demo, currPage, 20, 20, 100, 30, filename);


    // Create a page
    currPage = pdfpage(demo, gPageSizeA4);

    // Insert some images
    sprintf(filename, "%s",





"a9749668-1.tif"
    );

    imagebox(demo, currPage, 0, 0, currPage->height / 2, currPage->width, 
      filename, gImageTiff);

    textbox(demo, currPage, 20, 20, 100, 30, filename);



  pdfclose(demo);

  return 0;
}



