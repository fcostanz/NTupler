#include "SUSYBSMAnalysis/SusyCAF/interface/SusyCAF_BeamHaloSummary.h"
#include "FWCore/Framework/interface/Event.h"
#include "DataFormats/METReco/interface/BeamHaloSummary.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

SusyCAF_BeamHaloSummary::SusyCAF_BeamHaloSummary(const edm::ParameterSet& iConfig) :
  inputTag(iConfig.getParameter<edm::InputTag>("InputTag")),
  Prefix(iConfig.getParameter<std::string>("Prefix")),
  Suffix(iConfig.getParameter<std::string>("Suffix"))
{
  produces <bool>  ( Prefix + "HandleValid" + Suffix);

  produces <bool>  ( Prefix + "HcalTightHaloId" + Suffix);
  produces <bool>  ( Prefix + "HcalLooseHaloId" + Suffix);

  produces <bool>  ( Prefix + "EcalTightHaloId" + Suffix);
  produces <bool>  ( Prefix + "EcalLooseHaloId" + Suffix);

  produces <bool>  ( Prefix + "CSCTightHaloId" + Suffix);
  produces <bool>  ( Prefix + "CSCLooseHaloId" + Suffix);

  produces <bool>  ( Prefix + "GlobalTightHaloId" + Suffix);
  produces <bool>  ( Prefix + "GlobalLooseHaloId" + Suffix);

  //produces <bool>  ( Prefix + "AnyTightHaloId" + Suffix);
  //produces <bool>  ( Prefix + "AnyLooseHaloId" + Suffix);
}

void SusyCAF_BeamHaloSummary::
produce(edm::Event& iEvent, const edm::EventSetup& iSetup) {

  edm::Handle<reco::BeamHaloSummary> haloSummary;
  iEvent.getByLabel(inputTag,haloSummary);

  iEvent.put( std::auto_ptr<bool>( new bool(haloSummary.isValid())), Prefix + "HandleValid" + Suffix );
  if(haloSummary.isValid()) {

    iEvent.put(std::auto_ptr<bool>( new bool(haloSummary->HcalLooseHaloId()) ), Prefix + "HcalLooseHaloId" + Suffix);
    iEvent.put(std::auto_ptr<bool>( new bool(haloSummary->HcalTightHaloId()) ), Prefix + "HcalTightHaloId" + Suffix);

    iEvent.put(std::auto_ptr<bool>( new bool(haloSummary->EcalLooseHaloId()) ), Prefix + "EcalLooseHaloId" + Suffix);
    iEvent.put(std::auto_ptr<bool>( new bool(haloSummary->EcalTightHaloId()) ), Prefix + "EcalTightHaloId" + Suffix);
    
    iEvent.put(std::auto_ptr<bool>( new bool(haloSummary->CSCLooseHaloId()) ), Prefix + "CSCLooseHaloId" + Suffix);
    iEvent.put(std::auto_ptr<bool>( new bool(haloSummary->CSCTightHaloId()) ), Prefix + "CSCTightHaloId" + Suffix);

    iEvent.put(std::auto_ptr<bool>( new bool(haloSummary->GlobalLooseHaloId()) ), Prefix + "GlobalLooseHaloId" + Suffix);
    iEvent.put(std::auto_ptr<bool>( new bool(haloSummary->GlobalTightHaloId()) ), Prefix + "GlobalTightHaloId" + Suffix);

    //iEvent.put(std::auto_ptr<bool>( new bool(haloSummary->LooseHaloId()) ), Prefix + "AnyLooseHaloId" + Suffix);
    //iEvent.put(std::auto_ptr<bool>( new bool(haloSummary->TightHaloId()) ), Prefix + "AnyTightHaloId" + Suffix);
  }
}
