import FWCore.ParameterSet.Config as cms

susycafhcaldeadchannels = cms.EDProducer("SusyCAF_HcalDeadChannels",
                                         StatusMask = cms.uint32(1<<0 | 1<<1 | 1<<5 | 1<<6 | 1<<18 | 1<<19),
                                         #status values are described here: https://twiki.cern.ch/twiki/bin/view/CMS/HcalDataValidationWorkflow
                                         )
