#ifndef SUSY_CAF_SCAN
#define SUSY_CAF_SCAN

#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Utilities/interface/InputTag.h"
#include "boost/regex.hpp"
#include "SUSYBSMAnalysis/SusyCAF/interface/SusyCAF_ScanPoint.h"

class SusyCAF_Scan : public edm::EDProducer {
 public: 
  explicit SusyCAF_Scan(const edm::ParameterSet&);
  double convert(std::string);
 private:
  typedef std::map<std::string, double> DoubleParameterMap;
 
  void produce( edm::Event &, const edm::EventSetup & );
  const edm::InputTag inputTag;
  const std::string Prefix,Suffix;
  const boost::regex scanFormat;
  const std::vector<std::string> scanPars;
  const bool debug;
  std::map<ScanPoint, DoubleParameterMap > additionalDoubleVars_;
  DoubleParameterMap additionalDoubleDefaultVars_;
};

#endif
