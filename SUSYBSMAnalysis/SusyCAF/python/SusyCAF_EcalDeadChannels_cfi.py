import FWCore.ParameterSet.Config as cms

susycafecaldeadchannels = cms.EDProducer("SusyCAF_EcalDeadChannels",
                                         #*Branch  :EcalTriggerPrimitiveDigisSorted_ecalDigis_EcalTriggerPrimitives_SUSYCAF. * fromRAW=1
                                         #InputTag = cms.InputTag("ecalDigis","EcalTriggerPrimitives"),
                                         #
                                         #*Branch  :EcalTriggerPrimitiveDigisSorted_ecalTPSkim__RECO. * stored in RECO
                                         InputTag = cms.InputTag("ecalTPSkim"),
                                         MinStatus = cms.uint32(12)
                                         )
