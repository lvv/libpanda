/******************************************************************************
  utility.c

  Change Control:                                                      DDMMYYYY
    Michael Still    File created                                      17062000
                     Obscure bug in byte offsets fixed.                22062000
                       Windows printf will add a '\r' even if
                       you don't ask it to, and even if you
                       have added your own. I need to take
                       this into account in the byte offsets.
		     Added textstreamprintf(...) call.                 17072000
		     Added pdfputc call                                22072000

  Purpose:
    Utility functions for the panda library.
******************************************************************************/

#include "constants.h"
#include "functions.h"

// Print a possibly complex string into the PDF file and make sure the offset
// into the PDF file is stored correctly (dispite Windows)
void pdfprintf(pdf *file, char *format, ...){
  char     *buffer, *formatString, *token, *strtokVictim;
  int      newlineCount = 0, counter = 0, indent = 0, actualLen, value,
           targetNo, digits;
  va_list  argPtr;

  // This is a little strange... On a windows machine, printf inserts \r's 
  // for us, even if we have them already. Therefore we don't put them into 
  // the format string, and then add them here if we were not compiled on 
  // windows
#if defined WINDOWS
  if((formatString = malloc((strlen(format) + 1) * sizeof(char))) == NULL)
    error("Could not make temporary printing space.");

  strcpy(formatString, format);
#else
  // We need to go through the format string and replace \n with \r\n
  // because lines in a PDF end with \r\n (one of the options)

  // Count the number of \n's.
  for(counter = 0; counter < strlen(format); counter++)
    if(format[counter] == '\n') newlineCount++;

  if((formatString = 
    malloc((strlen(format) + newlineCount + 1) * sizeof(char))) == NULL)
    error("Could not make temporary printing space.");
  formatString[0] = 0;

  // Make every \n a \r\n in the format string
  //  for(counter = indent = 0; counter < strlen(format); counter++){
  //    if(format[counter] == '\n'){
  //      formatString[indent++] = '\r';
  //      formatString[indent++] = format[counter];
  //      }
  //     else formatString[indent++] = format[counter];
  //    }
  //
  //  formatString[indent] = 0;

  if((strtokVictim = (char *) malloc(sizeof(char) * (strlen(format) + 1)))
    == NULL)
    error("Could not make a strtok victim.");
  strcpy(strtokVictim, format);
  token = strtok(strtokVictim, "\n");

  // There is a special case when we are at the end of the format string and
  // have a left over token when we don't want to append the newline info...
  while(token != NULL){
    strcat(formatString, token);
    token = strtok(NULL, "\n");

    if(token != NULL) strcat(formatString, "\r\n");
  }

  // We don't need strtokVictim any more
  free(strtokVictim);

  // We might have missed at \n the end
  if(format[strlen(format) - 1] == '\n') strcat(formatString, "\r\n");
  
#endif

  // Now we need to make a best guess at how long buffer needs to be -- it is 
  // hardly ever longer than 1k...
  if((buffer = malloc(1024 * sizeof(char))) == NULL)
    error("Could not grab space for a pdfprintf call (1k).");

  // Build the information
  va_start(argPtr, format);
  if((actualLen = vsnprintf(buffer, 1024, formatString, argPtr)) > 1020){
    // We did not fit! Try again...
    free(buffer);

#if defined DEBUG
    printf("Needed to make a bigger space for the buffer in pdfprintf\n");
#endif

    if((buffer = malloc(actualLen * sizeof(char))) == NULL)
      error("Could not grab space for a pdfprintf call (actual).");

    if(vsnprintf(buffer, actualLen, formatString, argPtr) > actualLen){
      error("Really bad file i/o error.");
    }
  }

  // Record how long it was
  file->byteOffset += strlen(buffer);

#if defined WINDOWS
  // Count how many \n's there are and take into account the \r windows will
  // add (change for unix)

  // POSSIBLE BUG: Binary textstreams may be corrupted by Windows here
  for(icounter = 0; counter < strlen(buffer); counter++){
    if(buffer[counter] == '\n') newlineCount++;
  }

  // Add this number to the byteOffset
  file->byteOffset += newlineCount;
#endif

  // Store it
  fprintf(file->file, "%s", buffer);

  // Free the temp data structures
  free(formatString);
  free(buffer);
  va_end(argPtr);
}

// Append some text to the textstream that we are creating for a given page
char *streamprintf(char *stream, char *format, ...){
  va_list        argPtr;
  char           buffer[2048];
  unsigned long  currentlen = 0, len;

  // Get the data to add to the stream
  va_start(argPtr, format);
  vsprintf(buffer, format, argPtr);
  va_end(argPtr);

  // Determine some lengths for the various strings
  if(stream != NULL) currentlen = strlen(stream) + 1;
  len = strlen(buffer) + 1;

  // Make space for the new information
  if((stream = (char *) realloc(stream, 
    sizeof(char) * (len + currentlen))) == NULL)
    error("Could not append to an object's stream (of some form).");

  // Do the actual appending
  strcat(stream, buffer);

  // Return the stream
  return stream;
}

// Put just one character into the PDF file, while updating the offset so that
// the xref table works later on
void pdfputc(pdf *output, int c){
  fputc(c, output->file);
  output->byteOffset++;
}

// Put some known text into the PDF file, also update the offset
// Don't forget to do the \n to \r\n conversion as well
void pdfprint(pdf *output, char *format){
  char   *strtokVictim, *formatString, *token;
  int    counter, newlineCount;

#if defined WINDOWS
  if((formatString = malloc((strlen(format) + 1) * sizeof(char))) == NULL)
    error("Could not make temporary printing space.");

  strcpy(formatString, format);
#else
  // We need to go through the format string and replace \n with \r\n
  // because lines in a PDF end with \r\n (one of the options)

  // Count the number of \n's.
  newlineCount = 0;
  for(counter = 0; counter < strlen(format); counter++)
    if(format[counter] == '\n') newlineCount++;

  if((formatString = 
    malloc((strlen(format) + newlineCount + 1) * sizeof(char))) == NULL)
    error("Could not make temporary printing space.");
  formatString[0] = 0;

  if((strtokVictim = (char *) malloc(sizeof(char) * (strlen(format) + 1)))
    == NULL)
    error("Could not make a strtok victim.");
  strcpy(strtokVictim, format);
  token = strtok(strtokVictim, "\n");

  // There is a special case when we are at the end of the format string and
  // have a left over token when we don't want to append the newline info...
  while(token != NULL){
    strcat(formatString, token);
    token = strtok(NULL, "\n");

    if(token != NULL) strcat(formatString, "\r\n");
  }

  // We might have missed at \n the end
  if(format[strlen(format) - 1] == '\n') strcat(formatString, "\r\n");

  // We don't need strtokVictim any more
  free(strtokVictim);
#endif

  fprintf(output->file, "%s", formatString);
  output->byteOffset += strlen(formatString);

  // With windows we might also need to update the newline count here
}



