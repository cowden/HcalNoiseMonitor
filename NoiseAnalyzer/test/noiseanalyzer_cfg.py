import FWCore.ParameterSet.Config as cms

process = cms.Process("RECO")

process.load('FWCore/MessageService/MessageLogger_cfi')

process.load('Configuration/StandardSequences/MagneticField_AutoFromDBCurrent_cff')
process.load('Configuration/StandardSequences/GeometryExtended_cff')

process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")
process.GlobalTag.globaltag = 'GR10_P_V6::All' 

process.load("Configuration/StandardSequences/RawToDigi_Data_cff")
process.load("Configuration/StandardSequences/Reconstruction_cff")

process.MessageLogger.cerr.FwkReport.reportEvery = 100
process.maxEvents = cms.untracked.PSet(
   input = cms.untracked.int32(1000)
)

process.source = cms.Source("PoolSource",
   fileNames = cms.untracked.vstring(
	'/store/data/Run2010A/HcalHPDNoise/RAW/v1/000/137/619/BAA458AE-3C77-DF11-9E06-001D09F25456.root'
	#'file:*.root'
   )
)


process.hcalnoiseinfoanalyzer = cms.EDAnalyzer(
    'HcalNoiseInfoAnalyzer',
    rbxCollName = cms.string('hcalnoise'),
    MetTag     = cms.untracked.InputTag( 'met' ),
    SkipLumiBlocks = cms.double(0),
    NumLumiBlocks = cms.double(5000)
)

process.TFileService = cms.Service("TFileService",
   fileName = cms.string('RUN.root')
)

process.raw2digi_step = cms.Path(process.RawToDigi)
process.reconstruction_step = cms.Path(process.reconstruction)

process.p1 = cms.Path( process.hcalnoiseinfoanalyzer )

process.schedule = cms.Schedule( process.raw2digi_step, process.reconstruction_step, process.p1 )
