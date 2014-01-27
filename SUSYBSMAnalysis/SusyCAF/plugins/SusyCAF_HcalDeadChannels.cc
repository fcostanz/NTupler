#include "SUSYBSMAnalysis/SusyCAF/interface/SusyCAF_HcalDeadChannels.h"
#include "FWCore/Framework/interface/ESHandle.h"


#include "Geometry/Records/interface/CaloGeometryRecord.h"
#include "Geometry/CaloGeometry/interface/CaloGeometry.h"

#include "DataFormats/HcalDetId/interface/HcalDetId.h"
#include "CondFormats/HcalObjects/interface/HcalChannelQuality.h"
#include "CondFormats/DataRecord/interface/HcalChannelQualityRcd.h"

SusyCAF_HcalDeadChannels::SusyCAF_HcalDeadChannels(const edm::ParameterSet& conf) :
  statusMask(conf.getParameter<uint32_t>("StatusMask")),
  channelQuality_cache_id(0),
  caloGeometry_cache_id(0)
{
  produces <std::vector<PolarLorentzV> > ("hcalDeadChannelP4");
  produces <std::vector<unsigned> >      ("hcalDeadChannelStatus");
}

void SusyCAF_HcalDeadChannels::produce(edm::Event& iEvent, const edm::EventSetup& iSetup) {
  updateIfNecessary(iSetup);
  iEvent.put( std::auto_ptr <std::vector<PolarLorentzV> >(new std::vector<PolarLorentzV>(p4)),     "hcalDeadChannelP4");
  iEvent.put( std::auto_ptr <std::vector<unsigned> >(new std::vector<unsigned>(status)),       "hcalDeadChannelStatus");
}

void SusyCAF_HcalDeadChannels::
updateIfNecessary(const edm::EventSetup& es) {
  
  const uint32_t quality_id = es.get<HcalChannelQualityRcd>().cacheIdentifier();
  const uint32_t geom_id = es.get<CaloGeometryRecord>().cacheIdentifier();

  if ( quality_id == channelQuality_cache_id &&
       geom_id == caloGeometry_cache_id ) return;

  channelQuality_cache_id = quality_id; edm::ESHandle<HcalChannelQuality> channelQuality; es.get<HcalChannelQualityRcd>().get(channelQuality);
  caloGeometry_cache_id = geom_id;      edm::ESHandle<CaloGeometry> caloGeometry;         es.get<CaloGeometryRecord>().get(caloGeometry);
  
  p4.clear();
  status.clear();
  for (uint32_t i = 0; i < HcalDetId::kSizeForDenseIndexing; ++i) {
    HcalDetId id = HcalDetId::detIdFromDenseIndex(i);
    
    //ignore HO etc.
    HcalSubdetector subdet = id.subdet();
    if (subdet!=HcalBarrel && subdet!=HcalEndcap && subdet!=HcalForward) continue;

    const uint32_t channelStatus = channelQuality->getValues(id.rawId())->getValue();
    
    if ( channelStatus & statusMask ) {
      const GlobalPoint& point = caloGeometry->getPosition(id);
      p4.push_back( PolarLorentzV(0.0, point.eta(), point.phi(), 0.0));
      status.push_back(channelStatus);
    }
  }

}
