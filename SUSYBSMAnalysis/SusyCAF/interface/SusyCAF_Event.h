#ifndef SUSY_CAF_EVENT
#define SUSY_CAF_EVENT

#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/Framework/interface/Frameworkfwd.h"

class SusyCAF_Event : public edm::EDProducer {
 public: 
  explicit SusyCAF_Event(const edm::ParameterSet&);
 private: 
  void produce( edm::Event &, const edm::EventSetup & );
};

#endif
