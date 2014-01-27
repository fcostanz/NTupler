import FWCore.ParameterSet.Config as cms

def kargs() :
    return {
        "rhoIsoInputTag": cms.InputTag("kt6PFJetsForIsolation", "rho"),
        "isoValInputTags": cms.VInputTag(cms.InputTag('elPFIsoValueCharged03PFIdPFIso'),
                                         cms.InputTag('elPFIsoValueGamma03PFIdPFIso'),
                                         cms.InputTag('elPFIsoValueNeutral03PFIdPFIso')
                                         ),
        }


susydesypatelectrons = cms.EDProducer("SusyDESY_Electron",
                                      Prefix       = cms.string('DESYelectron'),
                                      Suffix       = cms.string('Pat'),
                                      
                                      PatElectrons = cms.InputTag('selectedPatElectrons'),
                                      beamSpotInputTag = cms.InputTag('offlineBeamSpot'),
                                      conversionsInputTag = cms.InputTag('allConversions'),
                                      **kargs()
                                      )

susydesypfelectrons = cms.EDProducer("SusyDESY_Electron",
                                     Prefix       = cms.string('DESYelectron'),
                                     Suffix       = cms.string('PF'),
                                     
                                     PatElectrons = cms.InputTag('patAllElectronsPF'),
                                     beamSpotInputTag = cms.InputTag('offlineBeamSpot'),
                                     conversionsInputTag = cms.InputTag('allConversions'),
                                     **kargs()
                                     )

susydesypatmuons = cms.EDProducer("SusyDESY_Muon",
                               Prefix       = cms.string('DESYmuon'),
                               Suffix       = cms.string('Pat'),
                                   
                               PatMuons     = cms.InputTag('selectedPatMuons'),
                               )

susydesypfmuons = cms.EDProducer("SusyDESY_Muon",
                               Prefix       = cms.string('DESYmuon'),
                               Suffix       = cms.string('PF'),
                                   
                               PatMuons     = cms.InputTag('patAllMuonsPF'),
                               )

susydesyjets = cms.EDProducer("SusyDESY_Jet",
                              Prefix          = cms.string('DESYak5JetPF'),
                              Suffix          = cms.string('Pat'),
                              VertexInputTag  = cms.InputTag("offlinePrimaryVertices"),
                              JetsTag         = cms.InputTag('selectionsusycafak5pfjetMatched0'),
                              SelectedJetsTag = cms.InputTag('cleanPatJetsAK5PF'),
                              JetCorrections  = cms.vstring('L1FastJet', 
                                                            'L2Relative', 
                                                            'L3Absolute'),
                              Discriminators  = cms.VInputTag("puJetMva:full53xDiscriminant","puJetMva:full5xDiscriminant"),
                              Flags           = cms.VInputTag("puJetMva:full53xId","puJetMva:full5xId"),
                              JECfile         = cms.string('SUSYBSMAnalysis/DesySusy/data/JECuncertainties/Fall12_V7_MC_Uncertainty_AK5PF.txt')
                              )

susydesytrack = cms.EDProducer("SusyDESY_Track",
                               PfCandidatesTag     = cms.InputTag("particleFlow"),
                               VertexInputTag      = cms.InputTag("offlinePrimaryVertices"),
                               dR_ConeSize         = cms.double(0.3),
                               dz_CutValue         = cms.double(0.1),
                               Prefix       = cms.string('DESYtracks'),
                               Suffix       = cms.string('PF')
                               )

susydesytau =  cms.EDProducer("SusyDESY_PatTau",
                              InputTag = cms.InputTag('selectedPatTaus'), 
                              Prefix = cms.string('DESYtau'),
                              Suffix = cms.string('Pat'),
                              TauIds = cms.vstring('byVLooseCombinedIsolationDeltaBetaCorr',
                                                   'byLooseCombinedIsolationDeltaBetaCorr',
                                                   'byMediumCombinedIsolationDeltaBetaCorr',
                                                   'byTightCombinedIsolationDeltaBetaCorr',
                                                   'byCombinedIsolationDeltaBetaCorrRaw',
                                                   'byLooseCombinedIsolationDeltaBetaCorr3Hits',
                                                   'byMediumCombinedIsolationDeltaBetaCorr3Hits',
                                                   'byTightCombinedIsolationDeltaBetaCorr3Hits',
                                                   'byCombinedIsolationDeltaBetaCorrRaw3Hits',
                                                   'byLooseIsolationMVA',
                                                   'byMediumIsolationMVA',
                                                   'byTightIsolationMVA',
                                                   'byIsolationMVAraw',
                                                   'byLooseIsolationMVA2',
                                                   'byMediumIsolationMVA2',
                                                   'byTightIsolationMVA2',
                                                   'byIsolationMVA2raw',
                                                   'decayModeFinding')
                              )

