#!/bin/bash


###
# This script adds a column to the RBX by number RBX noise rate.
# The rates are taken from the output of the rateCalc program.
# This script expects the run number, the threshold, input log file, and output file
# to be given.
# The data is expected to lie between the BTH# and ETH# flags found in the 
# log file.

if [ "$#" -ne "4" ]; then
  echo "The run number, threshold [20 50 100 500], input file, output file are required"
  echo "addToRBXNumber.sh runNumber threshold inFile outFile"
  exit 1
fi

RN=$1
TH=$2
INFILE=$3
OFILE=$4

insertZeros(){
  for b in `seq 1 80`; do echo "$RN $b 0" >> $OFILE; done 
  echo "" >> $OFILE
}

# check if the input file exists
if [ ! -f "$INFILE" ]; then
  echo "Inserting 0's for empty run number"
  insertZeros
  exit 0
fi


data=$(sed -n "/BTH${TH}/,/ETH${TH}/p" < $INFILE | egrep "^[0-9]")
data2=$(sed -n "/BTH${TH}/,/ETH${TH}/p" < $INFILE | egrep "^[0-9]" | gawk '{print $1+1" "$2+1" "$3}')


hasNan=$(echo "$data" | grep "nan")
if [ -n "$hasNan" ]; then
  insertZeros
  exit 0
fi

if [ -z "$data" ]; then
  insertZeros
  exit 0
fi

echo "$data" >> $OFILE
echo "" >> $OFILE
#echo "$data2" >> $OFILE
#echo "" >> $OFILE
