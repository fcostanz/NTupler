import FWCore.ParameterSet.Config as cms

susycafhcalnoisesummary = cms.EDProducer("SusyCAF_HcalNoiseSummary",
                            InputTag = cms.InputTag('hcalnoise'),
                            Prefix = cms.string('hns'),
                            Suffix = cms.string(''),
                            )
