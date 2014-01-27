#ifndef SUSY_CAF_PFTAU
#define SUSY_CAF_PFTAU

#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/Utilities/interface/InputTag.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "DataFormats/TauReco/interface/PFTauDiscriminator.h"
#include "DataFormats/PatCandidates/interface/Tau.h"
#include "DataFormats/TauReco/interface/PFTau.h"
#include <string>
using namespace std;

template< typename T >
class SusyCAF_PFTau : public edm::EDProducer {
 public: 
 explicit SusyCAF_PFTau(const edm::ParameterSet&);
 private:
  void initTemplate();
  void initRECO();
  void initPAT();
  void produce(edm::Event &, const edm::EventSetup & );
  void produceTemplate(edm::Event &, const edm::EventSetup &, edm::Handle<std::vector<T> > &);
  void produceRECO(edm::Event &, const edm::EventSetup &, edm::Handle<std::vector<T> > &);
  void producePAT(edm::Event &, const edm::EventSetup &, edm::Handle<std::vector<T> > &);

  typedef reco::Candidate::LorentzVector LorentzVector;
  const edm::InputTag inputTag;
  const std::string Prefix,Suffix;
  bool hps;
};

template< typename T >
SusyCAF_PFTau<T>::SusyCAF_PFTau(const edm::ParameterSet& iConfig) :
  inputTag(iConfig.getParameter<edm::InputTag>("InputTag")),
  Prefix(iConfig.getParameter<std::string>("Prefix")),
  Suffix(iConfig.getParameter<std::string>("Suffix")),
  hps(iConfig.getUntrackedParameter<bool>("isHps", false))
{

  initTemplate();
}

template< typename T >
void SusyCAF_PFTau<T>::
produce(edm::Event& iEvent, const edm::EventSetup& iSetup) {
  edm::Handle<std::vector<T> > collection;
  iEvent.getByLabel(inputTag,collection);

  produceTemplate(iEvent, iSetup, collection);
}

template<typename T>
void SusyCAF_PFTau<T>::initRECO()
{
  produces <bool> (Prefix + "HandleValid" + Suffix);
  produces <std::vector<LorentzVector> > ( Prefix + "P4" + Suffix );
  produces <std::vector<int> > (  Prefix + "Charge" + Suffix);
  produces <std::vector<math::XYZPoint> > (Prefix + "Vertex" + Suffix);

  produces <std::vector<int> > (Prefix + "NumSigTrks" + Suffix);
  produces <std::vector<int> > (Prefix + "NumIsoTrks" + Suffix);
  produces <std::vector<int> > (Prefix + "NumSigPFNeutHadrCands" + Suffix);  

  produces <std::vector<double> > (Prefix + "SumPtIsoPFNeutCands" + Suffix);
  produces <std::vector<double> > (Prefix + "NeutCandsTotEnergy" + Suffix);
  produces <std::vector<double> > (Prefix + "NeutCandsHoverHPlusE" + Suffix);
  

  if (hps){
      produces <std::vector<int> > (Prefix + "TauIdagainstElectron" + Suffix);
      produces <std::vector<int> > (Prefix + "TauIdagainstMuon" + Suffix);
      produces <std::vector<int> > (Prefix + "TauIdByDecay" + Suffix);
      produces <std::vector<int> > (Prefix + "TauIdByLooseIsolation" + Suffix);
      produces <std::vector<int> > (Prefix + "TauIdByMediumIsolation" + Suffix);
      produces <std::vector<int> > (Prefix + "TauIdByTightIsolation" + Suffix);
  }
}

