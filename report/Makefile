all:
	pdflatex report.tex
	bibtex report
	pdflatex report.tex
	pdflatex report.tex
clean:
	rm -rf ./report.log ./report.aux ./report.pdf ./*.bbl ./*~ ./*.run.xml ./*.blg
