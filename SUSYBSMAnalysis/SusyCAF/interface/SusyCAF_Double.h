#ifndef SUSY_CAF_DOUBLE
#define SUSY_CAF_DOUBLE

#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Utilities/interface/InputTag.h"

class SusyCAF_Double : public edm::EDProducer {
 public: 
  explicit SusyCAF_Double(const edm::ParameterSet&);
 private: 
  void produce( edm::Event &, const edm::EventSetup & );
  const edm::InputTag inputTag;
  const std::string Prefix,Suffix;
};

#endif
