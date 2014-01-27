#ifndef SUSY_CAF_VERTEX
#define SUSY_CAF_VERTEX

#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Utilities/interface/InputTag.h"

class SusyCAF_Vertex : public edm::EDProducer 
{
public: 
  explicit SusyCAF_Vertex(const edm::ParameterSet&);
private: 
  void produce( edm::Event &, const edm::EventSetup & );

  const edm::InputTag   inputTag;
  const std::string     prefix,suffix;
};

#endif
