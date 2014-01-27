import FWCore.ParameterSet.Config as cms

susycafhbherechit = cms.EDProducer("SusyCAF_HBHERecHitsSorted",
                                   InputTag = cms.InputTag('reducedHcalRecHits','hbhereco'),
                                   Prefix = cms.string('rechitCalo'),
                                   Suffix = cms.string('Hbhe'),
                                   SeverityLevelCut = cms.int32(11),
                                   ProduceExtraVariables = cms.bool(False),
                                   SingleRbxThresholds = cms.vdouble(10.0), #GeV
                                   SingleRmThresholds = cms.vdouble(5.0), #GeV
                                   SingleChannelThreshold = cms.double(1.0), #GeV
                                   )

susycafhfrechit = cms.EDProducer("SusyCAF_HFRecHitsSorted",
                                 InputTag = cms.InputTag('reducedHcalRecHits','hfreco'),
                                 Prefix = cms.string('rechitCalo'),
                                 Suffix = cms.string('Hf'),
                                 SeverityLevelCut = cms.int32(11),
                                 ProduceExtraVariables = cms.bool(False),
                                 SingleRbxThresholds = cms.vdouble(10.0), #GeV
                                 SingleRmThresholds = cms.vdouble(5.0), #GeV
                                 SingleChannelThreshold = cms.double(1.0), #GeV
                                 )

susycafhorechit = cms.EDProducer("SusyCAF_HORecHitsSorted",
                                 InputTag = cms.InputTag('reducedHcalRecHits','horeco'),
                                 Prefix = cms.string('rechitCalo'),
                                 Suffix = cms.string('Ho'),
                                 SeverityLevelCut = cms.int32(11),
                                 ProduceExtraVariables = cms.bool(False),
                                 SingleRbxThresholds = cms.vdouble(10.0), #GeV
                                 SingleRmThresholds = cms.vdouble(5.0), #GeV
                                 SingleChannelThreshold = cms.double(1.0), #GeV
                                 )
