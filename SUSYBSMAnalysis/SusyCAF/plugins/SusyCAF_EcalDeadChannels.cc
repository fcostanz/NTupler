#include <memory>
#include "SUSYBSMAnalysis/SusyCAF/interface/SusyCAF_EcalDeadChannels.h"

SusyCAF_EcalDeadChannels::SusyCAF_EcalDeadChannels(const edm::ParameterSet& conf) 
  : inputTag(conf.getParameter<edm::InputTag>("InputTag")),
    minStatus(conf.getParameter<uint32_t>("MinStatus")),
    channelStatus_cache_id(0),
    caloGeometry_cache_id(0),
    caloConstituents_cache_id(0)
{
  produces <std::vector<PolarLorentzV> > ( "ecalDeadTowerTrigPrimP4" );
  produces <std::vector<unsigned> > ( "ecalDeadTowerNBadXtals" );
  produces <std::vector<unsigned> > ( "ecalDeadTowerMaxStatus" );
}


void SusyCAF_EcalDeadChannels::
produce(edm::Event& e, const edm::EventSetup& es) {

  std::auto_ptr<std::vector<PolarLorentzV> > p4(new std::vector<PolarLorentzV>());
  std::auto_ptr<std::vector<unsigned> > nbadxtal(new std::vector<unsigned>());
  std::auto_ptr<std::vector<unsigned> > maxstatus(new std::vector<unsigned>());

  edm::Handle<EcalTrigPrimDigiCollection> tpDigis;  e.getByLabel(inputTag, tpDigis);
  updateBadTowers(es);
  ecalScale_.setEventSetup(es);
  
  for (std::vector<towerInfo>::const_iterator it = badTowers.begin(); it!=badTowers.end(); ++it) {
    float tpEt=-1;
    if(tpDigis.isValid()) {
      EcalTrigPrimDigiCollection::const_iterator tp = tpDigis->find(it->id) ;
      if(tp != tpDigis->end()) tpEt = std::max(0.,ecalScale_.getTPGInGeV(*tp));
    }
    p4->push_back( PolarLorentzV( tpEt, it->eta, it->phi, tpEt*cosh(it->eta) ));
    nbadxtal->push_back(it->nbad);
    maxstatus->push_back(it->maxstatus);
  }

  e.put(p4,       "ecalDeadTowerTrigPrimP4");
  e.put(nbadxtal, "ecalDeadTowerNBadXtals");
  e.put(maxstatus,"ecalDeadTowerMaxStatus");
}


void SusyCAF_EcalDeadChannels::
updateBadTowers(const edm::EventSetup& es) {

  const uint32_t status_id = es.get<EcalChannelStatusRcd>().cacheIdentifier();
  const uint32_t geom_id = es.get<CaloGeometryRecord>().cacheIdentifier();
  const uint32_t const_id = es.get<IdealGeometryRecord>().cacheIdentifier();

  if ( status_id == channelStatus_cache_id &&
       geom_id == caloGeometry_cache_id &&
       const_id == caloConstituents_cache_id ) return;

  channelStatus_cache_id = status_id;   edm::ESHandle<EcalChannelStatus> channelStatus;    es.get<EcalChannelStatusRcd>().get(channelStatus);
  caloGeometry_cache_id = geom_id;      edm::ESHandle<CaloGeometry> caloGeometry;          es.get<CaloGeometryRecord>().get(caloGeometry);
  caloConstituents_cache_id = const_id; edm::ESHandle<EcalTrigTowerConstituentsMap> ttMap; es.get<IdealGeometryRecord>().get(ttMap);

  std::map<uint32_t,unsigned> nBadXtal, maxStatus;
  std::map<uint32_t,double> sumEta,sumPhi;

  loopXtals<EBDetId>(nBadXtal, maxStatus, sumEta, sumPhi, channelStatus.product(), caloGeometry.product(), ttMap.product());
  loopXtals<EEDetId>(nBadXtal, maxStatus, sumEta, sumPhi, channelStatus.product(), caloGeometry.product(), ttMap.product());
  
  badTowers.clear();
  for(std::map<uint32_t,unsigned>::const_iterator it = nBadXtal.begin(); it!=nBadXtal.end(); it++) {
    uint32_t key = it->first;
    badTowers.push_back( towerInfo( key, nBadXtal[key], maxStatus[key], sumEta[key]/nBadXtal[key], sumPhi[key]/nBadXtal[key] )  );
  }
}

template <class Id>
void SusyCAF_EcalDeadChannels::
loopXtals(std::map<uint32_t,unsigned>& nBadXtal,
	  std::map<uint32_t,unsigned>& maxStatus,
	  std::map<uint32_t,double>& sumEta,
	  std::map<uint32_t,double>& sumPhi ,
	  const EcalChannelStatus* channelStatus,
	  const CaloGeometry* caloGeometry,
	  const EcalTrigTowerConstituentsMap* ttMap ) const {
  for (int i = 0;i<Id::kSizeForDenseIndexing;++i) {
    Id id = Id::unhashIndex( i );  if (id==Id(0)) continue;
    EcalChannelStatusMap::const_iterator it = channelStatus->getMap().find(id.rawId());
    unsigned status = it == channelStatus->end() ? 0 : it->getStatusCode()&statusMask;
    if (status >= minStatus) {
      const GlobalPoint& point = caloGeometry->getPosition(id);
      uint32_t key = ttMap->towerOf(id); //id.tower().rawId();
      maxStatus[key] = std::max(status,maxStatus[key]);
      nBadXtal[key]++;
      sumEta[key]+=point.eta();
      sumPhi[key]+=point.phi();
    }
  }
}
