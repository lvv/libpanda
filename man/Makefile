# Copied from Fast Artificial Neural Network Library

XML = panda.xml

all: html html-single dvi pdf ps rtf tex txt

html: $(XML)
	jw -b html -o html $(XML)

html-single: $(XML)
	jw -u -b html $(XML)

dvi: $(XML)
	jw -u -b dvi $(XML)

pdf: $(XML)
	jw -u -b pdf $(XML)

ps: $(XML)
	jw -u -b ps $(XML)

rtf: $(XML)
	jw -u -b rtf $(XML)

tex: $(XML)
	jw -u -b tex $(XML)

txt: $(XML)
	jw -u -b txt $(XML)

clean:
	rm -rf \
		html html-single dvi pdf ps rtf tex txt \
		fann.html \
		fann.dvi \
		fann.pdf \
		fann.ps \
		fann.rtf \
		fann.tex \
		fann.txt

distclean: clean
