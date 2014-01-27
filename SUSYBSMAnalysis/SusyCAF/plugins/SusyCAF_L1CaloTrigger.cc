#include "SUSYBSMAnalysis/SusyCAF/interface/SusyCAF_L1CaloTrigger.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "DataFormats/L1GlobalCaloTrigger/interface/L1GctCollections.h"
#include "DataFormats/L1CaloTrigger/interface/L1CaloCollections.h"
#include "TMath.h"

SusyCAF_L1CaloTrigger::SusyCAF_L1CaloTrigger(const edm::ParameterSet& iConfig) :
  inputTag(iConfig.getParameter<edm::InputTag>("InputTag")),
  Prefix(iConfig.getParameter<std::string>("Prefix")),
  Suffix(iConfig.getParameter<std::string>("Suffix"))
{
  produces <std::vector<unsigned int> > ( Prefix + "ht"                       + Suffix);
  produces <std::vector<unsigned int> > ( Prefix + "et"                       + Suffix);
  produces <std::vector<unsigned int> > ( Prefix + "met"                      + Suffix);
  produces <std::vector<unsigned int> > ( Prefix + "metPhi"                   + Suffix);
  produces <std::vector<unsigned int> > ( Prefix + "metOverflow"              + Suffix);
  produces <std::vector<unsigned int> > ( Prefix + "mht"                      + Suffix);
  produces <std::vector<unsigned int> > ( Prefix + "mhtPhi"                   + Suffix);
  produces <double>                     ( Prefix + "regionMet"                + Suffix);
  produces <double>                     ( Prefix + "regionMetPhi"             + Suffix);
  produces <int>                        ( Prefix + "nRegionOverflows"         + Suffix);
  produces <int>                        ( Prefix + "regionEtMax"              + Suffix);
  produces <int>                        ( Prefix + "regioniPhiMax"            + Suffix);
  produces <int>                        ( Prefix + "regioniEtaMax"            + Suffix);
  
  setupGeometry();
}

void SusyCAF_L1CaloTrigger::setupGeometry()
{
  for (int iPhi=0;iPhi<nPhi;iPhi++) {
    phi[iPhi]=(iPhi+0.0)*2.0*TMath::Pi()/nPhi;
    if (phi[iPhi]>=TMath::Pi()) phi[iPhi]-=2.0*TMath::Pi();
  }
}

