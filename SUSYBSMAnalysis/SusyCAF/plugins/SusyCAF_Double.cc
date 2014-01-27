#include "SUSYBSMAnalysis/SusyCAF/interface/SusyCAF_Double.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

SusyCAF_Double::SusyCAF_Double(const edm::ParameterSet& iConfig) :
  inputTag(iConfig.getParameter<edm::InputTag>("InputTag")),
  Prefix(iConfig.getParameter<std::string>("Prefix")),
  Suffix(iConfig.getParameter<std::string>("Suffix"))
{
  produces<bool>( Prefix + "HandleValid" + Suffix);
  produces<double>( Prefix + Suffix );
}

void SusyCAF_Double::
produce( edm::Event& iEvent, const edm::EventSetup& iSetup ) {
  edm::Handle<double> handle;
  iEvent.getByLabel(inputTag,handle);
  
  iEvent.put( std::auto_ptr<bool>( new bool(handle.isValid()) ),
	      Prefix + "HandleValid" + Suffix  );
  iEvent.put( std::auto_ptr<double>( new double( handle.isValid() ? *handle.product(): -1000. ) ),
	      Prefix + Suffix );
}
