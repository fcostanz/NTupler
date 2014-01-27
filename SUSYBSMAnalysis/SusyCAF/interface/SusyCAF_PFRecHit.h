#ifndef SUSY_CAF_PFRECHIT
#define SUSY_CAF_PFRECHIT

#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Utilities/interface/InputTag.h"

class SusyCAF_PFRecHit : public edm::EDProducer {
 public: 
  explicit SusyCAF_PFRecHit(const edm::ParameterSet&);
 private: 
  void produce(edm::Event&,const edm::EventSetup&);
  const edm::InputTag inputTag;
  const std::string Prefix,Suffix;
};

#endif
