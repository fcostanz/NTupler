import FWCore.ParameterSet.Config as cms

susycafl1calotrigger = cms.EDProducer("SusyCAF_L1CaloTrigger",
                            InputTag = cms.InputTag('gctDigis'),
                            Prefix = cms.string('l1'),
                            Suffix = cms.string('hw'),
                            )
