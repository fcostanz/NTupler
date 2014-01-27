import FWCore.ParameterSet.Config as cms

susycafpfsump4 = cms.EDProducer( "SusyCAF_SumP4",
                                 InputTag = cms.InputTag("particleFlow"),
                                 Prefix = cms.string("pf"),
                                 Suffix = cms.string("")
                                 )
