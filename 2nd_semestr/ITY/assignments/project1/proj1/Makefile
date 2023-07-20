FILE=proj1
LOGIN=xplick04

all:
	latex $(FILE).tex
	dvips -t a4 $(FILE).dvi
	ps2pdf $(FILE).ps

zip:
	zip $(LOGIN)-fit.zip *.tex Makefile

clean:
	rm -rf *.dvi *.aux *.out *.ps *.log *.pdf *.zip
