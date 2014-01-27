import FWCore.ParameterSet.Config as cms

susycafcalotau = cms.EDProducer("SusyCAF_CalTau",
                            InputTag = cms.InputTag('caloRecoTauProducer'),
                            Prefix = cms.string('tau'),
                            Suffix = cms.string('calo')
                            )
