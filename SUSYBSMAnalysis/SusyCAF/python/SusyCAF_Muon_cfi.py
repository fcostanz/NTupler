import FWCore.ParameterSet.Config as cms

susycafmuonreco = cms.EDProducer("SusyCAF_RecoMuon",
                                 InputTag = cms.InputTag('muons'),
                                 SelectedMuons = cms.InputTag(''),
                                 Prefix = cms.string('muon'),
                                 Suffix = cms.string('Global')
                                 )

susycafmuon = cms.EDProducer("SusyCAF_PatMuon",
                             InputTag = cms.InputTag('cleanPatMuons'),
                             SelectedMuons = cms.InputTag(''),
                             Prefix = cms.string('muon'),
                             Suffix = cms.string('Pat')
                             )


susycafpfmuon = cms.EDProducer("SusyCAF_PatMuon",
                               Prefix = cms.string('muon'),
                               SelectedMuons = cms.InputTag('selectedPatMuonsPF'),
                               InputTag = cms.InputTag("patAllMuonsPF"),
                               Suffix = cms.string('PF'),
                               )