template< typename T >
void SusyCAF_PFTau<T>::initPAT()
{

  produces <std::vector<double> > (Prefix + "TrkIso" + Suffix);
  produces <std::vector<double> >(Prefix + "EcalIso" + Suffix);
  produces <std::vector<double> >(Prefix + "HcalIso" +Suffix);
  produces <std::vector<double> >(Prefix + "CaloIso" + Suffix);

  produces <std::vector<float> > (Prefix + "ParticleIso" + Suffix);
  produces <std::vector<float> > (Prefix + "ChargedHadronIso" + Suffix);
  produces <std::vector<float> > (Prefix + "NeutralHadronIso" + Suffix);
  produces <std::vector<float> > (Prefix + "PhotonIso" + Suffix);
  produces <std::vector<float> > (Prefix + "TauIdbyIsolation" + Suffix);
  produces <std::vector<float> > (Prefix + "TauIdbyTaNC" + Suffix);
  produces <std::vector<float> > (Prefix + "TauIdbyTaNCfrHalfPercent" + Suffix);
  produces <std::vector<float> > (Prefix + "TauIdbyTaNCfrOnePercent" + Suffix);
  produces <std::vector<float> > (Prefix + "TauIdbyTaNCfrQuarterPercent" + Suffix);
  produces <std::vector<float> > (Prefix + "TauIdbyTaNCfrTenthPercent" + Suffix);
  produces <std::vector<float> > (Prefix + "TauIdecalIsolation" + Suffix);
  produces <std::vector<float> > (Prefix + "TauIdecalIsolationUsingLeadingPion" + Suffix);
  produces <std::vector<float> > (Prefix + "TauIdleadingPionPtCut" + Suffix);
  produces <std::vector<float> > (Prefix + "TauIdleadingTrackFinding" + Suffix);
  produces <std::vector<float> > (Prefix + "TauIdleadingTrackPtCut" + Suffix);
  produces <std::vector<float> > (Prefix + "TauIdtrackIsolation" + Suffix);
  produces <std::vector<float> > (Prefix + "TauIdtrackIsolationUsingLeadingPion" + Suffix);
  produces <std::vector<float> > (Prefix + "TauIdbyIsolationUsingLeadingPion" + Suffix);
  if (!hps)  produces <std::vector<float> > (Prefix + "TauIdagainstElectron" + Suffix);
  if (!hps)  produces <std::vector<float> > (Prefix + "TauIdagainstMuon" + Suffix);

}




