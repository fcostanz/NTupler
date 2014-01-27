#include "FWCore/Framework/interface/MakerMacros.h"
#include "DataFormats/JetReco/interface/CaloJet.h"
#include "DataFormats/PatCandidates/interface/Jet.h"
#include "DataFormats/PatCandidates/interface/Muon.h"
#include "DataFormats/PatCandidates/interface/Electron.h"

#include "DataFormats/PatCandidates/interface/Tau.h"
#include "DataFormats/TauReco/interface/PFTau.h"

#include "DataFormats/HcalRecHit/interface/HcalRecHitCollections.h"

#include "SUSYBSMAnalysis/SusyCAF/interface/SusyTree.h"
#include "SUSYBSMAnalysis/SusyCAF/interface/ProductReducer.h"
#include "SUSYBSMAnalysis/SusyCAF/interface/SusyCAF_Event.h"
#include "SUSYBSMAnalysis/SusyCAF/interface/SusyCAF_Track.h"
#include "SUSYBSMAnalysis/SusyCAF/interface/SusyCAF_Triggers.h"
#include "SUSYBSMAnalysis/SusyCAF/interface/SusyCAF_L1Triggers.h"
#include "SUSYBSMAnalysis/SusyCAF/interface/SusyCAF_MET.h"
#include "SUSYBSMAnalysis/SusyCAF/interface/SusyCAF_SumP4.h"
#include "SUSYBSMAnalysis/SusyCAF/interface/SusyCAF_Jet.h"
#include "SUSYBSMAnalysis/SusyCAF/interface/SusyCAF_Photon.h"
#include "SUSYBSMAnalysis/SusyCAF/interface/SusyCAF_Electron.h"
#include "SUSYBSMAnalysis/SusyCAF/interface/SusyCAF_Muon.h"
#include "SUSYBSMAnalysis/SusyCAF/interface/SusyCAF_BeamSpot.h"
#include "SUSYBSMAnalysis/SusyCAF/interface/SusyCAF_BeamHaloSummary.h"
#include "SUSYBSMAnalysis/SusyCAF/interface/SusyCAF_LogError.h"
#include "SUSYBSMAnalysis/SusyCAF/interface/SusyCAF_Vertex.h"
#include "SUSYBSMAnalysis/SusyCAF/interface/SusyCAF_L1GlobalTrigger.h"
#include "SUSYBSMAnalysis/SusyCAF/interface/SusyCAF_L1CaloTrigger.h"
#include "SUSYBSMAnalysis/SusyCAF/interface/SusyCAF_L1Extra.h"
#include "SUSYBSMAnalysis/SusyCAF/interface/SusyCAF_PileupSummary.h"
#include "SUSYBSMAnalysis/SusyCAF/interface/SusyCAF_HcalNoiseFilter.h"
#include "SUSYBSMAnalysis/SusyCAF/interface/SusyCAF_HcalNoiseSummary.h"
#include "SUSYBSMAnalysis/SusyCAF/interface/SusyCAF_HcalNoiseSummaryExists.h"
#include "SUSYBSMAnalysis/SusyCAF/interface/SusyCAF_HcalNoiseRBX.h"
#include "SUSYBSMAnalysis/SusyCAF/interface/SusyCAF_HcalRecHit.h"
#include "SUSYBSMAnalysis/SusyCAF/interface/SusyCAF_EcalRecHit.h"
#include "SUSYBSMAnalysis/SusyCAF/interface/SusyCAF_PFRecHit.h"
#include "SUSYBSMAnalysis/SusyCAF/interface/SusyCAF_CaloTowers.h"
#include "SUSYBSMAnalysis/SusyCAF/interface/SusyCAF_EcalDeadChannels.h"
#include "SUSYBSMAnalysis/SusyCAF/interface/SusyCAF_HcalDeadChannels.h"
#include "SUSYBSMAnalysis/SusyCAF/interface/SusyCAF_Double.h"
#include "SUSYBSMAnalysis/SusyCAF/interface/SusyCAF_PFTau.h"

#include "SUSYBSMAnalysis/SusyCAF/interface/SusyCAF_Gen.h"
#include "SUSYBSMAnalysis/SusyCAF/interface/SusyCAF_AllTracks.h"

#include "SUSYBSMAnalysis/SusyCAF/interface/SusyCAF_DQMFlags.h"
#include "SUSYBSMAnalysis/SusyCAF/interface/SusyCAF_DCSBits.h"

#include "SUSYBSMAnalysis/SusyCAF/interface/SusyCAF_TrackerIsolation.h"


