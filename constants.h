/******************************************************************************
	Constants.h

	Change Control:													DDMMYYYY
		Michael Still		File created							08062000
		Michael Still		Added magic header string				17062000

	Purpose:
		This file defines some constants that are used in various places
		throughout the code. If the value GLOBALS is defined, then we are in 
		the one source file which owns the variables
******************************************************************************/

// The text that we make binary to display in the first 100 bytes or so of the
// PDF (forces some FTP clients into binary mode). The function initgnupdf
// actually generates the gBinaryHeaderString string value.
#define	gMagicHeaderString		"%PDF-1.3 "
#define	gHeaderString			"GNU PDF, 2000"
#define	gBinaryChar(x)			256 - x

#if defined GLOBALS
	char	gBinaryHeaderString[14];
#else
	extern char	gBinaryHeaderString[14];
#endif

// There are two kinds of dictionary values, ints and text. We need to say
// which we are using
#define	gDictionaryValue		4
#define	gIntValue				0
#define	gTextValue				1
#define	gLiteralTextValue		5
#define	gObjValue				2
#define	gObjArrayValue			3

// Standard page sizes -- these are expressed in inches, with 72 dots per inch
// with no fractional values shown. As many values as needed can be added here
#define	gPageSizeA4				"[0 0 594 841]"
#define	gPageSizeUsLetter		"[0 0 612 792]"

// We have the concept of placeholder objects, that do not get written out to
// the PDF
#define	gPlaceholder			0
#define gNormal					1
