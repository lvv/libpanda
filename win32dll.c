/******************************************************************************
	Win32dll.c

	Change Control:													DDMMYYYY
		Michael Still		File created							24072000

	Purpose:
		This file is used to export the relevant symbols for a win32 dll of
		the Panda library. It simply exports the names of the functions that
		I think should be accessible to the calling application. This file,
		therefore, does not include any exports for the internal functions
		of the library such as pdfputc(...)

		If this is a major problem, then feel free to add these function calls
		to this file, but I cannot think of a good reason to do so.
******************************************************************************/