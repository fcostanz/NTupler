import FWCore.ParameterSet.Config as cms

susycafhcalnoiserbx = cms.EDProducer("SusyCAF_HcalNoiseRBX",
                            InputTag = cms.InputTag('hcalnoiserbx'),
                            Prefix = cms.string('hnrbx'),
                            Suffix = cms.string(''),
                            OutputHpdVariables = cms.bool(False),
                            EnergyThresholdForRecHitCount   = cms.double(1.5),  #GeV
                            EnergyThresholdForRecHitTimeRbx = cms.double(20.0), #GeV
                            EnergyThresholdForRecHitTimeHpd = cms.double(10.0)  #GeV (float, not double)
                            )
