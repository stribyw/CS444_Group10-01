TARGET=writing1
default: all
TEX=/usr/local/apps/tex_live/current/bin/x86_64-linux/latex
all: pdf 


# Documentation

pdf: ps
	ps2pdf $(TARGET).ps

ps: dvi
	dvips $(TARGET).dvi

dvi: $(TARGET).tex
	$(TEX) $(TARGET).tex
	# Uncomment below for included bibliography
	bibtex $(TARGET)
	$(TEX) $(TARGET).tex
	$(TEX) $(TARGET).tex

clean:
	rm -f $(TARGET).aux
	rm -f $(TARGET).dvi
	rm -f $(TARGET).log
	rm -f $(TARGET).out
	rm -f $(TARGET).ps
