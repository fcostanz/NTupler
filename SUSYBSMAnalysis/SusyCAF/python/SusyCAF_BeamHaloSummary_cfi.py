import FWCore.ParameterSet.Config as cms

susycafbeamhalosummary = cms.EDProducer("SusyCAF_BeamHaloSummary",
                                        Prefix = cms.string('beamHalo'),
                                        Suffix = cms.string(''),
                                        InputTag = cms.InputTag('BeamHaloSummary')
                                        )
