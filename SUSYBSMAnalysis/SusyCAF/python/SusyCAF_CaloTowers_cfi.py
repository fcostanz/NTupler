import FWCore.ParameterSet.Config as cms

susycafcalotowers = cms.EDProducer("SusyCAF_CT",
                            InputTag = cms.InputTag('towerMaker'),
                            Prefix = cms.string('CT'),
                            Suffix = cms.string('calo')
                            )