template< typename T >
void SusyCAF_PFTau<T>::
produceRECO(edm::Event& iEvent, const edm::EventSetup& iSetup, edm::Handle<std::vector<T> >& collection) {

  std::auto_ptr<bool> isHandleValid (new bool(collection.isValid()) );
  std::auto_ptr<std::vector<LorentzVector> > p4 ( new std::vector<LorentzVector>() );
  std::auto_ptr<std::vector<int> >  charge   ( new std::vector<int>()  ) ;
  std::auto_ptr<std::vector<math::XYZPoint> > vertex (new std::vector<math::XYZPoint>() );

  std::auto_ptr<std::vector<int> > numSigTrks (new std::vector<int>() );
  std::auto_ptr<std::vector<int> > numIsoTrks (new std::vector<int>() );
  std::auto_ptr<std::vector<int> > numSigPFNeuHadCands (new std::vector<int> () );
  std::auto_ptr<std::vector<double> > SumPtIsoTrks (new std::vector<double> () );
  std::auto_ptr<std::vector<double> > NeutCandstotE (new std::vector<double> () );
  std::auto_ptr<std::vector<double> > NeutCandsHoHplusE (new std::vector<double> () );
  std::auto_ptr<std::vector<int> > againstele  (new std::vector<int> () );
  std::auto_ptr<std::vector<int> > againstmu (new std::vector<int> () );
  std::auto_ptr<std::vector<int> > bydecay  (new std::vector<int> () );
  std::auto_ptr<std::vector<int> > bylooseisolation (new std::vector<int> () );
  std::auto_ptr<std::vector<int> > bymediumisolation  (new std::vector<int> () );
  std::auto_ptr<std::vector<int> > bytightisolation (new std::vector<int> () );

  std::vector< reco::PFTauDiscriminator > discr;
  discr.clear();
  if (hps){
    edm::Handle<reco::PFTauDiscriminator> AgEle;
    edm::Handle<reco::PFTauDiscriminator> AgMu;
    edm::Handle<reco::PFTauDiscriminator> Decay;
    edm::Handle<reco::PFTauDiscriminator> LIso;
    edm::Handle<reco::PFTauDiscriminator> MIso;
    edm::Handle<reco::PFTauDiscriminator> TIso;
    iEvent.getByLabel("hpsPFTauDiscriminationByDecayModeFinding",Decay);
    iEvent.getByLabel("hpsPFTauDiscriminationByLooseIsolation",LIso);
    iEvent.getByLabel("hpsPFTauDiscriminationByMediumIsolation",MIso);
    iEvent.getByLabel("hpsPFTauDiscriminationByTightIsolation",TIso); 
    iEvent.getByLabel("hpsPFTauDiscriminationByMediumElectronRejection" ,AgEle);
    iEvent.getByLabel("hpsPFTauDiscriminationByLooseMuonRejection",AgMu);
    discr.push_back(*AgEle);
    discr.push_back(*AgMu);
    discr.push_back(*Decay);
    discr.push_back(*LIso);
    discr.push_back(*MIso);
    discr.push_back(*TIso);
   }


  
  if(collection.isValid()){
    uint itau=0;
    for(typename std::vector<T>::const_iterator it = collection->begin(); it!=collection->end(); it++) {
      p4->push_back(it->p4());
      charge->push_back(it->charge());
      
      bool pfVertex = (it->signalPFChargedHadrCands().size() && 
		       it->signalPFChargedHadrCands().begin()->isNonnull() &&
		       (*it->signalPFChargedHadrCands().begin())->trackRef().isNonnull() );
      vertex->push_back( !pfVertex ? it->vertex() : (*it->signalPFChargedHadrCands().begin())->trackRef()->vertex() );
         
      numSigTrks->push_back(it->signalTracks().size());
      numIsoTrks->push_back((it->isolationTracks()).size());
      numSigPFNeuHadCands->push_back((it->signalPFNeutrHadrCands()).size());
      double sumptisotrks=0.0;
      for(reco::track_iterator isotrk = (it->isolationTracks()).begin(); isotrk!=(it->isolationTracks()).end(); isotrk++){
	sumptisotrks+=(*isotrk)->pt();
      }

      SumPtIsoTrks->push_back(sumptisotrks);

      double ecaltauen = 0.0, hcaltauen = 0.0;

      if(it->signalPFNeutrHadrCands().isAvailable()){
	for(reco::PFCandidateRefVector::const_iterator sigNCands = (it->signalPFNeutrHadrCands()).begin(); sigNCands!=(it->signalPFNeutrHadrCands()).end(); sigNCands++){
	  ecaltauen += (*sigNCands)->ecalEnergy();
	  hcaltauen += (*sigNCands)->hcalEnergy();
	}
      }
      NeutCandstotE->push_back(ecaltauen+hcaltauen);
      if(ecaltauen+hcaltauen>0.0){
	NeutCandsHoHplusE->push_back(hcaltauen/(hcaltauen+ecaltauen));
      }
  if (hps){
    againstele->push_back(int((discr[0][itau]).second));
    againstmu->push_back(int((discr[1][itau]).second)); 
    bydecay->push_back(int((discr[2][itau]).second));    
    bylooseisolation->push_back(int((discr[3][itau]).second)); 
    bymediumisolation->push_back(int((discr[4][itau]).second)); 
    bytightisolation->push_back(int((discr[5][itau]).second)); 
       }
itau++;
   } 

 }
  
  iEvent.put(isHandleValid, Prefix + "HandleValid" + Suffix);
  iEvent.put( p4,  Prefix + "P4" + Suffix );
  iEvent.put(charge,  Prefix + "Charge" + Suffix);
  iEvent.put(vertex ,Prefix + "Vertex" + Suffix);
  iEvent.put(numSigTrks, Prefix + "NumSigTrks" + Suffix);
  iEvent.put(numIsoTrks, Prefix + "NumIsoTrks" + Suffix);
  iEvent.put(numSigPFNeuHadCands, Prefix + "NumSigPFNeutHadrCands" + Suffix);  
  iEvent.put(SumPtIsoTrks, Prefix + "SumPtIsoPFNeutCands" + Suffix);
  iEvent.put(NeutCandstotE, Prefix + "NeutCandsTotEnergy" + Suffix);
  iEvent.put(NeutCandsHoHplusE, Prefix + "NeutCandsHoverHPlusE" + Suffix);
  if (hps){
    iEvent.put(againstele, Prefix + "TauIdagainstElectron" + Suffix);
    iEvent.put(againstmu, Prefix + "TauIdagainstMuon" + Suffix);
    iEvent.put(bydecay, Prefix + "TauIdByDecay" + Suffix);
    iEvent.put(bylooseisolation, Prefix + "TauIdByLooseIsolation" + Suffix);
    iEvent.put(bymediumisolation, Prefix + "TauIdByMediumIsolation" + Suffix); 
    iEvent.put(bytightisolation, Prefix + "TauIdByTightIsolation" + Suffix); 
  }
}
  
  // extra information stored for PAT data
