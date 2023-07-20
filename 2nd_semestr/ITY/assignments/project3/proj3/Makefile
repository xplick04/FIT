FILE=proj3
LOGIN=xplick04

all:
	latex $(FILE).tex
	latex $(FILE).tex
	dvips -t a4 $(FILE).dvi
	ps2pdf -sPAPERSIZE=a4 $(FILE).ps

zip:
	zip $(LOGIN)-fit.zip *.tex etiopan.eps oniisan.eps oniisan2.eps algorithm2e.sty Makefile

clean:
	rm -rf *.dvi *.out *.aux *.log *.ps *.pdf *.zip
