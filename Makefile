# Panda 0.1 Makefile

# Use the debug flag to help debug the program. I wont produce valid PDF files
# though...

# This is used to do the Windows specific I/O things we have to do.
#PLATFORM = WINDOWS
PLATFORM = LINUX

DODEBUG = NO
#DODEBUG = DEBUG

COMPILER = gcc
COMPILER_FLAGS = -g -c -D$(PLATFORM) -D$(DODEBUG) -Wall

# Main is excluded from here for the benefit of the tests and the library
OBJFILES = error.o  font.o  objects.o  panda.o  text.o  trailer.o  utility.o  xref.o

# Build panda (including the sample application)
all:		$(OBJFILES) main.o
		gcc $(OBJFILES) main.o -o panda

# This makes the archive that is the panda library. It is not installed by this
# set of commands though, that needs a make install... The ar command is
# quite interesting, read the man page for more information...
lib:		$(OBJFILES)
		ar rs libpanda.a $(OBJFILES)

# This installs the library and header files in the right places
# I would have used -b here as well, but it is not supported on FreeBSD...
install:	lib
		install -m 664 libpanda.a /usr/local/lib/libpanda.a
		install -m 664 -d /usr/local/include/panda
		install -m 664 -D *.h /usr/local/include/panda/
		install -m 664 -d /usr/local/man/man3
		install -m 664 -D man/*.3 /usr/local/man/man3

# This is used in my testing of make install. It does not uninstall the man
# pages yet because I am not sure of the neatest way of doing this just yet...
uninstall:	
		rm -i /usr/local/lib/libpanda*
		rm -ri /usr/local/include/panda

# Clean up
clean:	
		rm -i *.o
		rm -i panda
		rm -i libpanda.a
		rm -i *core*
		rm -i *~

# This is used to test the functionality of routines to make sure they are not
# broken
test:		test.c constants.h functions.h objects.h $(OBJFILES)
		$(COMPILER) $(COMPILER_FLAGS) test.c -o test.o
		$(COMPILER) $(OBJFILES) test.o -o test; ./test

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