typedef SusyCAF_MET<reco::GenMET> SusyCAF_GenMET;
typedef SusyCAF_MET<reco::CaloMET> SusyCAF_CaloMET;
typedef SusyCAF_MET<reco::PFMET> SusyCAF_PFMET;
typedef SusyCAF_MET<pat::MET> SusyCAF_PatMET;
typedef SusyCAF_Jet<reco::PFJet> SusyCAF_PFJet;
typedef SusyCAF_Jet<reco::JPTJet> SusyCAF_JPTJet;
typedef SusyCAF_Jet<reco::CaloJet> SusyCAF_CaloJet;
typedef SusyCAF_Jet<pat::Jet> SusyCAF_PatJet;
typedef SusyCAF_Photon<reco::Photon> SusyCAF_RecoPhoton;
typedef SusyCAF_Photon<pat::Photon> SusyCAF_PatPhoton;
typedef SusyCAF_Electron<reco::GsfElectron> SusyCAF_GsfElectron;
typedef SusyCAF_Electron<pat::Electron> SusyCAF_PatElectron;
typedef SusyCAF_Muon<reco::Muon> SusyCAF_RecoMuon;
typedef SusyCAF_Muon<pat::Muon> SusyCAF_PatMuon;
typedef SusyCAF_PFTau<reco::PFTau> SusyCAF_pfTau;
typedef SusyCAF_PFTau<pat::Tau> SusyCAF_PatTau;
typedef SusyCAF_Gen<reco::GenParticle> SusyCAF_GenParticle;
typedef SusyCAF_AllTracks<reco::Track> SusyCAF_alltracks;

typedef SusyCAF_HcalRecHit<HBHERecHitCollection> SusyCAF_HBHERecHitsSorted;
typedef SusyCAF_HcalRecHit<HFRecHitCollection> SusyCAF_HFRecHitsSorted;
typedef SusyCAF_HcalRecHit<HORecHitCollection> SusyCAF_HORecHitsSorted;
typedef SusyCAF_CaloTowers<CaloTowerCollection> SusyCAF_CT;

DEFINE_FWK_MODULE(SusyTree);
DEFINE_FWK_MODULE(ProductReducer);
DEFINE_FWK_MODULE(SusyCAF_Event);
DEFINE_FWK_MODULE(SusyCAF_Track);
DEFINE_FWK_MODULE(SusyCAF_alltracks);
DEFINE_FWK_MODULE(SusyCAF_Triggers);
DEFINE_FWK_MODULE(SusyCAF_L1Triggers);
DEFINE_FWK_MODULE(SusyCAF_GenMET);
DEFINE_FWK_MODULE(SusyCAF_CaloMET);
DEFINE_FWK_MODULE(SusyCAF_PFMET);
DEFINE_FWK_MODULE(SusyCAF_PatMET);
DEFINE_FWK_MODULE(SusyCAF_SumP4);
DEFINE_FWK_MODULE(SusyCAF_Double);
DEFINE_FWK_MODULE(SusyCAF_PFJet);
DEFINE_FWK_MODULE(SusyCAF_JPTJet);
DEFINE_FWK_MODULE(SusyCAF_CaloJet);
DEFINE_FWK_MODULE(SusyCAF_PatJet);
DEFINE_FWK_MODULE(SusyCAF_RecoPhoton);
DEFINE_FWK_MODULE(SusyCAF_PatPhoton);
DEFINE_FWK_MODULE(SusyCAF_GsfElectron);
DEFINE_FWK_MODULE(SusyCAF_PatElectron);
DEFINE_FWK_MODULE(SusyCAF_RecoMuon);
DEFINE_FWK_MODULE(SusyCAF_PatMuon);
DEFINE_FWK_MODULE(SusyCAF_BeamSpot);
DEFINE_FWK_MODULE(SusyCAF_BeamHaloSummary);
DEFINE_FWK_MODULE(SusyCAF_LogError);
DEFINE_FWK_MODULE(SusyCAF_Vertex);
DEFINE_FWK_MODULE(SusyCAF_L1GlobalTrigger);
DEFINE_FWK_MODULE(SusyCAF_L1CaloTrigger);
DEFINE_FWK_MODULE(SusyCAF_L1Extra);
DEFINE_FWK_MODULE(SusyCAF_PileupSummary);
DEFINE_FWK_MODULE(SusyCAF_HcalNoiseFilter);
DEFINE_FWK_MODULE(SusyCAF_HcalNoiseSummary);
DEFINE_FWK_MODULE(SusyCAF_HcalNoiseSummaryExists);
DEFINE_FWK_MODULE(SusyCAF_HcalNoiseRBX);
DEFINE_FWK_MODULE(SusyCAF_HBHERecHitsSorted);
DEFINE_FWK_MODULE(SusyCAF_HFRecHitsSorted);
DEFINE_FWK_MODULE(SusyCAF_HORecHitsSorted);
DEFINE_FWK_MODULE(SusyCAF_EcalRecHit);
DEFINE_FWK_MODULE(SusyCAF_PFRecHit);
DEFINE_FWK_MODULE(SusyCAF_CT);
DEFINE_FWK_MODULE(SusyCAF_EcalDeadChannels);
DEFINE_FWK_MODULE(SusyCAF_HcalDeadChannels);


DEFINE_FWK_MODULE(SusyCAF_PatTau);
DEFINE_FWK_MODULE(SusyCAF_pfTau);

DEFINE_FWK_MODULE(SusyCAF_GenParticle);

DEFINE_FWK_MODULE(SusyCAF_DQMFlags);
DEFINE_FWK_MODULE(SusyCAF_DCSBits);

DEFINE_FWK_MODULE(SusyCAF_TrackerIsolation);
