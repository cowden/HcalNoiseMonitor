#!/bin/bash

#####
# Divide one job into 10.

if [ "$#" -ne "1" ]; then
  echo "You must provide a run list to analyze."
  echo "parallelizeJobs.sh runList.txt"
  exit 1
fi

RUNLIST="$1"

if [ -z "$NOISEANALYZER" ] ; then
  echo "Please setup the HcalNoiseMonitor environment."
  echo "source scripts/setup.sh"
  exit 2
fi

if [ ! -f "$RUNLIST" ]; then
  echo "The run list ${RUNLIST} does not exist ... bailing out!"
  exit 3
fi

# a function to find the number of events
findNEvents(){
  RN=$1
  FILES=$(dascli.py --limit=0 --query="file dataset=$DS run=$RN")
  nevents=0
  for f in $FILES; do
    NE=$(dascli.py --limit=0 --query="file=$f | grep file.nevents")
    if [[ "$NE" =~ ^[0-9]+$ ]]; then
      nevents=$((nevents+NE))
    fi
  done
  echo $nevents
}


for RN in `cat $RUNLIST`; do 
  # generate the file list
  genFileList.sh $RN ${NOISEANALYZER}/python/noiseFiles_${RN}.py

  DS="/MinimumBias/Run2015B-v1/RAW"
  NEVENTS=$(findNEvents $RN)
  NSKIP=$(echo "$NEVENTS/10" | bc)
  echo "$DS $NEVENTS $NSKIP" 
  INLIST="noiseFiles_$RN"
  for num in `seq 0 9`; do 
    TOSKIP=$(echo "$NSKIP*$num" | bc)
    sed -e "s/SKIP/$TOSKIP/;s/NEVENTS/$NSKIP/;s/INPUTLIST/$INLIST/;s/OUTPUTNAME/Hcal_noise_${RN}_${num}.root/" < ${CMSSW_BASE}/src/HcalNoiseMonitor/NoiseAnalyzer/scripts/hcalNoiseFullReco_TEMPLATE.py > scripts/hcalNoiseAnalysis_${RN}_${num}.py;
    bsub -q 2nd \
     	-o logs/hnm2_${RN}_${num}.stdout \
	-e logs/hnm2_${RN}_${num}.stderr \
	-J hnm2_${RN}_${num} \
	-F 50000000 \
	${CMSSW_BASE}/src/HcalNoiseMonitor/NoiseAnalyzer/scripts/cmsSubmission.sh ${PWD}/scripts/hcalNoiseAnalysis_${RN}_${num}.py ${PWD}/data/
  done
done
