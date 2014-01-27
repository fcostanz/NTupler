import FWCore.ParameterSet.Config as cms

susycafPFtau = cms.EDProducer("SusyCAF_pfTau",
                            InputTag = cms.InputTag('fixedConePFTauProducer'), #new name in CMSSW 3x (was pfRecoTauProducer in 22x)
                            Prefix = cms.string('tau'),
                            Suffix = cms.string('PFReco')
                            )

susycaftau = cms.EDProducer("SusyCAF_PatTau",
                            InputTag = cms.InputTag('cleanPatTaus'), #new name in CMSSW 3x (was pfRecoTauProducer in 22x)
                            Prefix = cms.string('tau'),
                            Suffix = cms.string('Pat')
                            )

susycafpftau = cms.EDProducer("SusyCAF_PatTau",
                            InputTag = cms.InputTag('selectedPatTausPF'), 
                            Prefix = cms.string('tau'),
                            Suffix = cms.string('PF')
                            )

susycafHPStau = cms.EDProducer("SusyCAF_pfTau",
    Prefix = cms.string('tauHPS'),   
    InputTag = cms.InputTag("hpsPFTauProducer"),
    Suffix = cms.string('PF'),
    isHps= cms.untracked.bool(True)
)
