#!/bin/bash

####
#  genConfiguration.sh
# generate the CMSSW configuration file for a particular run number
# This script is similar to the genFileList.sh, in fact this script will
# call that script to generate the file list for the given run number.
# The run number and output configuration file name are required arguments
# to this script.

if [ "$#" -ne "3" ]; then
  echo "You must provide a run number, output root file name,  and output configuration file name"
  echo "genConfiguration.sh runNumber outRootFile.root config.py"
  exit 1
fi

if [ -z "$NOISEANALYZER" ]; then
  echo "Please setup the HcalNoiseMonitor environment"
  echo "source scripts/setup.sh"
  exit 2
fi

RN=$1
OUTPUTROOT=$2
OUTPUT=$3
NEVENTS=200

FILELIST=noiseFiles_${RN}.py
FLSCRIPT=${FILELIST%%.py}

# generate the file list
genFileList.sh $RN $NOISEANALYZER/python/$FILELIST

# generate configuration from template
sed -e "s/INPUTLIST/$FLSCRIPT/;s/OUTPUTNAME/$OUTPUTROOT/;s/NEVENTS/$NEVENTS/" < $NOISEANALYZER/scripts/hcalNoiseFullReco_TEMPLATE.py > $OUTPUT




