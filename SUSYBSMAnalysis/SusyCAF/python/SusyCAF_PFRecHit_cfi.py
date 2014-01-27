import FWCore.ParameterSet.Config as cms

susycafpfrechitclusterecal = cms.EDProducer("SusyCAF_PFRecHit",
                                     InputTag = cms.InputTag("particleFlowClusterECAL","Cleaned"),
                                     Prefix = cms.string('rechitclusterPF'),
                                     Suffix = cms.string('Ecal'),
                                     )

susycafpfrechitclusterhcal = cms.EDProducer("SusyCAF_PFRecHit",
                                     InputTag = cms.InputTag("particleFlowClusterHCAL","Cleaned"),
                                     Prefix = cms.string('rechitclusterPF'),
                                     Suffix = cms.string('Hcal'),
                                     )

susycafpfrechitclusterhfem = cms.EDProducer("SusyCAF_PFRecHit",
                                     InputTag = cms.InputTag("particleFlowClusterHFEM","Cleaned"),
                                     Prefix = cms.string('rechitclusterPF'),
                                     Suffix = cms.string('Hfem'),
                                     )

susycafpfrechitclusterhfhad = cms.EDProducer("SusyCAF_PFRecHit",
                                      InputTag = cms.InputTag("particleFlowClusterHFHAD","Cleaned"),
                                      Prefix = cms.string('rechitclusterPF'),
                                      Suffix = cms.string('Hfhad'),
                                      )

susycafpfrechitclusterps = cms.EDProducer("SusyCAF_PFRecHit",
                                   InputTag = cms.InputTag("particleFlowClusterPS","Cleaned"),
                                   Prefix = cms.string('rechitclusterPF'),
                                   Suffix = cms.string('Ps'),
                                   )

susycafpfrechitecal = cms.EDProducer("SusyCAF_PFRecHit",
                                     InputTag = cms.InputTag("particleFlowRecHitECAL","Cleaned"),
                                     Prefix = cms.string('rechitPF'),
                                     Suffix = cms.string('Ecal'),
                                     )

susycafpfrechithcal = cms.EDProducer("SusyCAF_PFRecHit",
                                     InputTag = cms.InputTag("particleFlowRecHitHCAL","Cleaned"),
                                     Prefix = cms.string('rechitPF'),
                                     Suffix = cms.string('Hcal'),
                                     )

susycafpfrechithfem = cms.EDProducer("SusyCAF_PFRecHit",
                                     InputTag = cms.InputTag("particleFlowRecHitHFEM","Cleaned"),
                                     Prefix = cms.string('rechitPF'),
                                     Suffix = cms.string('Hfem'),
                                     )

susycafpfrechithfhad = cms.EDProducer("SusyCAF_PFRecHit",
                                      InputTag = cms.InputTag("particleFlowRecHitHFHAD","Cleaned"),
                                      Prefix = cms.string('rechitPF'),
                                      Suffix = cms.string('Hfhad'),
                                      )

susycafpfrechitps = cms.EDProducer("SusyCAF_PFRecHit",
                                   InputTag = cms.InputTag("particleFlowRecHitPS","Cleaned"),
                                   Prefix = cms.string('rechitPF'),
                                   Suffix = cms.string('Ps'),
                                   )
