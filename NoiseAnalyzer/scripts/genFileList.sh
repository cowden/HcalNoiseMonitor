#!/bin/bash

#####
# genFileList
# Generate the file list from a given run number 
# This script requires the run number and output file name
#

if [ "$#" -ne "2" ]; then
  echo "You must provide a run number and output file"
  echo "genFileList runNumber outputFile"
  exit 1
fi

if [ -z "$NOISEANALYZER" ]; then
  echo "Please setup the HcalNoiseMonitor environment"
  echo "source scripts/setup.sh"
  exit 2
fi


RN=$1
OFILE=$2


STREAM=MinimumBias
COLLECTION=Run2015B-v1
FORMAT=RAW

####
# dumpFiles function
# dump a list of files in dataset matching run numbers
dumpFiles(){
  RN1=${RN:0:3}
  RN2=${RN:3}
  gawkcmd="BEGIN { FS=\"/\" } { if ( \$9 == $RN1 && \$10 == $RN2 ) print \$0 }"
  dascli.py --limit=0 --query="file dataset=${DS}" | gawk "$gawkcmd"
}

DS=$(dascli.py --limit=0 --query="dataset dataset=/$STREAM/${COLLECTION}*/${FORMAT} run=$RN" | sed -n "/\/${STREAM}/p")

if [ -z "$DS" ]; then
  echo "The dataset /${STREAM}/${COLLECTION}*/${RAW} for run=$RN cannot be found"
  exit 2
fi


cat $NOISEANALYZER/scripts/DataTemplate.txt > $OFILE
count=0
fileList=$(dumpFiles)
for file in $fileList; do
  if [ "$count" == "0" ]; then
    echo "  '$file'" >> $OFILE
  else
    echo "  ,'$file'" >> $OFILE
  fi
  count=$((count+1))
  if [ "$count" == "256" ]; then
    echo "])" >> $OFILE
    echo "readFiles.extend([" >> $OFILE
    count=0
  fi
done
echo "])" >> $OFILE


echo "Completed generating $OFILE"


