#ifndef SUSY_CAF_LOGERROR
#define SUSY_CAF_LOGERROR

#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Utilities/interface/InputTag.h"
#include <set>

class SusyCAF_LogError : public edm::EDProducer {
 public: 
  explicit SusyCAF_LogError(const edm::ParameterSet&);
 private: 
  void produce( edm::Event &, const edm::EventSetup & );
  const edm::InputTag inputTag;
  const std::string Prefix,Suffix;
  const std::set<std::string> modulesOfInterest, categoriesOfInterest;

  std::set<std::string> setFromVector(std::vector<std::string>);
};

#endif
