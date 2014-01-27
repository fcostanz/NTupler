#include "SUSYBSMAnalysis/SusyCAF/interface/SusyCAF_Photon.h"


// init method in case of RECO data
template<>
void SusyCAF_Photon<reco::Photon>::initTemplate() { initRECO(); initExtra(); }

// init method in case of PAT data
template<>
void SusyCAF_Photon<pat::Photon>::initTemplate()
{
  initRECO();
  initExtra();
  initPAT();
}

