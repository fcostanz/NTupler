#include "SUSYBSMAnalysis/SusyCAF/interface/SusyCAF_HcalNoiseFilter.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

SusyCAF_HcalNoiseFilter::SusyCAF_HcalNoiseFilter(const edm::ParameterSet& iConfig) :
  inputTag(iConfig.getParameter<edm::InputTag>("InputTag")),
  Prefix(iConfig.getParameter<std::string>("Prefix")),
  Suffix(iConfig.getParameter<std::string>("Suffix"))
{
  produces<bool>( Prefix + "HandleValid" + Suffix);
  produces<bool>( Prefix + "FilterResult" + Suffix);
}

void SusyCAF_HcalNoiseFilter::
produce( edm::Event& iEvent, const edm::EventSetup& iSetup ) {
  edm::Handle<bool> hcalnoisefilter;
  iEvent.getByLabel(inputTag,hcalnoisefilter);
  
  iEvent.put( std::auto_ptr<bool>( new bool(hcalnoisefilter.isValid()) ),
	      Prefix + "HandleValid" + Suffix  );
  iEvent.put( std::auto_ptr<bool>( new bool(hcalnoisefilter.isValid()? *hcalnoisefilter : true) ),
	      Prefix + "FilterResult" + Suffix );
}
