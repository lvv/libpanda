# Panda 0.1 Makefile -- Time trials

# First one turns profiling on
PROFILING = -pg
#PROFILING =

COMPILER=gcc
COMPOPTS=-I/usr/local/include -L/usr/local/lib -Wall -ggdb $(PROFILING)

##############################################################################

all:
		echo "There is no all here"

clean:		
		rm *.o; rm timings*; rm output.pdf

##############################################################################

hello-clibpdf-202r1:	
		$(COMPILER) $(COMPOPTS) hello-clibpdf-202r1.c -o hello-clibpdf-202r1.o -lcpdf -lm

hello-pdflib-30:
		$(COMPILER) $(COMPOPTS) hello-pdflib-30.c -o hello-pdflib-30.o -lpdf -lm -ltiff -lpng

hello-panda-02:
		$(COMPILER) $(COMPOPTS) hello-panda-02.c -o hello-panda-02.o -lpanda

##############################################################################

manytextboxes-clibpdf-202r1:	
		$(COMPILER) $(COMPOPTS) manytextboxes-clibpdf-202r1.c -o manytextboxes-clibpdf-202r1.o -lcpdf -lm

manytextboxes-pdflib-30:
		$(COMPILER) $(COMPOPTS) manytextboxes-pdflib-30.c -o manytextboxes-pdflib-30.o -lpdf -lm -ltiff -lpng

manytextboxes-panda-02:
		$(COMPILER) $(COMPOPTS) manytextboxes-panda-02.c -o manytextboxes-panda-02.o -lpanda

##############################################################################

manypages-panda-02:
		$(COMPILER) $(COMPOPTS) manypages-panda-02.c -o manypages-panda-02.o -lpanda	
