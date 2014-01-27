#include "SUSYBSMAnalysis/SusyCAF/interface/SusyCAF_HcalNoiseSummary.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "DataFormats/METReco/interface/HcalNoiseSummary.h"

SusyCAF_HcalNoiseSummary::SusyCAF_HcalNoiseSummary(const edm::ParameterSet& iConfig) :
  inputTag(iConfig.getParameter<edm::InputTag>("InputTag")),
  Prefix(iConfig.getParameter<std::string>("Prefix")),
  Suffix(iConfig.getParameter<std::string>("Suffix"))
{
  produces <bool>  ( Prefix + "HandleValid" + Suffix);

  //descriptions of variables here: http://cmslxr.fnal.gov/lxr/source/DataFormats/METReco/interface/HcalNoiseSummary.h

  produces <bool>  ( Prefix + "PassLooseNoiseFilter" + Suffix);
  produces <bool>  ( Prefix + "PassTightNoiseFilter" + Suffix);
  produces <bool>  ( Prefix + "PassHighLevelNoiseFilter" + Suffix);

  produces <int>   ( Prefix + "NoiseFilterStatus" + Suffix);
  produces <int>   ( Prefix + "NoiseType" + Suffix);

  produces <float> ( Prefix + "EventEMEnergy" + Suffix);
  produces <float> ( Prefix + "EventHadEnergy" + Suffix);
  produces <float> ( Prefix + "EventTrackEnergy" + Suffix);
  produces <float> ( Prefix + "EventEMFraction" + Suffix);
  produces <float> ( Prefix + "EventChargeFraction" + Suffix);

  produces <float> ( Prefix + "Min10GeVHitTime" + Suffix);
  produces <float> ( Prefix + "Max10GeVHitTime" + Suffix);
  produces <float> ( Prefix + "Rms10GeVHitTime" + Suffix);
  produces <float> ( Prefix + "Min25GeVHitTime" + Suffix);
  produces <float> ( Prefix + "Max25GeVHitTime" + Suffix);
  produces <float> ( Prefix + "Rms25GeVHitTime" + Suffix);

  produces <int>   ( Prefix + "Num10GeVHits" + Suffix);
  produces <int>   ( Prefix + "Num25GeVHits" + Suffix);

  produces <float> ( Prefix + "MinE2TS" + Suffix);
  produces <float> ( Prefix + "MinE10TS" + Suffix);
  produces <float> ( Prefix + "MinE2Over10TS" + Suffix);

  produces <int>   ( Prefix + "MaxZeros" + Suffix);

  produces <int>   ( Prefix + "MaxHPDHits" + Suffix);
  produces <int>   ( Prefix + "MaxRBXHits" + Suffix);

  produces <float> ( Prefix + "MinHPDEMF" + Suffix);
  produces <float> ( Prefix + "MinRBXEMF" + Suffix);

  produces <int>   ( Prefix + "NumProblematicRBXs" + Suffix);
}