void SusyCAF_L1CaloTrigger::
produce(edm::Event& iEvent, const edm::EventSetup& iSetup) {
  std::auto_ptr<std::vector<unsigned int> > ht                        ( new std::vector<unsigned int>() );
  std::auto_ptr<std::vector<unsigned int> > et                        ( new std::vector<unsigned int>() );
  std::auto_ptr<std::vector<unsigned int> > met                       ( new std::vector<unsigned int>() );
  std::auto_ptr<std::vector<unsigned int> > metPhi                    ( new std::vector<unsigned int>() );
  std::auto_ptr<std::vector<unsigned int> > metOverflow               ( new std::vector<unsigned int>()         );
  std::auto_ptr<std::vector<unsigned int> > mht                       ( new std::vector<unsigned int>() );
  std::auto_ptr<std::vector<unsigned int> > mhtPhi                    ( new std::vector<unsigned int>() );
  std::auto_ptr<double>                     regionMet                 ( new double()                    );
  std::auto_ptr<double>                     regionMetPhi              ( new double()                    );
  std::auto_ptr<int>                        nRegionOverflows          ( new int()                       );
  std::auto_ptr<int>                        regionEtMax               ( new int()                       );
  std::auto_ptr<int>                        regioniPhiMax             ( new int()                       );
  std::auto_ptr<int>                        regioniEtaMax             ( new int()                       );

  //todo: l1 jets, electrons

  //ET
  edm::Handle<L1GctEtTotalCollection> l1GctEtTotals;
  iEvent.getByLabel(inputTag,l1GctEtTotals);
  for(L1GctEtTotalCollection::const_iterator it = l1GctEtTotals->begin(); it != l1GctEtTotals->end(); ++it) {
    et->push_back(it->et());
  }
  
  //HT
  edm::Handle<L1GctEtHadCollection> l1GctEtHads;
  iEvent.getByLabel(inputTag,l1GctEtHads);
  for(L1GctEtHadCollection::const_iterator it = l1GctEtHads->begin(); it != l1GctEtHads->end(); ++it) {
    ht->push_back(it->et());
  }

  //MET
  edm::Handle<L1GctEtMissCollection> l1GctEtMisss;
  iEvent.getByLabel(inputTag,l1GctEtMisss);
  for(L1GctEtMissCollection::const_iterator it = l1GctEtMisss->begin(); it != l1GctEtMisss->end(); ++it) {
    met->push_back(it->et());
    metPhi->push_back(it->phi());
    metOverflow->push_back(it->overFlow());
  }

  //MHT
  edm::Handle<L1GctHtMissCollection> l1GctHtMisss;
  iEvent.getByLabel(inputTag,l1GctHtMisss);
  for(L1GctHtMissCollection::const_iterator it = l1GctHtMisss->begin(); it != l1GctHtMisss->end(); ++it) {
    mht->push_back(it->et());
    mhtPhi->push_back(it->phi());
  }

  //compute MET from regions
  double ex=0.0;
  double ey=0.0;
  int nOverFlows=0;

  L1CaloRegionCollection::const_iterator itMax;
  int etLocalMax=-1;

  edm::Handle<L1CaloRegionCollection> l1CaloRegions;
  iEvent.getByLabel(inputTag,l1CaloRegions);
  for(L1CaloRegionCollection::const_iterator it = l1CaloRegions->begin(); it != l1CaloRegions->end(); ++it) {
    if (it->overFlow()) nOverFlows++;
    int etLocal=it->et();
    double phiLocal=phi[it->id().iphi()];
    ex+=etLocal*cos(phiLocal);
    ey+=etLocal*sin(phiLocal);
    if (etLocal>etLocalMax) {
      etLocalMax=etLocal;
      itMax=it;
    }
  }
  *nRegionOverflows.get()=nOverFlows;
  *regionMet.get()=sqrt(ex*ex+ey*ey);
  *regionMetPhi.get()=atan2(-ey,-ex);

  if (etLocalMax>-1) {
    *regionEtMax.get()=itMax->et();
    *regioniPhiMax.get()=itMax->id().iphi();
    *regioniEtaMax.get()=itMax->id().ieta();
  }
  else {
    *regionEtMax.get()=-1;
    *regioniPhiMax.get()=-1;
    *regioniEtaMax.get()=-1;
  }
  
  iEvent.put( ht,                       Prefix + "ht"                        + Suffix);
  iEvent.put( et,                       Prefix + "et"                        + Suffix);
  iEvent.put( met,                      Prefix + "met"                       + Suffix);
  iEvent.put( metPhi,                   Prefix + "metPhi"                    + Suffix);
  iEvent.put( metOverflow,              Prefix + "metOverflow"               + Suffix);
  iEvent.put( mht,                      Prefix + "mht"                       + Suffix);
  iEvent.put( mhtPhi,                   Prefix + "mhtPhi"                    + Suffix);
  iEvent.put( regionMet,                Prefix + "regionMet"                 + Suffix);
  iEvent.put( regionMetPhi,             Prefix + "regionMetPhi"              + Suffix);
  iEvent.put( nRegionOverflows,         Prefix + "nRegionOverflows"          + Suffix);
  iEvent.put( regionEtMax,              Prefix + "regionEtMax"               + Suffix);
  iEvent.put( regioniPhiMax,            Prefix + "regioniPhiMax"             + Suffix);
  iEvent.put( regioniEtaMax,            Prefix + "regioniEtaMax"             + Suffix);
}
