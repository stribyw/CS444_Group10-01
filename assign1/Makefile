TARGET=hw1
default: all

all: pdf 


# Documentation

pdf: ps
	ps2pdf $(TARGET).ps

ps: dvi
	dvips $(TARGET).dvi

dvi: $(TARGET).tex
	latex $(TARGET).tex
	# Uncomment below for included bibliography
	#bibtex $(TARGET)
	#latex $(TARGET).tex
	latex $(TARGET).tex

clean:
	rm -f $(TARGET).aux
	rm -f $(TARGET).dvi
	rm -f $(TARGET).log
	rm -f $(TARGET).out
	rm -f $(TARGET).ps
