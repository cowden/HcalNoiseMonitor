
####
# Setup the HcalNoiseMonitoring environment
# if the CMSSW environment is not already
# setup, then assume you are in the analysis
# directory

if [ ! -z "$CMSSW_BASE" ]; then
  export NOISEANALYZER=$CMSSW_BASE/src/HcalNoiseMonitor/NoiseAnalyzer

  export PATH=$PATH:$NOISEANALYZER/bin:$NOISEANALYZER/scripts
  export PYTHONPATH=$PYTHONPATH:$NOISEANALYZER/python

else

  export NOISEANALYZER=$PWD
  export PATH=$PATH:$PWD/bin:$PWD/scripts
  export PYTHONPATH=$PYTHONPATH:$PWD/python

fi


