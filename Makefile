# Panda 0.1 Makefile

COMPILER = gcc
COMPILER_FLAGS = -g -c

# This is used to do the Windows specific I/O things we have to do.
#PLATFORM = WINDOWS
PLATFORM = LINUX

OBJFILES = error.o  font.o  main.o  objects.o  panda.o  text.o  trailer.o  utility.o  xref.o

# Build panda
all:		$(OBJFILES)
		gcc $(OBJFILES) -o panda

# Clean up
clean:	
		rm *.o; rm panda

# Make sure we build a realistic release for the world
release:
		make clean; make all; rm *.o; strip panda

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
		$(COMPILER) $(COMPILER_FLAGS) -D$(PLATFORM) utility.c -o utility.o

xref.o:		xref.c constants.h functions.h objects.h
		$(COMPILER) $(COMPILER_FLAGS) xref.c -o xref.o

