#ifndef SUSY_CAF_L1EXTRA
#define SUSY_CAF_L1EXTRA

#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Utilities/interface/InputTag.h"

class SusyCAF_L1Extra : public edm::EDProducer {
 public: 
  explicit SusyCAF_L1Extra(const edm::ParameterSet&);
 private: 
  void produce( edm::Event &, const edm::EventSetup & );
  const edm::InputTag inputTag;
  const std::string Prefix,Suffix;
};

#endif
