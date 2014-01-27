import FWCore.ParameterSet.Config as cms

susycafalltracks = cms.EDProducer("SusyCAF_alltracks",
                            InputTag = cms.InputTag('generalTracks'),
                            Prefix = cms.string('allTracks'),
                            Suffix = cms.string(''),
                            MinPt = cms.double(-1.)
                            )
