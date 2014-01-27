import FWCore.ParameterSet.Config as cms

susycafL1triggers = cms.EDProducer("SusyCAF_L1Triggers",
                                   InputTag = cms.InputTag('gtDigis'),
                                   NBx = cms.int32(0)
                                   )

susycafL1triggersP2 = susycafL1triggers.clone( NBx =  2)
susycafL1triggersP1 = susycafL1triggers.clone( NBx =  1)
susycafL1triggersM1 = susycafL1triggers.clone( NBx = -1)
susycafL1triggersM2 = susycafL1triggers.clone( NBx = -2)
