#ifndef SUSY_CAF_HCALNOISERBX
#define SUSY_CAF_HCALNOISERBX

#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Utilities/interface/InputTag.h"
#include "DataFormats/METReco/interface/HcalNoiseRBX.h"

class SusyCAF_HcalNoiseRBX : public edm::EDProducer {
 public: 
  explicit SusyCAF_HcalNoiseRBX(const edm::ParameterSet&);
 private: 
  void produce( edm::Event &, const edm::EventSetup & );
  void doHpdVariables(edm::Event&, edm::Handle<reco::HcalNoiseRBXCollection>);
  const edm::InputTag inputTag;
  const std::string Prefix,Suffix;
  bool outputHpdVariables;
  double energyThresholdForRecHitCount;
  double energyThresholdForRecHitTimeRbx;
  float  energyThresholdForRecHitTimeHpd;
  static const unsigned int nHpdsPerHbheRbx=4;
};

#endif
