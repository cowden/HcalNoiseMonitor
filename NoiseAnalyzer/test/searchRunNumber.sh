#
#	File = 'searchRunNumber.sh"
#
#	A script that contains most of the command-
#	line dbs command to look for the status of
#	the HCal Noise Monitoriing processing
#
#	Written by:
#			C M Jenkins
#			Department of Physics
#			University of South Alabama
#			Mobile, Al 36688
#			251.460.6224
#
#	To run
#		source searchRunNumber.sh 167221
#	Define boundaries
set RunNumber = $1
set LastRunPromptRecoV2 = 163982
set LastRunPromptRecoV3 = 164626
set LastRunPromptRecoV4 = 167964
set Run2011A = 175366
set Run2012 = 185250
echo "Search for Run: " $1
if ($1 <= $LastRunPromptRecoV2 ) then
echo 'dbs search --query "find file where dataset like *Interfill*Run2011A-PromptReco-v2*RECO* and run='$1'"'
dbs search --query "find file where dataset like *Interfill*Run2011A-PromptReco-v2*RECO* and run=$1"
else if ($1 > $LastRunPromptRecoV2 && $1 <= $LastRunPromptRecoV3) then
echo 'dbs search --query "find file where dataset like *Interfill*Run2011A-PromptReco-v3*RECO* and run='$1'"'
dbs search --query "find file where dataset like *Interfill*Run2011A-PromptReco-v3*RECO* and run=$1"
else if ($1 > $LastRunPromptRecoV3 && $1 <=  $LastRunPromptRecoV4) then
echo 'dbs search --query "find file where dataset like *Interfill*Run2011A-PromptReco-v4*RECO* and run='$1'"'
dbs search --query "find file where dataset like *Interfill*Run2011A-PromptReco-v4*RECO* and run=$1"
else
  if ($1 <= $Run2011A) then
  echo 'dbs search --query "find file where dataset like *Interfill*Run2011A-PromptReco*RECO* and run='$1'"'
  dbs search --query "find file where dataset like *Interfill*Run2011A-PromptReco*RECO* and run=$1"
  else if ($1 < $Run2012) then
  echo 'dbs search --query "find file where dataset like *Interfill*Run2011B-PromptReco*RECO* and run='$1'"'
  dbs search --query "find file where dataset like *Interfill*Run2011B-PromptReco*RECO* and run=$1"
  else
  echo 'dbs search --query "find file where dataset like *HcalHPDNoise*Run2012A*RAW* and run='$1'"'
  dbs search --query "find file where dataset like *HcalHPDNoise*Run2012A*RAW* and run=$1"
  endif
endif
#
