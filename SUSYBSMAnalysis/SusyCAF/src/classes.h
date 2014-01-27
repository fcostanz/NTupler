#ifndef SUSYCAF_CLASSES_H
#define SUSYCAF_CLASSES_H

#include "DataFormats/Common/interface/Wrapper.h"
#include <string>
#include <map>
#include "Math/LorentzVector.h"
#include "Math/PtEtaPhiE4D.h"
#include "Math/PtEtaPhiM4D.h"


namespace {
  struct dictionary {
    std::map<std::string,bool> dummy0;
    edm::Wrapper<std::map<std::string,bool> > dummy1;

    std::map<std::string,int> dummi0;
    edm::Wrapper<std::map<std::string,int> > dummi1;

    std::map<std::string,std::string> dummee0;
    edm::Wrapper<std::map<std::string,std::string> > dummee1;

    ROOT::Math::PtEtaPhiM4D<float> dumdum;
  };
}

#endif 
