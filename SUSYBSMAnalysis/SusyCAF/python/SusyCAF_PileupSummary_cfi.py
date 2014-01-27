import FWCore.ParameterSet.Config as cms

susycafpileupsummary = cms.EDProducer("SusyCAF_PileupSummary",
                                      InputTag = cms.InputTag('addPileupInfo'),
                                      Prefix = cms.string('pileup'),
                                      Suffix = cms.string(''),
                                      )
