#
# File = "hcalNoiseFullReco_cff.py"
#
#  This file from and E-mail sent by Shuichi 2012Apr23
#
import FWCore.ParameterSet.Config as cms

process = cms.Process("ANA")

process.load('Configuration.StandardSequences.Services_cff')
process.load('SimGeneral.HepPDTESSource.pythiapdt_cfi')
process.load('FWCore.MessageService.MessageLogger_cfi')
process.load('Configuration.EventContent.EventContent_cff')
process.load('Configuration.StandardSequences.GeometryDB_cff')
process.load('Configuration.StandardSequences.MagneticField_AutoFromDBCurrent_cff')
process.load('Configuration.StandardSequences.RawToDigi_Data_cff')
process.load('Configuration.StandardSequences.Reconstruction_Data_cff')
process.load('Configuration.StandardSequences.EndOfProcess_cff')
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')


# Print every message
process.load("FWCore.MessageService.MessageLogger_cfi")
process.MessageLogger.cerr.FwkReport.reportEvery = 100

# Process 100 events
process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(NEVENTS))

# Load RAW data (or RECO)
#from run_minimumbias_227489 import *
from INPUTLIST import *
process.source = source



# -sk- histogram output.
process.TFileService = cms.Service("TFileService",
#cmj2011Apr24    fileName = cms.string('hitogram_output.root')
    fileName = cms.string('OUTPUTNAME.root')
)

# Output definition

process.RECOoutput = cms.OutputModule("PoolOutputModule",
    splitLevel = cms.untracked.int32(0),
    eventAutoFlushCompressedSize = cms.untracked.int32(5242880),
    outputCommands = process.RECOEventContent.outputCommands,
    fileName = cms.untracked.string('RelVal_HLT_RECO_name.root'),
    dataset = cms.untracked.PSet(
        filterName = cms.untracked.string(''),
        dataTier = cms.untracked.string('RAW-HLT-RECO')
    )
)

# -sk-  calo-only reco,  turn off vertex correction
from RecoLocalCalo.Configuration.RecoLocalCalo_cff import *
from RecoEcal.Configuration.RecoEcal_cff import *
from RecoJets.Configuration.CaloTowersRec_cff import *
from RecoMET.Configuration.RecoMET_cff import *
from RecoJets.JetProducers.ak5CaloJets_cfi import *
process.calotowermaker.hbheInput = cms.InputTag("hbheprereco")
process.towerMaker.hbheInput = cms.InputTag("hbheprereco")
process.towerMakerWithHO.hbheInput = cms.InputTag("hbheprereco")
process.ak5CaloJets.doPVCorrection= False
ak5cj = cms.Sequence(ak5CaloJets)
camet = cms.Sequence(metreco)
process.recoCaloOnly = cms.Sequence(calolocalreco*ecalClusters*caloTowersRec*ak5cj*camet)

# choose method 0 for hbheprereco
process.hbheprereco.puCorrMethod = cms.int32(0)

#------------------------------------------------------------
# If using RAW data, do Hcal RECO
#------------------------------------------------------------

process.GlobalTag.globaltag = 'GR_E_V42::All'

# Path and EndPath definitions
process.raw2digi_step = cms.Path(process.RawToDigi)
process.reconstruction_step = cms.Path(process.reconstruction)
# -sk- process.reconstruction_step = cms.Path(process.recoCaloOnly)
process.endjob_step = cms.EndPath(process.endOfProcess)
process.RECOoutput_step = cms.EndPath(process.RECOoutput)


# Analyzer
# process.ana = cms.EDAnalyzer("HcalPFGRecHitAna")

#cmj2011Apr24process.hcaljetmon = cms.EDAnalyzer('HCalJetMon',
#cmj2011Apr24  TriggerTag = cms.untracked.InputTag( 'TriggerResults::HLT' )
#cmj2011Apr24 )
#cmj2011Apr24 process.q       = cms.Path(process.HCalJetMon)
#cmj2011Apr24
process.hcalnoiseinfoanalyzer = cms.EDAnalyzer('HcalNoiseInfoAnalyzer',
  TriggerTag = cms.untracked.InputTag( 'TriggerResults::HLT' ),
#cmj2011Apr24
  rbxCollName = cms.string('hcalnoise'),
  MetTag     = cms.untracked.InputTag( 'met' ),
  SkipLumiBlocks = cms.double(0),
  NumLumiBlocks = cms.double(5000)
#cmj2011Apr24
)
process.q       = cms.Path(process.hcalnoiseinfoanalyzer)

#-------------------------------------HLT Trigger Selection--------------------------------------------------------------
# -sk-  HLT filter is not activated yet.   
process.load('HLTrigger.HLTfilters.hltHighLevel_cfi')

# process.hltHighLevel.HLTPaths = cms.vstring( "HLT_Mu17_Mu8*" )
# process.hltHighLevel.HLTPaths = cms.vstring( "HLT_ZeroBias*" )
# process.hltHighLevel.HLTPaths = cms.vstring( "HLT_CentralJet80*" )
process.hltHighLevel.HLTPaths = cms.vstring( "HLT_MonoCentralPFJet80_PFMETnoMu95_NHEF0p95*" )

process.hltHighLevel.andOr = True   # True = OR, False = AND
process.hltHighLevel.throw = False  # tolerate if triggers not available

process.p = cms.Path(process.hltHighLevel)

# Schedule definition
#    -- produce output event file. 
# process.schedule = cms.Schedule(process.raw2digi_step,process.reconstruction_step,process.endjob_step,process.RECOoutput_step,process.q)
#    -- no output event file.
process.schedule = cms.Schedule(process.raw2digi_step,process.reconstruction_step,process.endjob_step,process.q)


