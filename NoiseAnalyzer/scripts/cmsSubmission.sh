#!/bin/bash

####
# cmsSubmission.sh
# A script to be executed on batch node.
# This script sets up the CMSSW environment and then 
# executes cmsRun with the given configuration file (required argument)
# Since execution is done a local file system the output directory is also required.
# This script will copy all root files to that directory.

ANALYSIS_DIR=$PWD

if [ "$#" -ne "2" ]; then
  echo "Please provide CMSSW configuration script and output directory"
  echo "cmsSubmission.sh  config.py outputDir/"
  exit 1
fi

echo "Setting up analysis with configuration -> $1"

OUTPUTDIR=$2

env
echo ""

# setup CMSSW
if [ -z "$SCRAM_ARCH" ]; then
  export SCRAM_ARCH=slc6_amd64_gcc491
fi

source /cvmfs/cms.cern.ch/cmsset_default.sh

if [ ! -z "$CMSSW_BASE" ]; then
  cd $CMSSW_BASE
  eval `scram runtime -sh`
else
  echo "CMSSW_BASE is not defined"
  exit 2
fi

cd $ANALYSIS_DIR
export PYTHONPATH=$PYTHONPATH:$PWD/scripts
  
NOISEANALYZER=$CMSSW_BASE/src/HcalNoiseMonitor/NoiseAnalyzer
export PATH=$PATH:$NOISEANALYZER/bin:$NOISEANALYZER/scripts

cmsRun $1

echo "Job Complete $?"

echo "Moving output to $OUTPUTDIR"
mv *.root $OUTPUTDIR/
