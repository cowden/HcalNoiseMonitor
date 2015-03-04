import FWCore.ParameterSet.Config as cms

process = cms.Process("Test")

process.load('FWCore/MessageService/MessageLogger_cfi')

process.MessageLogger.cerr.FwkReport.reportEvery = 100
process.maxEvents = cms.untracked.PSet(
   input = cms.untracked.int32(10000)
)

process.source = cms.Source("PoolSource",
   fileNames = cms.untracked.vstring(
        '/store/data/Commissioning10/MinimumBias/RECO/v4/000/130/910/E48DC8D8-902F-DF11-AE6D-0030487C5CE2.root'
   )
)

process.trigFilter1 = cms.EDFilter(
    "HcalMonTrigFilter",
    l1Tag = cms.InputTag("gtDigis"),	
    hltTag = cms.InputTag("TriggerResults::HLT"),
    l1Paths = cms.vstring("L1_SingleEG1"),
    techPaths = cms.vint32(),
    hltPaths = cms.vstring()
)

process.trigFilter2 = cms.EDFilter(
    "HcalMonTrigFilter",
    l1Tag = cms.InputTag("gtDigis"),
    hltTag = cms.InputTag("TriggerResults::HLT"),
    l1Paths = cms.vstring(),
    techPaths = cms.vint32(1),
    hltPaths = cms.vstring()
)

process.trigFilter3 = cms.EDFilter(
    "HcalMonTrigFilter",
    l1Tag = cms.InputTag("gtDigis"),
    hltTag = cms.InputTag("TriggerResults::HLT"),
    l1Paths = cms.vstring(),
    techPaths = cms.vint32(),
    hltPaths = cms.vstring('HLT_L1SingleEG1')
)

process.DirName1 = cms.EDAnalyzer(
    'HcalNoiseInfoAnalyzer',
    rbxCollName = cms.string('hcalnoise'),
    MetTag     = cms.untracked.InputTag( 'htMetIC5' ),
    SkipLumiBlocks = cms.double(0),
    NumLumiBlocks = cms.double(1000)
)

process.DirName2 = cms.EDAnalyzer(
    'HcalNoiseInfoAnalyzer',
    rbxCollName = cms.string('hcalnoise'),
    MetTag     = cms.untracked.InputTag( 'htMetIC5' ),
    SkipLumiBlocks = cms.double(0),
    NumLumiBlocks = cms.double(1000)
)

process.DirName3 = cms.EDAnalyzer(
    'HcalNoiseInfoAnalyzer',
    rbxCollName = cms.string('hcalnoise'),
    MetTag     = cms.untracked.InputTag( 'htMetIC5' ),
    SkipLumiBlocks = cms.double(0),
    NumLumiBlocks = cms.double(1000)
)

process.TFileService = cms.Service("TFileService",
   fileName = cms.string('XRUN.root')
)

process.p1 = cms.Path( process.trigFilter1 * process.DirName1 )
process.p2 = cms.Path( process.trigFilter2 * process.DirName2 )
process.p3 = cms.Path( process.trigFilter3 * process.DirName3 )

process.schedule = cms.Schedule( process.p1, process.p2, process.p3 )
