#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "SUSYBSMAnalysis/SusyCAF/interface/SusyCAF_EcalRecHit.h"
#include "DataFormats/Candidate/interface/Candidate.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "CondFormats/DataRecord/interface/EcalChannelStatusRcd.h"
#include "DataFormats/EcalRecHit/interface/EcalRecHitCollections.h"
#include "RecoLocalCalo/EcalRecAlgos/interface/EcalSeverityLevelAlgo.h"
#include "RecoLocalCalo/EcalRecAlgos/interface/EcalSeverityLevelAlgoRcd.h"
#include "Geometry/Records/interface/CaloGeometryRecord.h"
#include "Geometry/CaloGeometry/interface/CaloGeometry.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

SusyCAF_EcalRecHit::SusyCAF_EcalRecHit(const edm::ParameterSet& iConfig) :
  inputTag(iConfig.getParameter<edm::InputTag>("InputTag")),
  Prefix(iConfig.getParameter<std::string>("Prefix")),
  Suffix(iConfig.getParameter<std::string>("Suffix")),
  severityLevelCut(iConfig.getParameter<int>("SeverityLevelCut"))
{
  typedef ROOT::Math::LorentzVector<ROOT::Math::PtEtaPhiE4D<double> > PolarLorentzV;

  produces <bool>                        ( Prefix + "HandleValid"   + Suffix );
  produces <std::vector<PolarLorentzV> > ( Prefix + "P4"            + Suffix );
  produces <std::vector<float> >         ( Prefix + "Time"          + Suffix );
  produces <std::vector<unsigned> >      ( Prefix + "FlagWord"      + Suffix );
  produces <std::vector<int> >           ( Prefix + "SeverityLevel" + Suffix );
}

void SusyCAF_EcalRecHit::produce(edm::Event& iEvent, const edm::EventSetup& iSetup) {
  typedef ROOT::Math::LorentzVector<ROOT::Math::PtEtaPhiE4D<double> > PolarLorentzV;

  std::auto_ptr<std::vector<PolarLorentzV> > p4(new std::vector<PolarLorentzV>() );
  std::auto_ptr<std::vector<float> >         time          (new std::vector<float>    () );
  std::auto_ptr<std::vector<unsigned> >      flagWord      (new std::vector<unsigned> () );
  std::auto_ptr<std::vector<int> >           severityLevel (new std::vector<int>      () );

  edm::Handle<EcalRecHitCollection> collection;  iEvent.getByLabel(inputTag, collection);
  if (collection.isValid()) {
    edm::ESHandle<EcalSeverityLevelAlgo> severityLevelAlgo; iSetup.get<EcalSeverityLevelAlgoRcd>().get(severityLevelAlgo);
    edm::ESHandle<CaloGeometry> caloGeometry;         iSetup.get<CaloGeometryRecord>().get(caloGeometry);

    for(EcalRecHitCollection::const_iterator it = collection->begin(); it != collection->end(); ++it) {
      const int theSeverityLevel = severityLevelAlgo->severityLevel(it->detid(),*collection);
      if (theSeverityLevel >= severityLevelCut) {
  
  	const GlobalPoint& point = caloGeometry->getPosition(it->detid());
    	const double eta(point.eta()), phi(point.phi()), energy(fabs(it->energy()));
  	
  	p4->push_back(PolarLorentzV(energy/cosh(eta),eta,phi,energy));
  	time->push_back(it->time());
  	flagWord->push_back(it->flags());
  	severityLevel->push_back(theSeverityLevel);
      }  
    }
  }

  iEvent.put( std::auto_ptr<bool>(new bool(collection.isValid())),  Prefix + "HandleValid"   + Suffix );
  iEvent.put( p4,                 Prefix + "P4"                 + Suffix ); 
  iEvent.put( time,               Prefix + "Time"               + Suffix ); 
  iEvent.put( flagWord,           Prefix + "FlagWord"           + Suffix ); 
  iEvent.put( severityLevel,      Prefix + "SeverityLevel"      + Suffix ); 
}
