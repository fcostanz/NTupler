import FWCore.ParameterSet.Config as cms

susycafebrechit = cms.EDProducer("SusyCAF_EcalRecHit",
                                 InputTag = cms.InputTag("reducedEcalRecHitsEB"),
                                 Prefix = cms.string('rechitCalo'),
                                 Suffix = cms.string('Eb'),
                                 SeverityLevelCut = cms.int32(2),
                                 )

susycafeerechit = cms.EDProducer("SusyCAF_EcalRecHit",
                                 InputTag = cms.InputTag("reducedEcalRecHitsEE"),
                                 Prefix = cms.string('rechitCalo'),
                                 Suffix = cms.string('Ee'),
                                 SeverityLevelCut = cms.int32(2),                                 
                                 )

#severity levels defined here: http://cmssw.cvs.cern.ch/cgi-bin/cmssw.cgi/CMSSW/RecoLocalCalo/EcalRecAlgos/interface/EcalSeverityLevelAlgo.h?view=log
