#ifndef SUSY_CAF_BEAMSPOT
#define SUSY_CAF_BEAMSPOT

#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Utilities/interface/InputTag.h"

class SusyCAF_BeamSpot : public edm::EDProducer 
{
public: 
  explicit SusyCAF_BeamSpot(const edm::ParameterSet&);
private: 
  void produce( edm::Event &, const edm::EventSetup & );

  const edm::InputTag   inputTag;
  const std::string     prefix,suffix;
};

#endif
