#!/bin/bash

####
# cmsSubmission.sh
# A script to be executed on batch node.
# This script sets up the CMSSW environment and then 
# executes cmsRun with the given configuration file (required argument)

ANALYSIS_DIR=$PWD

if [ "$#" -ne "1" ]; then
  echo "Please provide CMSSW configuration script"
  exit 1
fi

echo "Setting up analysis with configuration -> $1"

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

