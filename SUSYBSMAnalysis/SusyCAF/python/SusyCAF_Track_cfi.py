import FWCore.ParameterSet.Config as cms

susycaftrack = cms.EDProducer ( "SusyCAF_Track",
                                InputTag          = cms.InputTag('generalTracks'),
                                PrimaryVertexTag  = cms.InputTag('offlinePrimaryVertices'),
                                Prefix            = cms.string('tracks'),
                                Suffix            = cms.string(''),
                                MaxD0             = cms.double(5), # wide enough to include tracks from most b-jets?
                                Qualities         = cms.vint32(2), # highPurity (see reco::TrackBase::TrackQuality)
                                PixelSeedOnly     = cms.bool(False),
                                ScrapingVars      = cms.bool(True),
                                )