template< typename T >
void SusyCAF_PFTau<T>::
producePAT(edm::Event& iEvent, const edm::EventSetup& iSetup, edm::Handle<std::vector<T> >& collection) {
  
  std::auto_ptr<std::vector<double> > trkiso (new std::vector<double>());
  std::auto_ptr<std::vector<double> > ecaliso (new std::vector<double>());
  std::auto_ptr<std::vector<double> > hcaliso (new std::vector<double>());
  std::auto_ptr<std::vector<double> > caloiso (new std::vector<double>());
  
  std::auto_ptr<std::vector<float> > partIso (new std::vector<float>() );
  std::auto_ptr<std::vector<float> > charHadIso (new std::vector<float>() );
  std::auto_ptr<std::vector<float> > neutHadIso (new std::vector<float>() );
  std::auto_ptr<std::vector<float> > photIso (new std::vector<float>() );
  
  
  std::map<std::string, std::vector<float> *> TauDiscs; //fill map with pointers rather than auto_ptr
  TauDiscs["againstElectron"] = new std::vector<float>();
  TauDiscs["againstMuon"] = new std::vector<float>();
  TauDiscs["byIsolation"] = new std::vector<float>();
  TauDiscs["byTaNC"] = new std::vector<float>();
  TauDiscs["byTaNCfrHalfPercent"] = new std::vector<float>();
  TauDiscs["byTaNCfrOnePercent"] = new std::vector<float>();
  TauDiscs ["byTaNCfrQuarterPercent"] = new std::vector<float>();
  TauDiscs ["byTaNCfrTenthPercent"] = new std::vector<float>();
  TauDiscs ["ecalIsolation"] = new std::vector<float>();
  TauDiscs ["ecalIsolationUsingLeadingPion"] = new std::vector<float>();
  TauDiscs ["leadingPionPtCut"] = new std::vector<float>();
  TauDiscs ["leadingTrackFinding"] = new std::vector<float>();
  TauDiscs ["leadingTrackPtCut"] = new std::vector<float>();
  TauDiscs ["trackIsolation"] = new std::vector<float>();
  TauDiscs ["trackIsolationUsingLeadingPion"] = new std::vector<float>();
  TauDiscs ["byIsolationUsingLeadingPion"] = new std::vector<float>();
  
  if( collection.isValid()) {
    for(std::vector<pat::Tau>::const_iterator it = collection->begin(); it!=collection->end(); it++){
      
      trkiso->push_back(it->trackIso());
      ecaliso->push_back(it->ecalIso());
      hcaliso->push_back(it->hcalIso());
      caloiso->push_back(it->caloIso());
      
      partIso->push_back(it->particleIso());
      charHadIso->push_back(it->chargedHadronIso());
      neutHadIso->push_back(it->neutralHadronIso());
      photIso->push_back(it->photonIso());
      
      
      for(std::vector<std::pair<std::string, float> >::const_iterator tauid=it->tauIDs().begin(); tauid!=it->tauIDs().end(); tauid++) {
	if(TauDiscs.count(tauid->first)) 
	  TauDiscs[tauid->first]->push_back(tauid->second);
	else 
	  std::cout<<"Unrecoginsed TauDiscriminator =" << tauid->first << std::endl;
      }
    }
  }
  
  iEvent.put(trkiso, Prefix + "TrkIso" + Suffix);
  iEvent.put(ecaliso, Prefix + "EcalIso" + Suffix);
  iEvent.put(hcaliso, Prefix + "HcalIso"+ Suffix);
  iEvent.put(caloiso, Prefix + "CaloIso" + Suffix);
  iEvent.put(partIso, Prefix + "ParticleIso" + Suffix);
  iEvent.put(charHadIso, Prefix + "ChargedHadronIso" + Suffix);
  iEvent.put(neutHadIso, Prefix + "NeutralHadronIso" + Suffix);
  iEvent.put(photIso, Prefix + "PhotonIso" + Suffix);
  for(std::map<std::string, std::vector<float> *>::iterator tauid = TauDiscs.begin();
	tauid != TauDiscs.end(); tauid++){
    iEvent.put(std::auto_ptr<std::vector<float> >(tauid->second), Prefix + "TauId" + tauid->first + Suffix);
  }
  
}

#endif
  
