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
process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(20))

# Load RAW data (or RECO)
process.source = cms.Source("PoolSource",
                            fileNames = cms.untracked.vstring(
#    '/store/data/Run2011B/Jet/RECO/PromptReco-v1/000/175/975/EA1195E0-C3DD-E011-8D10-BCAEC518FF44.root',
#    '/store/data/Run2011B/Jet/RECO/PromptReco-v1/000/175/975/D89DF3AE-E5DD-E011-B7C1-001D09F34488.root',
#    '/store/data/Run2011B/Jet/RECO/PromptReco-v1/000/175/975/4828385D-DFDD-E011-8826-001D09F25041.root',
#    '/store/data/Run2011B/Jet/RECO/PromptReco-v1/000/175/975/38DB41C5-ACDD-E011-A70E-001D09F241B9.root',
#    '/store/data/Run2011B/Jet/RECO/PromptReco-v1/000/175/975/10C33618-FADD-E011-8FD9-001D09F253FC.root',
#    '/store/data/Run2011B/Jet/RECO/PromptReco-v1/000/175/975/1088020D-D4DD-E011-BEAB-0030486780EC.root',
#    '/store/data/Run2011B/Jet/RECO/PromptReco-v1/000/175/975/0A550845-F2DD-E011-8081-001D09F23944.root'
# '/store/data/Run2012A/HT/RECO/PromptReco-v1/000/191/226/F8DC3513-EA87-E111-980E-003048673374.root',
#  '/store/data/Run2012A/HT/RECO/PromptReco-v1/000/191/226/EC0BE285-D587-E111-84B8-003048D2BC52.root',
#  '/store/data/Run2012A/HT/RECO/PromptReco-v1/000/191/226/E8B08B9D-E687-E111-81E0-0030486780E6.root',
#  '/store/data/Run2012A/HT/RECO/PromptReco-v1/000/191/226/E62C0FB8-D887-E111-9719-003048D3C980.root',
#  '/store/data/Run2012A/HT/RECO/PromptReco-v1/000/191/226/E071C92F-1C88-E111-8569-BCAEC5329720.root',
#  '/store/data/Run2012A/HT/RECO/PromptReco-v1/000/191/226/E029DBF8-D487-E111-BA88-003048D2BD66.root',
#  '/store/data/Run2012A/HT/RECO/PromptReco-v1/000/191/226/DE2DB6A9-F687-E111-A3A4-BCAEC518FF68.root'
#   '/store/data/Run2012A/MET/RAW/v1/000/191/700/BE450D4C-9A89-E111-A25B-003048F11DE2.root'
   '/store/data/Run2012A/HcalHPDNoise/RAW/v1/000/191/210/C268F8C7-A185-E111-9B8D-00215AEDFCCC.root'
    )
                            
                            )

# -sk- histogram output.
process.TFileService = cms.Service("TFileService",
#cmj2011Apr24    fileName = cms.string('hitogram_output.root')
    fileName = cms.string('HCalNoiseMon_histogram_output.root')
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
camet = cms.Sequence(met)
process.recoCaloOnly = cms.Sequence(calolocalreco*ecalClusters*caloTowersRec*ak5cj*camet)

#------------------------------------------------------------
# If using RAW data, do Hcal RECO
#------------------------------------------------------------

process.GlobalTag.globaltag = 'GR_R_52_V7::All'

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


