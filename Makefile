# Panda 0.1 Makefile

# Use the debug flag to help debug the program. I wont produce valid PDF files
# though...

# This is used to do the Windows specific I/O things we have to do.
#PLATFORM = WINDOWS
PLATFORM = LINUX

DODEBUG = NO
#DODEBUG = DEBUG

COMPILER = gcc
COMPILER_FLAGS = -g -c -D$(PLATFORM) -D$(DODEBUG)

# Main is excluded from here for the benefit of the tests
OBJFILES = error.o  font.o  objects.o  panda.o  text.o  trailer.o  utility.o  xref.o

# Build panda
all:		$(OBJFILES) main.o
		gcc $(OBJFILES) main.o -o panda

# Clean up
clean:	
		rm *.o; rm panda; rm *core*

# Make sure we build a realistic release for the world
release:
		make clean; make all; rm *.o; strip panda

# This is used to test the functionality of routines to make sure they are not
# broken
test:		test.c constants.h functions.h objects.h $(OBJFILES)
		$(COMPILER) $(COMPILER_FLAGS) test.c -o test.o; gcc $(OBJFILES) test.o -o test; ./test

################################################################################
error.o:	error.c constants.h functions.h objects.h
		$(COMPILER) $(COMPILER_FLAGS) error.c -o error.o

font.o:		font.c constants.h functions.h objects.h
		$(COMPILER) $(COMPILER_FLAGS) font.c -o font.o

main.o:		main.c constants.h functions.h objects.h
		$(COMPILER) $(COMPILER_FLAGS) main.c -o main.o

objects.o:	objects.c constants.h functions.h objects.h
		$(COMPILER) $(COMPILER_FLAGS) objects.c -o objects.o

panda.o:	panda.c constants.h functions.h objects.h
		$(COMPILER) $(COMPILER_FLAGS) panda.c -o panda.o

text.o:		text.c constants.h functions.h objects.h
		$(COMPILER) $(COMPILER_FLAGS) text.c -o text.o

trailer.o:	trailer.c constants.h functions.h objects.h
		$(COMPILER) $(COMPILER_FLAGS) trailer.c -o trailer.o

utility.o:	utility.c constants.h functions.h objects.h
		$(COMPILER) $(COMPILER_FLAGS) utility.c -o utility.o

xref.o:		xref.c constants.h functions.h objects.h
		$(COMPILER) $(COMPILER_FLAGS) xref.c -o xref.o

