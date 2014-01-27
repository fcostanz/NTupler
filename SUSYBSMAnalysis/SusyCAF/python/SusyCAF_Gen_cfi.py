import FWCore.ParameterSet.Config as cms

susycafgen = cms.EDProducer("SusyCAF_GenParticle",
                            InputTag = cms.InputTag('genParticles'),
                            JetCollections = cms.VInputTag('ak5GenJets','ak7GenJets'),
                            Prefix = cms.string('gen'),
                            Suffix = cms.string(''),
                            GenStatus1PtCut = cms.double(10.0),
                            GenJetPtCut = cms.double(10.0),
                        )

from SUSYBSMAnalysis.SusyCAF.SusyCAF_MET_cfi import met

susycafgenMetCalo             = met('Gen','genMetCalo',             'Calo',          'genmet', calo=False)            
susycafgenMetCaloAndNonPrompt = met('Gen','genMetCaloAndNonPrompt', 'CaloNonPrompt', 'genmet', calo=False)
susycafgenMetTrue             = met('Gen','genMetTrue',             'True',          'genmet', calo=False)

