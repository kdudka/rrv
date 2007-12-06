DOC=proj_doc

.PHONY: clean

$(DOC).pdf: clean
	pdflatex $(DOC)
	bibtex $(DOC)
	pdflatex $(DOC)
	pdflatex $(DOC)

clean:
	rm -f *.dvi *.log *.blg *.bbl *.toc *.aux *.out *.nav *.snm #$(DOC).pdf
