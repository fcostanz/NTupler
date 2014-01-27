#ifndef SUSY_CAF_L1GLOBALTRIGGER
#define SUSY_CAF_L1GLOBALTRIGGER

#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Utilities/interface/InputTag.h"
#include <set>

class SusyCAF_L1GlobalTrigger : public edm::EDProducer {
 public: 
  explicit SusyCAF_L1GlobalTrigger(const edm::ParameterSet&);
 private: 
  void produce( edm::Event &, const edm::EventSetup & );
  void endJob();
  void fillAlgoNameListNoUnderScores();

  const edm::InputTag inputTag;
  int nBxOutput;
  bool storeByName;

  std::vector<std::string> algoNameList;
  std::vector<std::string> algoNameListNoUnderScores;  

  std::set<std::string> algosPresentInData;
  std::set<std::string> menusAlreadyParsed;
};

#endif
