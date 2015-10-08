#!/bin/bash

###
# This script adds a point to the HPD Discharge
# rate vs run number plot.
# The rates are taken from the output of the rateCalc program.
# This script expects the run number, input log file, and output file.
# The output file is a text file in a format to be plotted by gnuplot
# or some other root script.

if [ "$#" -ne "3" ]; then
  echo "The run number, input and output files are required"
  echo "addToHPDDischarge runNumber inFile outFile"
  exit 1
fi

RUN=$1
INFILE=$2
OFILE=$3

# check if input file exists
if [ ! -f "$INFILE" ]; then
  echo "The input file $INFILE does not exist ... bailing out!"
  exit 2
fi

data=$(egrep "^[0-9]" $INFILE | gawk 'BEGIN { th20=0; th50=0; th100=0; th500=0;} { if ( $1 == 20 ) th20=$3; else if ( $1 == 50 ) th50=$3; else if ( $1 == 100 ) th100=$3; else if ( $1 == 500 ) th500=$3; } END { print th20,th50,th100,th500 }')

echo "$RUN $data" >> $OFILE


