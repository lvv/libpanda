/******************************************************************************
	utility.c

	Change Control:													DDMMYYYY
		Michael Still		File created							17062000
		Michael Still		Obscure bug in byte offsets fixed.		22062000
							Windows printf will add a '\r' even if
							you don't ask it to, and even if you
							have added your own. I need to take
							this into account in the byte offsets.

	Purpose:
		Utility functions for the gnu pdf library.
******************************************************************************/

#include "constants.h"
#include "functions.h"

void pdfprintf(pdf *file, char *format, ...){
	// Print some information into the pdf file, but also record how many
	// bytes we have gone into the file. POSSIBLE BUG: Cannot print more than
	// 1024 bytes at a time. I am not sure how to decide how much memory to
	// allocate...
	char	buffer[1024];
	int		newlineCount = 0, counter = 0;
	va_list	argPtr;

	// Build the information
	va_start(argPtr, format);
	vsprintf(buffer, format, argPtr);
	va_end(argPtr);

	// Record how long it was
	file->byteOffset += strlen(buffer);

	// Count how many \n's there are and take into account the \r windows will
	// add (change for unix)
	while(counter < strlen(buffer)){
		if(buffer[counter] == '\n') newlineCount++;
		counter++;
	}

	// Add this number to the byteOffset
	file->byteOffset += newlineCount;

	// Store it
	fprintf(file->file, "%s", buffer);
}
