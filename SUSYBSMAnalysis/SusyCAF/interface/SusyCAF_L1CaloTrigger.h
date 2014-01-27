#ifndef SUSY_CAF_L1CALOTRIGGER
#define SUSY_CAF_L1CALOTRIGGER

#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "DataFormats/L1CaloTrigger/interface/L1CaloRegionDetId.h"
#include "FWCore/Utilities/interface/InputTag.h"

class SusyCAF_L1CaloTrigger : public edm::EDProducer {
 public: 
  explicit SusyCAF_L1CaloTrigger(const edm::ParameterSet&);
 private: 
  void produce( edm::Event &, const edm::EventSetup & );
  const edm::InputTag inputTag;
  const std::string Prefix,Suffix;

  void setupGeometry();
  static const int nPhi=18;
  double phi[nPhi];
};

#endif
