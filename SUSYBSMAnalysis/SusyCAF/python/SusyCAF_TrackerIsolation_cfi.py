import FWCore.ParameterSet.Config as cms

susycafTrackerIsolation = cms.EDProducer("SusyCAF_TrackerIsolation",
                                         InputTag = cms.InputTag("particleFlow"),
                                         vertexInputTag = cms.InputTag("offlinePrimaryVertices"),
                                         Prefix = cms.string('trkIso'),
                                         Suffix = cms.string(''),
                                         dR_ConeSize = cms.double(0.3),
                                         dz_CutValue = cms.double(0.05),
                                         minPt_PFCandidate = cms.double(0.0)
                                         )

