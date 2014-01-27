import FWCore.ParameterSet.Config as cms

lumiTree = cms.EDAnalyzer("SusyCAF_LumiTreeMaker",
                          InputTag = cms.InputTag("lumiProducer")
)

