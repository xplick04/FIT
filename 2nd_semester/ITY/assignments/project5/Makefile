FILE=proj5
LOGIN=xplick04

all:
	pdflatex $(FILE).tex

zip:
	zip $(LOGIN)-fit.zip *.tex array.png *.pdf Makefile

clean:
	rm -rf *.dvi *.out *.aux *.log *.ps *.pdf *.zip *.toc *.snm *.nav
