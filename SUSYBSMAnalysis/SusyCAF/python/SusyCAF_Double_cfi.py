import FWCore.ParameterSet.Config as cms

susycafrho = cms.EDProducer(
    "SusyCAF_Double",
    InputTag = cms.InputTag('kt6PFJets:rho'),
    Prefix = cms.string('rho'),
    Suffix = cms.string(''),
    )

susycafrho25 = cms.EDProducer(
    "SusyCAF_Double",
    InputTag = cms.InputTag('kt6PFJetsForIsolation:rho'),
    Prefix = cms.string('rho25'),
    Suffix = cms.string(''),
    )


