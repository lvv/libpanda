/******************************************************************************
  Memory.c

  Change Control:                                                      DDMMYYYY
    Michael Still    File created                                      09012001

  Purpose:
    Abstract the out of memory tests so that calling functions can asusme that
    memory was returned by malloc et al.
******************************************************************************/

#include <stdio.h>

void *panda_xmalloc(size_t size){
  void *buffer;

  if((buffer = malloc(size)) == NULL){
    panda_error("panda_xmalloc failed to allocate memory");
  }
  
  return buffer;
}