EventFilterFromList  = cms.EDFilter("SusyDESY_EventFilterFromList",
                                      ListFile  = cms.string('')
                                      )

susydesytobtecfakesProducer = cms.EDProducer("SusyDESY_TobTecFakesProducer",
                                             minEta = cms.double(0.9), # beginning of transition region for "jet" search
                                             maxEta = cms.double(1.6), # end of transition region for "jet" search
                                             phiWindow = cms.double(0.7), # size of phi region for "jet" search
                                             filter = cms.bool(True), # if true, only events passing filter (bad events) will pass
                                             trackCollection = cms.InputTag("generalTracks"), # track collection to use
                                             ratioAllCut = cms.double(-1.0), # minimum ratio of TOBTEC-seeded tracks / pixelseeded tracks
                                             ratioJetCut = cms.double(3.0),  # minimum ratio of TOBTEC-seeded tracks / pixelseeded tracks in jet
                                             absJetCut = cms.double(20.0)    # minimum number of TOBTEC-seeded tracks in "
                                             )


susydesytrigger = cms.EDProducer("SusyDESY_Trigger",
                                 Prefix       = cms.string('DESYtrigger'),
                                 Suffix       = cms.string(''),
                                 inputTag     = cms.InputTag('susycaftriggers:triggered'),
                                 muons        = cms.InputTag("muonTriggerMatchHLTMuonsEmbedder"),
                                 electrons    = cms.InputTag("electronTriggerMatchHLTElectronsEmbedder")
                                 )

susydesypu = cms.EDProducer("SusyDESY_PileUp",
                            PileUp     = cms.InputTag('susycafpileupsummary:pileupTrueNumInteractionsBX0')
                            )

susydesyscanTemp = cms.EDProducer("SusyDESY_Scan",
                                  InputTag  = cms.InputTag('source'),
                                  Prefix    = cms.string('susyScan'),
                                  Suffix    = cms.string(''),
                                  ScanFormat = cms.string(''),
                                  ScanParameters = cms.vstring(),
                                  Debug    = cms.untracked.bool (False)
                                  )

muonTriggerMatchHLTMuons = cms.EDProducer(
  "PATTriggerMatcherDRLessByR"
  , src     = cms.InputTag( 'cleanPatMuons' )
  , matched = cms.InputTag( 'patTrigger' )
  , matchedCuts = cms.string('filter("*") && (type( "TriggerMuon" ) || type( "TriggerL1Mu" ))')
  #, matchedCuts = cms.string('path("*")')
  #, matchedCuts = cms.string('path("HLT_IsoMu17_eta2p1_TriCentralPFJet30_v5",0)')
  #hltL1sMu16Eta2p1
  , maxDeltaR   = cms.double( 0.5 )
  , resolveAmbiguities    = cms.bool( True )
  , resolveByMatchQuality = cms.bool( True )
)

electronTriggerMatchHLTElectrons = muonTriggerMatchHLTMuons.clone()
electronTriggerMatchHLTElectrons.src = 'cleanPatElectrons'
electronTriggerMatchHLTElectrons.matchedCuts = 'filter("*") && (type("TriggerElectron") || type("TriggerL1IsoEG"))'

muonTriggerMatchHLTMuonsEmbedder = cms.EDProducer(
  "PATTriggerMatchMuonEmbedder",
  src = cms.InputTag("cleanPatMuons"),
  matches = cms.VInputTag("muonTriggerMatchHLTMuons")
)

electronTriggerMatchHLTElectronsEmbedder = cms.EDProducer(
  "PATTriggerMatchElectronEmbedder",
  src = cms.InputTag("cleanPatElectrons"),
  matches = cms.VInputTag("electronTriggerMatchHLTElectrons")
)









