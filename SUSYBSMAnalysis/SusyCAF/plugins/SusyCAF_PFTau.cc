#include "SUSYBSMAnalysis/SusyCAF/interface/SusyCAF_PFTau.h"


// init method in case of RECO data
template<>
void SusyCAF_PFTau<reco::PFTau>::initTemplate() { initRECO(); }

// init method in case of PAT data
template<>
void SusyCAF_PFTau<pat::Tau>::initTemplate()  { initRECO(); initPAT(); }

// produce method in case of RECO data
template<>
void SusyCAF_PFTau<reco::PFTau>::produceTemplate(edm::Event& iEvent, const edm::EventSetup& iSetup, edm::Handle<std::vector<reco::PFTau> >& collection)
{
  produceRECO(iEvent, iSetup, collection);
}

// produce method in case of PAT data
template<>
void SusyCAF_PFTau<pat::Tau>::produceTemplate(edm::Event& iEvent, const edm::EventSetup& iSetup, edm::Handle<std::vector<pat::Tau> >& collection)
{
  produceRECO(iEvent, iSetup, collection);
  producePAT(iEvent, iSetup, collection);
}
