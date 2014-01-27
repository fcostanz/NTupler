#define SUSY_CAF_CALOTOWERS

#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Utilities/interface/InputTag.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include <string>


template< typename T >
class SusyCAF_CaloTowers : public edm::EDProducer {
 public: 
  explicit SusyCAF_CaloTowers(const edm::ParameterSet&);
 private: 
  void produce(edm::Event &, const edm::EventSetup & );

  typedef math::XYZVectorD Vector3D;
  const edm::InputTag inputTag;
  const std::string Prefix,Suffix;
};

template< typename T >
SusyCAF_CaloTowers<T>::SusyCAF_CaloTowers(const edm::ParameterSet& iConfig) :
  inputTag(iConfig.getParameter<edm::InputTag>("InputTag")),
  Prefix(iConfig.getParameter<std::string>("Prefix")),
  Suffix(iConfig.getParameter<std::string>("Suffix"))
{
  produces <std::vector<Vector3D> > ( Prefix + "ECALEsum"  + Suffix );
  produces <std::vector<Vector3D> > ( Prefix + "HCALEsum"  + Suffix );
}

template< typename T >
void SusyCAF_CaloTowers<T>::
produce(edm::Event& iEvent, const edm::EventSetup& iSetup) {
  std::auto_ptr<std::vector<Vector3D> >  ECALEsum   ( new std::vector<Vector3D>()  ) ;
  std::auto_ptr<std::vector<Vector3D> >  HCALEsum   ( new std::vector<Vector3D>()  ) ;

  // Get CaloTowers
  edm::Handle<CaloTowerCollection> calotowers;
  iEvent.getByLabel(inputTag,calotowers);

  double tECALEsumx = 0.;
  double tECALEsumy = 0.;
  double tECALEsumz = 0.;
  double tHCALEsumx = 0.;
  double tHCALEsumy = 0.;
  double tHCALEsumz = 0.;
  
  for(CaloTowerCollection::const_iterator itow = calotowers->begin(); itow!=calotowers->end(); ++itow) {
    //if the calotower energy is not zero, add up all the energies
    //    if(itow->energy()==0.) continue;
    double emFrac = itow->emEnergy()/itow->energy();
    double hadFrac = itow->hadEnergy()/itow->energy();
    tECALEsumx += itow->px()*emFrac;
    tECALEsumy += itow->py()*emFrac;
    tECALEsumz += itow->pz()*emFrac;
    tHCALEsumx += itow->px()*hadFrac;
    tHCALEsumy += itow->py()*hadFrac;
    tHCALEsumz += itow->pz()*hadFrac;
  }

  //fill the 3vectors

  Vector3D tECALEsum(tECALEsumx, tECALEsumy, tECALEsumz);
  Vector3D tHCALEsum(tHCALEsumx, tHCALEsumy, tHCALEsumz);

  ECALEsum->push_back(tECALEsum);
  HCALEsum->push_back(tHCALEsum);

  iEvent.put( ECALEsum,  Prefix + "ECALEsum" + Suffix );
  iEvent.put( HCALEsum,  Prefix + "HCALEsum" + Suffix );

}
