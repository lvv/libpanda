# Panda 0.1 Makefile -- Time trials

COMPILER=gcc

##############################################################################

all:
		echo "There is no all here"

clean:		
		rm *.o; rm timings*; rm output.pdf

hello-clibpdf-202r1:	
		$(COMPILER) hello-clibpdf-202r1.c -o hello-clibpdf-202r1.o -lcpdf -lm

hello-pdflib-30:
		$(COMPILER) hello-pdflib-30.c -o hello-pdflib-30.o -lpdf -lm -ltiff -lpng

hello-panda-01:
		$(COMPILER) hello-panda-01.c -o hello-panda-01.o -lpanda

manytextboxes-clibpdf-202r1:	
		$(COMPILER) manytextboxes-clibpdf-202r1.c -o manytextboxes-clibpdf-202r1.o -lcpdf -lm

manytextboxes-pdflib-30:
		$(COMPILER) manytextboxes-pdflib-30.c -o manytextboxes-pdflib-30.o -lpdf -lm -ltiff -lpng

manytextboxes-panda-01:
		$(COMPILER) manytextboxes-panda-01.c -o manytextboxes-panda-01.o -lpanda