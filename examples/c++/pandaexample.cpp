#include <panda/functions.h>
#include <panda/constants.h>
#include "pandacpp.h"

int main(int argc, char *argv[]){
  // Setup Panda
  Cpanda mypanda;
  mypanda.init();

  // Create a new document
  Cpanda_pdf mypdf = mypanda.open("foo.pdf", "w");
  Cpanda_page mypage = mypdf.newpage(panda_pagesize_a4);
  mypage.textboxrot(300, 30, 330, 30, 45.0, "With new improved angled text!");
  mypdf.close();

  // We have finished
  return 1;
}
