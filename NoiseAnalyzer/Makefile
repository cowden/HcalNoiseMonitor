#####################################
# Makefile to build the standalone analysis
# tools.
#####################################

ROOTCFLAGS = $(shell root-config --cflags)
ROOTLIBS = $(shell root-config --glibs)

#----------------------------------------
.PHONY: all clean

all:
	g++ $(ROOTCFLAGS) -o bin/rateCalc analysis/rateCalc.cc $(ROOTLIBS)

clean:
	rm bin/*

