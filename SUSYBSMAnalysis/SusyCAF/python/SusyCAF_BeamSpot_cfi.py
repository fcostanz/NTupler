import FWCore.ParameterSet.Config as cms

susycafbeamspot = cms.EDProducer( "SusyCAF_BeamSpot"
                              , InputTag  = cms.InputTag('offlineBeamSpot')
                              , Prefix    = cms.string('beamSpot')
                              , Suffix    = cms.string('')
                              )
