#ifndef SUSY_CAF_HCALNOISESUMMARYEXISTS
#define SUSY_CAF_HCALNOISESUMMARYEXISTS

#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDFilter.h"
#include "FWCore/Framework/interface/Event.h"
#include "DataFormats/METReco/interface/HcalNoiseSummary.h"

class SusyCAF_HcalNoiseSummaryExists : public edm::EDFilter {
 public: 
  explicit SusyCAF_HcalNoiseSummaryExists(const edm::ParameterSet&) {}
 private: 
  bool filter( edm::Event & iEvent, const edm::EventSetup & ) { 
    edm::Handle<HcalNoiseSummary> summary_h;
    iEvent.getByType(summary_h);
    return summary_h.isValid();
  }
};

#endif
