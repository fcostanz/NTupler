#include "FWCore/Framework/interface/Event.h"
#include "SUSYBSMAnalysis/SusyCAF/interface/SusyCAF_PFRecHit.h"
#include "DataFormats/Candidate/interface/Candidate.h"
#include "DataFormats/ParticleFlowReco/interface/PFRecHit.h"
#include "DataFormats/ParticleFlowReco/interface/PFRecHitFwd.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

SusyCAF_PFRecHit::SusyCAF_PFRecHit(const edm::ParameterSet& iConfig) :
  inputTag(iConfig.getParameter<edm::InputTag>("InputTag")),
  Prefix(iConfig.getParameter<std::string>("Prefix")),
  Suffix(iConfig.getParameter<std::string>("Suffix"))
{
  produces <bool>                                         ( Prefix + "HandleValid" + Suffix );
  produces <std::vector<ROOT::Math::LorentzVector<ROOT::Math::PtEtaPhiE4D<double> > > > ( Prefix + "P4" + Suffix );
}

void SusyCAF_PFRecHit::produce(edm::Event& iEvent, const edm::EventSetup& iSetup) {
  std::auto_ptr<std::vector<ROOT::Math::LorentzVector<ROOT::Math::PtEtaPhiE4D<double> > > > p4(new std::vector<ROOT::Math::LorentzVector<ROOT::Math::PtEtaPhiE4D<double> > >() );

  //get rechits
  edm::Handle<reco::PFRecHitCollection> collection;
  iEvent.getByLabel(inputTag, collection);
  std::auto_ptr<bool> isHandleValid ( new bool(collection.isValid()) );
  
  ROOT::Math::LorentzVector<ROOT::Math::PtEtaPhiE4D<double> > thisP4(0.0,0.0,0.0,0.0);

  if (collection.isValid()) {

    //loop over rechits
    for(reco::PFRecHitCollection::const_iterator it = collection->begin(); it != collection->end(); ++it) {

      reco::PFRecHit pfr(*it);

      double energy=fabs(pfr.energy());//absolute value

      pfr.calculatePositionREP();
      double eta=pfr.positionREP().Eta();
      double phi=pfr.positionREP().Phi();
      
      thisP4.SetCoordinates(energy/cosh(eta),eta,phi,energy);
      //std::cout 
      //<< "pT:  " << energy/cosh(eta) << " " << thisP4.pt()  << std::endl
      //<< "eta: " << eta     << " " << thisP4.eta() << std::endl
      //<< "phi: " << phi     << " " << thisP4.phi() << std::endl
      //<< "e:   " << energy  << " " << thisP4.e()   << std::endl;
      
      p4->push_back(thisP4);
    } //end loop over rechits

  }//end if handle valid

  iEvent.put( isHandleValid,  Prefix + "HandleValid" + Suffix );
  iEvent.put( p4,             Prefix + "P4"          + Suffix ); 
}
