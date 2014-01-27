#ifndef SUSY_CAF_ALLTRACKS
#define SUSY_CAF_ALLTRACKS

#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Utilities/interface/InputTag.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "DataFormats/TrackReco/interface/Track.h"
#include "DataFormats/TrackReco/interface/TrackBase.h"
#include "DataFormats/TrackReco/interface/TrackFwd.h"

#include "DataFormats/VertexReco/interface/VertexFwd.h"
#include "DataFormats/VertexReco/interface/Vertex.h"
#include "DataFormats/Candidate/interface/Candidate.h"

#include "DataFormats/BeamSpot/interface/BeamSpot.h"
#include "SUSYBSMAnalysis/SusyCAF/interface/SusyCAF_functions.h"

#include <string>
#include <vector>

template< typename T >
class SusyCAF_AllTracks : public edm::EDProducer {
 public: 
  explicit SusyCAF_AllTracks(const edm::ParameterSet&);
 private: 
  void initTemplate();
  void initRECO();
  void produce(edm::Event &, const edm::EventSetup & );
  void produceTemplate(edm::Event &, const edm::EventSetup &, edm::Handle<std::vector<T> > &);
  void produceRECO(edm::Event &, const edm::EventSetup &, edm::Handle<std::vector<T> > &);

  typedef math::XYZVector Vector;
  typedef reco::Candidate::LorentzVector LorentzVector;

  const edm::InputTag inputTag;
  const std::string Prefix,Suffix;
  double MinPt;
};

template< typename T >
SusyCAF_AllTracks<T>::SusyCAF_AllTracks(const edm::ParameterSet& iConfig) :
  inputTag(iConfig.getParameter<edm::InputTag>("InputTag")),
     Prefix(iConfig.getParameter<std::string>("Prefix")),
     Suffix(iConfig.getParameter<std::string>("Suffix")),
     MinPt(iConfig.getParameter<double>("MinPt"))
{
  initTemplate();
}

template< typename T >
void SusyCAF_AllTracks<T>::initRECO()
{
  produces <bool> (  Prefix + "HandleValid" + Suffix);
  produces <std::vector<LorentzVector> > ( Prefix + "TrkMomentum" + Suffix );
  produces <std::vector<double> > ( Prefix + "TrkChi2" + Suffix );
  produces <std::vector<double> > ( Prefix + "TrkNdof" + Suffix );
  produces <std::vector<double> > ( Prefix + "TrkNormChi2" + Suffix );
  produces <std::vector<int> > ( Prefix + "TrkCharge" + Suffix );
  produces <std::vector<double> > ( Prefix + "TrkPtError" + Suffix );
  produces <std::vector<double> > ( Prefix + "TrkDxy" + Suffix );
  produces <std::vector<double> > ( Prefix + "TrkDxyError" + Suffix );
  produces <std::vector<double> > ( Prefix + "TrkDz" + Suffix );
  produces <std::vector<double> > ( Prefix + "TrkDzError" + Suffix );
  produces <std::vector<double> > ( Prefix + "TrkDxyRelBeamPos" + Suffix );
  produces <std::vector<double> > ( Prefix + "TrkDxyVrtxPos" + Suffix );
  produces <std::vector<unsigned> > ( Prefix + "TrkNumberOfValidHits" + Suffix );
}


template< typename T >
void SusyCAF_AllTracks<T>::
produce(edm::Event& iEvent, const edm::EventSetup& iSetup) {
  edm::Handle<std::vector<T> > collection;
  iEvent.getByLabel(inputTag,collection);

  produceTemplate(iEvent, iSetup, collection);
}


