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





"g4-B-msb2lsb-contig-1000.tif"
    );

    imagebox(demo, currPage, 0, 0, currPage->height / 2, currPage->width, 
      filename, gImageTiff);

    textbox(demo, currPage, 20, 20, 100, 30, filename);



  pdfclose(demo);

  return 0;
}



