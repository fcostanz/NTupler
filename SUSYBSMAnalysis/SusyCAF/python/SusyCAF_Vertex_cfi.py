import FWCore.ParameterSet.Config as cms

susycafvertex = cms.EDProducer( "SusyCAF_Vertex"
                              , InputTag  = cms.InputTag('offlinePrimaryVertices')
                              , Prefix    = cms.string('vertex')
                              , Suffix    = cms.string('')
                              )
