import FWCore.ParameterSet.Config as cms

susycaflogerror = cms.EDProducer("SusyCAF_LogError",
                                 Prefix = cms.string('logError'),
                                 Suffix = cms.string(''),
                                 InputTag = cms.InputTag('logErrorHarvester'),
                                 ModulesOfInterest = cms.vstring(),
                                 CategoriesOfInterest = cms.vstring("TooManyClusters","TooManySeeds")
                                 )