template< typename T >
void SusyCAF_AllTracks<T>::
produceRECO(edm::Event& iEvent, const edm::EventSetup& iSetup, edm::Handle<std::vector<T> >& collection) {
  std::auto_ptr<bool> isHandleValid ( new bool(collection.isValid()) );
  std::auto_ptr<std::vector<LorentzVector> > trkMomentum ( new std::vector<LorentzVector>() );
  std::auto_ptr<std::vector<double> > trkChi2 ( new std::vector<double>() );
  std::auto_ptr<std::vector<double> > trkNdof ( new std::vector<double>() );
  std::auto_ptr<std::vector<double> > trkNormChi2 ( new std::vector<double>() );
  std::auto_ptr<std::vector<int> > trkCharge ( new std::vector<int>() );
  std::auto_ptr<std::vector<double> > trkPtError ( new std::vector<double>() );
  std::auto_ptr<std::vector<double> > trkDxy ( new std::vector<double>() );
  std::auto_ptr<std::vector<double> > trkDxyError ( new std::vector<double>() );
  std::auto_ptr<std::vector<double> > trkDz ( new std::vector<double>() );
  std::auto_ptr<std::vector<double> > trkDzError ( new std::vector<double>() );
  std::auto_ptr<std::vector<double> > trkDxyRelBeamPos ( new std::vector<double>() );
  std::auto_ptr<std::vector<double> > trkDxyVrtxPos ( new std::vector<double>() );
  std::auto_ptr<std::vector<unsigned> > trkNumberOfValidHits ( new std::vector<unsigned>() );


  edm::Handle<reco::BeamSpot> beamspots;  iEvent.getByLabel("offlineBeamSpot", beamspots); 
  edm::Handle<reco::VertexCollection> vertices; iEvent.getByLabel("offlinePrimaryVertices", vertices); 

  math::XYZPoint bs = math::XYZPoint(0.,0.,0.);
  //math::XYZPoint vx = math::XYZPoint(0.,0.,0.);//unused


  if (beamspots.isValid()){ bs = beamspots->position(); } 

  const reco::Vertex primaryVertex;

  if (collection.isValid()){

    for(typename std::vector<T>::const_iterator it = collection->begin(); it!=collection->end(); it++) {

      //      if ( it->quality(reco::TrackBase::highPurity) ) { // apply track quality criteria
	
	if (MinPt<0.) { // do not apply a pT cut

	  LorentzVector trackVector(it->momentum().x(),it->momentum().y(),it->momentum().z(),it->p());
	  trkMomentum->push_back(trackVector);
	  trkChi2->push_back(it->chi2());
	  trkNdof->push_back(it->ndof());
	  trkNormChi2->push_back(it->normalizedChi2());
	  trkCharge->push_back(it->charge());
	  trkPtError->push_back(it->ptError());
	  trkDxy->push_back(it->dxy());
	  trkDxyError->push_back(it->dxyError());
	  trkDz->push_back(it->dz());
	  trkDzError->push_back(it->dzError());
	  trkDxyRelBeamPos->push_back(it->dxy(bs));
	  trkDxyVrtxPos->push_back(it->dxy(primaryVertex.position()));
	  trkNumberOfValidHits->push_back(it->numberOfValidHits());

	} //~do not apply a pT cut 
	
	else if ( (it->pt()) > MinPt) { // apply a pT cut
	  
	  LorentzVector trackVector(it->momentum().x(),it->momentum().y(),it->momentum().z(),it->p());
	  trkMomentum->push_back(trackVector);
	  trkChi2->push_back(it->chi2());
	  trkNdof->push_back(it->ndof());
	  trkNormChi2->push_back(it->normalizedChi2());
	  trkCharge->push_back(it->charge());
	  trkPtError->push_back(it->ptError());
	  trkDxy->push_back(it->dxy());
	  trkDxyError->push_back(it->dxyError());
	  trkDz->push_back(it->dz());
	  trkDzError->push_back(it->dzError());
	  trkDxyRelBeamPos->push_back(it->dxy(bs));
	  trkDxyVrtxPos->push_back(it->dxy(primaryVertex.position()));
	  trkNumberOfValidHits->push_back(it->numberOfValidHits());

	} //~apply a pT cut 
	
	//      } // ~end of apply track quality criteria
      
    } // ~end of looping over all tracks 
  } // ~end of collection.IsValid()

  iEvent.put( isHandleValid,  Prefix + "HandleValid" + Suffix );
  iEvent.put( trkMomentum,  Prefix + "TrkMomentum" + Suffix );
  iEvent.put( trkChi2,  Prefix + "TrkChi2" + Suffix );
  iEvent.put( trkNdof,  Prefix + "TrkNdof" + Suffix );
  iEvent.put( trkNormChi2,  Prefix + "TrkNormChi2" + Suffix );
  iEvent.put( trkCharge,  Prefix + "TrkCharge" + Suffix );
  iEvent.put( trkPtError,  Prefix + "TrkPtError" + Suffix );
  iEvent.put( trkDxy,  Prefix + "TrkDxy" + Suffix );
  iEvent.put( trkDxyError,  Prefix + "TrkDxyError" + Suffix );
  iEvent.put( trkDz,  Prefix + "TrkDz" + Suffix );
  iEvent.put( trkDzError,  Prefix + "TrkDzError" + Suffix );
  iEvent.put( trkDxyRelBeamPos,  Prefix + "TrkDxyRelBeamPos" + Suffix );
  iEvent.put( trkDxyVrtxPos,  Prefix + "TrkDxyVrtxPos" + Suffix );
  iEvent.put( trkNumberOfValidHits,  Prefix + "TrkNumberOfValidHits" + Suffix );
}



#endif
