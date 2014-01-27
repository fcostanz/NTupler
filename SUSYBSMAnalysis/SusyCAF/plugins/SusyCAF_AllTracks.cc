#include "SUSYBSMAnalysis/SusyCAF/interface/SusyCAF_AllTracks.h"


// init method in case of RECO data
template<>
void SusyCAF_AllTracks<reco::Track>::initTemplate() { initRECO(); }

// produce method in case of RECO data
template<>
void SusyCAF_AllTracks<reco::Track>::produceTemplate(edm::Event& iEvent, const edm::EventSetup& iSetup, edm::Handle<std::vector<reco::Track> >& collection)
{
  produceRECO(iEvent, iSetup, collection);
}
