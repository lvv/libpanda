/******************************************************************************
  date.c

  Change Control:                                                      DDMMYYYY
    Michael Still    Date handling functions first introduced          03092000

  Purpose:
    Functions related to the internal representation of dates in PDF land.
******************************************************************************/

#include "constants.h"
#include "functions.h"

// Format a given date into the approved format
char *makedate(int year, int month, int day, int hour, int minutes,
	       int seconds){
  char  *dateString, zulu = '+';
  int   gmthours = 10, gmtminutes = 0;

  // We need to work out how to decide what this machine's relationship to GMT
  // (zulu) is in a portable manner. I have a feeling that glibc offers some
  // assistance in this regard, but I need to have a look at the doco before
  // I can decide.
  
  // Make some space for the string we are going to return. We know this is
  // always going to be 21 characters long (so we say 23 characters to be
  // safe)
  if((dateString = malloc(sizeof(char) * 23)) == NULL)
    error("Could not make space for the date string.");

  // Do some insurance things to make sure we do y2k dates ok
  if(year < 100){
    if(year > 40) year += 1900;
    else year += 2000;
  }

  // Make the string
  snprintf(dateString, 22, "%4d%02d%02d%02d%02d%02d%c%02d'%02d'",
	   year, month, day, hour, minutes, seconds,
	   zulu, gmthours, gmtminutes);

  // And finish
  return dateString;
}

// Make a date string for the time that we are currently experiencing
char *nowdate(){
  


  return makedate(2000, 9, 4, 12, 42, 14);
}


