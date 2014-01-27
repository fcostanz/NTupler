import FWCore.ParameterSet.Config as cms

susycaftriggers = cms.EDProducer("SusyCAF_Triggers",
                                 InputTag = cms.InputTag('TriggerResults'),
                                 SourceName = cms.string(""),
                                 TriggerEventInputTag = cms.InputTag('hltTriggerSummaryAOD'),
                                 )