void SusyCAF_HcalNoiseSummary::
produce(edm::Event& iEvent, const edm::EventSetup& iSetup) {

  //declare
  std::auto_ptr<bool> handleValid             ( new bool() );

  std::auto_ptr<bool>  passLooseNoiseFilter ( new bool() );
  std::auto_ptr<bool>  passTightNoiseFilter ( new bool() );
  std::auto_ptr<bool>  passHighLevelNoiseFilter ( new bool() ); 

  std::auto_ptr<int>   noiseFilterStatus ( new int() );
  std::auto_ptr<int>   noiseType ( new int() );

  std::auto_ptr<float> eventEMEnergy ( new float() );
  std::auto_ptr<float> eventHadEnergy ( new float() );
  std::auto_ptr<float> eventTrackEnergy ( new float() );
  std::auto_ptr<float> eventEMFraction ( new float() );
  std::auto_ptr<float> eventChargeFraction ( new float() );

  std::auto_ptr<float> min10GeVHitTime ( new float() );
  std::auto_ptr<float> max10GeVHitTime ( new float() );
  std::auto_ptr<float> rms10GeVHitTime ( new float() );
  std::auto_ptr<float> min25GeVHitTime ( new float() );
  std::auto_ptr<float> max25GeVHitTime ( new float() );
  std::auto_ptr<float> rms25GeVHitTime ( new float() );

  std::auto_ptr<int>   num10GeVHits ( new int() );
  std::auto_ptr<int>   num25GeVHits ( new int() );

  std::auto_ptr<float> minE2TS ( new float() );
  std::auto_ptr<float> minE10TS ( new float() );
  std::auto_ptr<float> minE2Over10TS ( new float() );

  std::auto_ptr<int>   maxZeros ( new int() );

  std::auto_ptr<int>   maxHPDHits ( new int() );
  std::auto_ptr<int>   maxRBXHits ( new int() );

  std::auto_ptr<float> minHPDEMF ( new float() );
  std::auto_ptr<float> minRBXEMF ( new float() );

  std::auto_ptr<int>   numProblematicRBXs ( new int() );

  //assign
  edm::Handle<HcalNoiseSummary> hcalNoiseSummary;
  iEvent.getByLabel(inputTag,hcalNoiseSummary);

  *handleValid.get()=hcalNoiseSummary.isValid();
  if (*handleValid) {
    *passLooseNoiseFilter.get()=hcalNoiseSummary->passLooseNoiseFilter();
    *passTightNoiseFilter.get()=hcalNoiseSummary->passTightNoiseFilter();
    *passHighLevelNoiseFilter.get()=hcalNoiseSummary->passHighLevelNoiseFilter();

    *noiseFilterStatus.get()=hcalNoiseSummary->noiseFilterStatus();
    *noiseType.get()=hcalNoiseSummary->noiseType();

    *eventEMEnergy.get()=hcalNoiseSummary->eventEMEnergy();
    *eventHadEnergy.get()=hcalNoiseSummary->eventHadEnergy();
    *eventTrackEnergy.get()=hcalNoiseSummary->eventTrackEnergy();
    *eventEMFraction.get()=hcalNoiseSummary->eventEMFraction();
    *eventChargeFraction.get()=hcalNoiseSummary->eventChargeFraction();

    *min10GeVHitTime.get()=hcalNoiseSummary->min10GeVHitTime();
    *max10GeVHitTime.get()=hcalNoiseSummary->max10GeVHitTime();
    *rms10GeVHitTime.get()=hcalNoiseSummary->rms10GeVHitTime();
    *min25GeVHitTime.get()=hcalNoiseSummary->min25GeVHitTime();
    *max25GeVHitTime.get()=hcalNoiseSummary->max25GeVHitTime();
    *rms25GeVHitTime.get()=hcalNoiseSummary->rms25GeVHitTime();

    *num10GeVHits.get()=hcalNoiseSummary->num10GeVHits();
    *num25GeVHits.get()=hcalNoiseSummary->num25GeVHits();

    *minE2TS.get()=hcalNoiseSummary->minE2TS();
    *minE10TS.get()=hcalNoiseSummary->minE10TS();
    *minE2Over10TS.get()=hcalNoiseSummary->minE2Over10TS();

    *maxZeros.get()=hcalNoiseSummary->maxZeros();

    *maxHPDHits.get()=hcalNoiseSummary->maxHPDHits();
    *maxRBXHits.get()=hcalNoiseSummary->maxRBXHits();

    *minHPDEMF.get()=hcalNoiseSummary->minHPDEMF();
    *minRBXEMF.get()=hcalNoiseSummary->minRBXEMF();

    *numProblematicRBXs.get()=hcalNoiseSummary->numProblematicRBXs();
  }

  //put
  iEvent.put( handleValid, Prefix + "HandleValid" + Suffix );

  iEvent.put( passLooseNoiseFilter, Prefix + "PassLooseNoiseFilter" + Suffix );
  iEvent.put( passTightNoiseFilter, Prefix + "PassTightNoiseFilter" + Suffix );
  iEvent.put( passHighLevelNoiseFilter, Prefix + "PassHighLevelNoiseFilter" + Suffix );
  
  iEvent.put( noiseFilterStatus, Prefix + "NoiseFilterStatus" + Suffix );
  iEvent.put( noiseType, Prefix + "NoiseType" + Suffix );

  iEvent.put( eventEMEnergy, Prefix + "EventEMEnergy" + Suffix );
  iEvent.put( eventHadEnergy, Prefix + "EventHadEnergy" + Suffix );
  iEvent.put( eventTrackEnergy, Prefix + "EventTrackEnergy" + Suffix );
  iEvent.put( eventEMFraction, Prefix + "EventEMFraction" + Suffix );
  iEvent.put( eventChargeFraction, Prefix + "EventChargeFraction" + Suffix );

  iEvent.put( min10GeVHitTime, Prefix + "Min10GeVHitTime" + Suffix );
  iEvent.put( max10GeVHitTime, Prefix + "Max10GeVHitTime" + Suffix );
  iEvent.put( rms10GeVHitTime, Prefix + "Rms10GeVHitTime" + Suffix );
  iEvent.put( min25GeVHitTime, Prefix + "Min25GeVHitTime" + Suffix );
  iEvent.put( max25GeVHitTime, Prefix + "Max25GeVHitTime" + Suffix );
  iEvent.put( rms25GeVHitTime, Prefix + "Rms25GeVHitTime" + Suffix );

  iEvent.put( num10GeVHits, Prefix + "Num10GeVHits" + Suffix );
  iEvent.put( num25GeVHits, Prefix + "Num25GeVHits" + Suffix );

  iEvent.put( minE2TS, Prefix + "MinE2TS" + Suffix );
  iEvent.put( minE10TS, Prefix + "MinE10TS" + Suffix );
  iEvent.put( minE2Over10TS, Prefix + "MinE2Over10TS" + Suffix );

  iEvent.put( maxZeros, Prefix + "MaxZeros" + Suffix );

  iEvent.put( maxHPDHits, Prefix + "MaxHPDHits" + Suffix );
  iEvent.put( maxRBXHits, Prefix + "MaxRBXHits" + Suffix );

  iEvent.put( minHPDEMF, Prefix + "MinHPDEMF" + Suffix );
  iEvent.put( minRBXEMF, Prefix + "MinRBXEMF" + Suffix );

  iEvent.put( numProblematicRBXs, Prefix + "NumProblematicRBXs" + Suffix );
}
