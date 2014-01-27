#ifndef SUSY_CAF_PHOTON
#define SUSY_CAF_PHOTON

#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Utilities/interface/InputTag.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "DataFormats/EgammaCandidates/interface/Photon.h"
#include "DataFormats/EgammaCandidates/interface/Conversion.h"
#include "RecoEgamma/EgammaTools/interface/ConversionTools.h"

#include "DataFormats/TrackReco/interface/Track.h"
#include "DataFormats/PatCandidates/interface/Photon.h"

#include "DataFormats/HcalRecHit/interface/HcalRecHitCollections.h"
#include "DataFormats/EcalRecHit/interface/EcalRecHitCollections.h"
#include "RecoEcal/EgammaCoreTools/interface/EcalClusterTools.h"

#include "FWCore/Framework/interface/ESHandle.h"
#include "Geometry/Records/interface/CaloGeometryRecord.h"
#include "RecoCaloTools/MetaCollections/interface/CaloRecHitMetaCollections.h"
#include "RecoCaloTools/Selectors/interface/CaloConeSelector.h"
//#include "MyAnalysis/IsolationTools/interface/SuperClusterHitsEcalIsolation.h"

#include "Geometry/CaloTopology/interface/CaloTopology.h"
#include "Geometry/CaloEventSetup/interface/CaloTopologyRecord.h"
#include "DataFormats/EcalDetId/interface/EcalSubdetector.h"
#include "RecoLocalCalo/EcalRecAlgos/interface/EcalSeverityLevelAlgo.h"

#include "EGamma/EGammaAnalysisTools/interface/PFIsolationEstimator.h"


#include <Math/ProbFuncMathCore.h>
#include <Math/VectorUtil.h>

template< typename T >
class SusyCAF_Photon : public edm::EDProducer 
{
public: 
  explicit SusyCAF_Photon(const edm::ParameterSet&);

private: 
  void initTemplate();
  void initRECO();
  void initPAT();
  void initExtra();
  void produce(edm::Event &, const edm::EventSetup & );
  void produceTemplate(edm::Event &, const edm::EventSetup &, edm::Handle<std::vector<reco::Photon> > &);
  void produceTemplate(edm::Event &, const edm::EventSetup &, edm::Handle<std::vector<pat::Photon> > &);
  void produceRECO(edm::Event &, const edm::EventSetup &, edm::Handle<std::vector<T> > &);
  void producePAT(edm::Event &, const edm::EventSetup &, edm::Handle<std::vector<T> > &);
  void produceExtra(edm::Event &, const edm::EventSetup &, edm::Handle<std::vector<T> > &);
  void produceExtraSpikeVarsFunc(edm::Event &, const edm::EventSetup &,
				 edm::Handle<std::vector<T> > &,
				 edm::ESHandle<CaloTopology> &,
				 edm::Handle<EBRecHitCollection> &,
				 edm::Handle<EERecHitCollection> &
				 );

  double ebEeBoundary() const {return 1.479;}
  
  const edm::InputTag   inputTag;
  const std::string     prefix,suffix;

  const bool produceExtraSpikeVars;
  const std::string ebRecHitCollection;
  const std::string eeRecHitCollection;
  const edm::InputTag chIsoInputTag_, phIsoInputTag_, nhIsoInputTag_;
  const edm::InputTag PFIsoOfflineBeamSpot_, PFIsoConversions_, PFIsoElectrons_, PFIsoPhotons_;
  typedef reco::Candidate::LorentzVector  LorentzVector;
  typedef reco::Candidate::Vector         Vector;
  typedef math::XYZVectorF                VectorF;
  typedef reco::Candidate::Point          Point;
  typedef math::XYZPointF                 PointF;
};

template< typename T >
SusyCAF_Photon<T>::SusyCAF_Photon(const edm::ParameterSet& iConfig) 
  : inputTag(iConfig.getParameter<edm::InputTag>("InputTag"))
  , prefix  (iConfig.getParameter<std::string>  ("Prefix"  ))
  , suffix  (iConfig.getParameter<std::string>  ("Suffix"  ))

  , produceExtraSpikeVars(iConfig.getParameter<bool>("ProduceExtraSpikeVars"))
  , ebRecHitCollection(iConfig.getParameter<std::string>("EbRecHitCollection"))
  , eeRecHitCollection(iConfig.getParameter<std::string>("EeRecHitCollection"))
  , chIsoInputTag_(iConfig.getParameter<edm::InputTag>("chIsoInputTag")) 
  , phIsoInputTag_(iConfig.getParameter<edm::InputTag>("phIsoInputTag")) 
  , nhIsoInputTag_(iConfig.getParameter<edm::InputTag>("nhIsoInputTag")) 
  , PFIsoOfflineBeamSpot_(iConfig.getParameter<edm::InputTag>("PFIsoOfflineBeamSpot"))
  , PFIsoConversions_(iConfig.getParameter<edm::InputTag>("PFIsoConversions"))
  , PFIsoElectrons_(iConfig.getParameter<edm::InputTag>("PFIsoElectrons"))
  , PFIsoPhotons_(iConfig.getParameter<edm::InputTag>("PFIsoPhotons"))

                                 



{
  initTemplate();
}

template< typename T >
void SusyCAF_Photon<T>::initRECO()
{
  produces <bool                        >(prefix + "HandleValid"          + suffix);
  produces <std::vector<LorentzVector > >(prefix + "P4"                   + suffix);
  produces <std::vector<PointF        > >(prefix + "CaloPosition"         + suffix);
  produces <std::vector<float         > >(prefix + "HadronicDepth1OverEm" + suffix);
  produces <std::vector<float         > >(prefix + "HadronicDepth2OverEm" + suffix);
  produces <std::vector<float         > >(prefix + "HadronicOverEm"       + suffix);
  produces <std::vector<int           > >(prefix + "HasPixelSeed"         + suffix);
  produces <std::vector<int           > >(prefix + "IsEB"                 + suffix);
  produces <std::vector<int           > >(prefix + "IsEE"                 + suffix);
  produces <std::vector<int           > >(prefix + "IsEBGap"              + suffix);
  produces <std::vector<int           > >(prefix + "IsEEGap"              + suffix);
  produces <std::vector<int           > >(prefix + "IsEBEEGap"            + suffix);

  produces <std::vector<float> >  (prefix + "TrkSumPtHollowConeDR03"     + suffix);
  produces <std::vector<float> >  (prefix + "EcalRecHitEtConeDR03"       + suffix);
  produces <std::vector<float> >  (prefix + "HcalTowSumEtConeDR03"       + suffix);

  produces <std::vector<float> >  (prefix + "TrkSumPtHollowConeDR04"     + suffix);
  produces <std::vector<float> >  (prefix + "EcalRecHitEtConeDR04"       + suffix);
  produces <std::vector<float> >  (prefix + "HcalTowSumEtConeDR04"       + suffix);
  produces <std::vector<float> >  (prefix + "HcalDepth1TowSumEtConeDR04" + suffix);
  produces <std::vector<float> >  (prefix + "HcalDepth2TowSumEtConeDR04" + suffix);
  produces <std::vector<float> >  (prefix + "R9"   + suffix);

  produces <std::vector<float> >  (prefix + "e1x5" + suffix);
  produces <std::vector<float> >  (prefix + "e2x5" + suffix);
  produces <std::vector<float> >  (prefix + "e3x3" + suffix);
  produces <std::vector<float> >  (prefix + "e5x5" + suffix);
  produces <std::vector<double> > (prefix + "SuperClusterEnergy"         + suffix);
  produces <std::vector<Point> >  (prefix + "SuperClusterPos"            + suffix);
  produces <std::vector<double> > (prefix + "SuperClusterEtaWidth"       + suffix);
  produces <std::vector<double> > (prefix + "SuperClusterPhiWidth"       + suffix);

  produces <std::vector<int   > >(prefix + "NConversions"                + suffix);
  produces <std::vector<float > >(prefix + "AllConversionTracksSumPt"    + suffix);
  produces <std::vector<VectorF> >(prefix + "BestConversionTrack0P3"     + suffix);
  produces <std::vector<VectorF> >(prefix + "BestConversionTrack1P3"     + suffix);
  produces <std::vector<Point > >(prefix + "BestConversionVertex"        + suffix);
  produces <std::vector<float > >(prefix + "BestConversionEoverP"        + suffix);
  produces <std::vector<float > >(prefix + "BestConversionMass"          + suffix);
  
  produces <std::vector<double> >   (prefix + "ChPFIso"                  + suffix);
  produces <std::vector<double> >   (prefix + "PhPFIso"                  + suffix);
  produces <std::vector<double> >   (prefix + "NhPFIso"                  + suffix);

  produces <std::vector<float> >  (prefix + "HcalIsoConeDR03" + suffix);
  produces <std::vector<float> >  (prefix + "HcalIsoConeDR04" + suffix);
  produces <std::vector<bool> >   (prefix + "PassConvSafeElectronVeto"   + suffix); 
  
  produces <std::vector<float  > >(prefix + "HadronicSingleTowerOverEm"  + suffix);

}

template< typename T >
void SusyCAF_Photon<T>::initPAT()
{
  produces <std::vector<int> >  (prefix + "IDLoose" + suffix);
  produces <std::vector<int> >  (prefix + "IDTight" + suffix);
  produces <std::vector<float> >(prefix + "TrkIso"  + suffix);
  produces <std::vector<float> >(prefix + "CaloIso"       + suffix);
  produces <std::vector<float> >(prefix + "EcalIso"       + suffix);
  produces <std::vector<float> >(prefix + "HcalIso"       + suffix);
  produces <std::vector<float> >(prefix + "SigmaEtaEta"   + suffix);
  produces <std::vector<float> >(prefix + "SigmaIetaIeta" + suffix);
}

template< typename T >
void SusyCAF_Photon<T>::initExtra()
{
  if (produceExtraSpikeVars) {
    produces <std::vector<float> >  (prefix + "E2overE9"     + suffix);
    produces <std::vector<float> >  (prefix + "SeedTime"     + suffix);
    produces <std::vector<float> >  (prefix + "Time2"        + suffix);
    produces <std::vector<float> >  (prefix + "SeedEnergy"   + suffix);
    produces <std::vector<float> >  (prefix + "Energy2"      + suffix);
    produces <std::vector<float> >  (prefix + "e2x2"         + suffix);
    produces <std::vector<float> >  (prefix + "e4x4"         + suffix);
    produces <std::vector<double> > (prefix + "SuperClusterEtaPhiWidth" + suffix);
    produces <std::vector<int> >    (prefix + "SuperClusterNXtals"      + suffix);
  }

}

template< typename T >
void SusyCAF_Photon<T>::produce(edm::Event& iEvent, const edm::EventSetup& iSetup) 
{
  edm::Handle<std::vector<T> > collection;
  iEvent.getByLabel(inputTag, collection);
  produceTemplate(iEvent, iSetup, collection);
  }
// produce method in case of RECO data
template< typename T >
void SusyCAF_Photon<T>::produceTemplate(edm::Event& iEvent, const edm::EventSetup& iSetup, edm::Handle<std::vector<reco::Photon> >& collection) {
  produceRECO (iEvent, iSetup, collection);
  produceExtra(iEvent, iSetup, collection);
}
// produce method in case of PAT data
template< typename T >
void SusyCAF_Photon<T>::produceTemplate(edm::Event& iEvent, const edm::EventSetup& iSetup, edm::Handle<std::vector<pat::Photon> >& collection) {
  produceRECO (iEvent, iSetup, collection);
  produceExtra(iEvent, iSetup, collection);
  producePAT  (iEvent, iSetup, collection);
}

template< typename T >
void SusyCAF_Photon<T>::
produceRECO(edm::Event& iEvent, const edm::EventSetup& iSetup, edm::Handle<std::vector<T> >& collection) 
{
  std::auto_ptr<bool                        > isHandleValid       ( new bool(collection.isValid())    );
  std::auto_ptr<std::vector<LorentzVector > > p4                  ( new std::vector<LorentzVector >() );
  std::auto_ptr<std::vector<PointF> >caloPosition        ( new std::vector<PointF>() );
  std::auto_ptr<std::vector<float> > hadronicDepth1OverEm( new std::vector<float>() );
  std::auto_ptr<std::vector<float> > hadronicDepth2OverEm( new std::vector<float>() );
  std::auto_ptr<std::vector<float> > hadronicOverEm      ( new std::vector<float>() );
  std::auto_ptr<std::vector<int  > > hasPixelSeed        ( new std::vector<int  >() );
  std::auto_ptr<std::vector<int  > > isEB                ( new std::vector<int  >() );
  std::auto_ptr<std::vector<int  > > isEE                ( new std::vector<int  >() );
  std::auto_ptr<std::vector<int  > > isEBGap             ( new std::vector<int  >() );
  std::auto_ptr<std::vector<int  > > isEEGap             ( new std::vector<int  >() );
  std::auto_ptr<std::vector<int  > > isEBEEGap           ( new std::vector<int  >() );

  std::auto_ptr<std::vector<float> > trkSumPtHolConeDR03 (new std::vector<float>());
  std::auto_ptr<std::vector<float> > EcalrechitEtConeDR03 (new std::vector<float>());
  std::auto_ptr<std::vector<float> > HcalDR03   (new std::vector<float>());

  std::auto_ptr<std::vector<float> > trkSumPtHolConeDR04 (new std::vector<float>());
  std::auto_ptr<std::vector<float> > EcalrechitEtConeDR04 (new std::vector<float>());
  std::auto_ptr<std::vector<float> > HcalDR04   (new std::vector<float>());
  std::auto_ptr<std::vector<float> > HcalDepth1DR04 (new std::vector<float>());
  std::auto_ptr<std::vector<float> > HcalDepth2DR04 (new std::vector<float>());

  std::auto_ptr<std::vector<float> > R9   (new std::vector<float>());
  std::auto_ptr<std::vector<float> > e1x5 (new std::vector<float>());
  std::auto_ptr<std::vector<float> > e2x5 (new std::vector<float>());
  std::auto_ptr<std::vector<float> > e3x3 (new std::vector<float>());
  std::auto_ptr<std::vector<float> > e5x5 (new std::vector<float>());
  std::auto_ptr<std::vector<double> > SCenergy   (new std::vector<double>());
  std::auto_ptr<std::vector<Point> >  SCpoint    (new std::vector<Point>());
  std::auto_ptr<std::vector<double> > SCetaWidth (new std::vector<double>());
  std::auto_ptr<std::vector<double> > SCphiWidth (new std::vector<double>());

  std::auto_ptr<std::vector<int    > > nConversions       ( new std::vector<int  >() );
  std::auto_ptr<std::vector<float  > > allConversionTracksSumPt ( new std::vector<float  >() );
  std::auto_ptr<std::vector<VectorF> > bestConvTrack0P3   ( new std::vector<VectorF>() );
  std::auto_ptr<std::vector<VectorF> > bestConvTrack1P3   ( new std::vector<VectorF>() );
  std::auto_ptr<std::vector<Point  > > bestConvVertex     ( new std::vector<Point >() );
  std::auto_ptr<std::vector<float  > > bestConvEoverP     ( new std::vector<float >() ); 
  std::auto_ptr<std::vector<float  > > bestConvMass       ( new std::vector<float >() );

  std::auto_ptr<std::vector<double > > chPFIso            ( new std::vector<double >() );
  std::auto_ptr<std::vector<double > > phPFIso            ( new std::vector<double >() );
  std::auto_ptr<std::vector<double > > nhPFIso            ( new std::vector<double >() );

  std::auto_ptr<std::vector<float> > HcalIsoConeDR03( new std::vector<float>() );
  std::auto_ptr<std::vector<float> > HcalIsoConeDR04( new std::vector<float>() );
  std::auto_ptr<std::vector<bool> >  passConvSafeElectronVeto( new std::vector<bool>() );
  std::auto_ptr<std::vector<float> > hadronicSingleTowerOverEm      ( new std::vector<float>() );



  //from https://twiki.cern.ch/twiki/bin/view/CMS/ConversionTools
  Handle<reco::PhotonCollection> recoCollection;
  iEvent.getByLabel(PFIsoPhotons_, recoCollection);  

  edm::Handle<reco::BeamSpot> bsHandle;
  iEvent.getByLabel(PFIsoOfflineBeamSpot_, bsHandle);
  const reco::BeamSpot &beamspot = *bsHandle.product();

  edm::Handle<reco::ConversionCollection> hConversions;
  iEvent.getByLabel(PFIsoConversions_, hConversions);
  
  edm::Handle<reco::GsfElectronCollection> hElectrons;
  iEvent.getByLabel(PFIsoElectrons_, hElectrons);     

  
  if (collection.isValid()) {
    for (typename std::vector<T>::const_iterator it = collection->begin(); it != collection->end(); ++it) {
      const reco::Photon& photon = *it;
      p4                          ->push_back(photon.p4                   ());
      caloPosition                ->push_back(photon.caloPosition         ());
      hadronicDepth1OverEm        ->push_back(photon.hadronicDepth1OverEm ());
      hadronicDepth2OverEm        ->push_back(photon.hadronicDepth2OverEm ());
      hadronicOverEm              ->push_back(photon.hadronicOverEm       ());
      hadronicSingleTowerOverEm   ->push_back(photon.hadTowOverEm         ());
      hasPixelSeed                ->push_back(photon.hasPixelSeed         ());
      isEB                        ->push_back(photon.isEB                 ());
      isEE                        ->push_back(photon.isEE                 ());
      isEBGap                     ->push_back(photon.isEBGap              ());
      isEEGap                     ->push_back(photon.isEEGap              ());
      isEBEEGap                   ->push_back(photon.isEBEEGap            ());
      
      trkSumPtHolConeDR03  -> push_back(photon.trkSumPtHollowConeDR03());
      EcalrechitEtConeDR03 -> push_back(photon.ecalRecHitSumEtConeDR03());
      HcalDR03             -> push_back(photon.hcalTowerSumEtConeDR03());

      trkSumPtHolConeDR04  -> push_back(photon.trkSumPtHollowConeDR04());
      EcalrechitEtConeDR04 -> push_back(photon.ecalRecHitSumEtConeDR04());
      HcalDR04             -> push_back(photon.hcalTowerSumEtConeDR04());
      HcalDepth1DR04       -> push_back(photon.hcalDepth1TowerSumEtConeDR04());
      HcalDepth2DR04       -> push_back(photon.hcalDepth2TowerSumEtConeDR04());

      R9   -> push_back(photon.r9());
      e1x5 -> push_back(photon.e1x5());
      e2x5 -> push_back(photon.e2x5());
      e3x3 -> push_back(photon.e3x3());
      e5x5 -> push_back(photon.e5x5());
      

      reco::SuperClusterRef scluster = photon.superCluster();
      SCenergy   -> push_back(scluster->energy());
      SCpoint    -> push_back(scluster->position());
      SCetaWidth -> push_back(scluster->etaWidth());
      SCphiWidth -> push_back(scluster->phiWidth());

      float hcalIsoConeDR03_2012 = photon.hcalTowerSumEtConeDR03() 
	                           + (photon.hadronicOverEm() - photon.hadTowOverEm())
	                           * scluster->energy()/cosh(scluster->eta());

      float hcalIsoConeDR04_2012 = photon.hcalTowerSumEtConeDR04()  
				   + (photon.hadronicOverEm() - photon.hadTowOverEm())
	                           * scluster->energy()/cosh(scluster->eta());
																		  
      HcalIsoConeDR03 -> push_back(hcalIsoConeDR03_2012);
      HcalIsoConeDR04 -> push_back(hcalIsoConeDR04_2012);
    
      reco::ConversionRef bestConv;
      float tracksSumPt = 0;
      unsigned nConv = 0;
      
      reco::ConversionRefVector conversions = photon.conversions();
      for (unsigned int iConv = 0; iConv < conversions.size(); ++iConv) {
	const reco::ConversionRef& conversion = conversions[iConv];
	if  ( conversion->nTracks() != 2) continue;
	std::vector<edm::RefToBase<reco::Track> > tracks = conversion->tracks();
	const reco::Vertex&         vertex = conversion->conversionVertex();
	double chi2 = vertex.chi2();
	double ndof = vertex.ndof();
	double dPhi = conversion->dPhiTracksAtVtx();
	double dCotTheta = conversion->pairCotThetaSeparation();
	double chi2Prob = ROOT::Math::chisquared_cdf_c(chi2,ndof);
	// http://cdsweb.cern.ch/record/1279143
	if (! ( ndof &&
		chi2 > 0 &&
		fabs(dPhi) < 0.2 &&
		fabs(dCotTheta) < 0.3 &&
		chi2Prob > 5e-5) ) continue;

	if (bestConv.isNull() || bestConv->EoverP() > conversion->EoverP())
	  bestConv  = conversion;
	tracksSumPt += tracks[0]->pt() ;
	tracksSumPt += tracks[1]->pt() ;
	nConv++;
      } // end loop over conversions
      
      nConversions->push_back(nConv);
      allConversionTracksSumPt->push_back(tracksSumPt);
      bestConvTrack0P3->push_back(bestConv.isNull() ? VectorF() : bestConv->tracksPin()[0]);
      bestConvTrack1P3->push_back(bestConv.isNull() ? VectorF() : bestConv->tracksPin()[1]);
      bestConvVertex ->push_back(bestConv.isNull() ? Point() : bestConv->conversionVertex().position());
      bestConvEoverP ->push_back(bestConv.isNull() ? 0 : bestConv->EoverP());
      bestConvMass   ->push_back(bestConv.isNull() ? 0 : bestConv->pairInvariantMass());

      bool passelectronveto = !ConversionTools::hasMatchedPromptElectron(scluster, hElectrons, hConversions, beamspot.position());    
      passConvSafeElectronVeto->push_back(passelectronveto);
       
    }
  }
  if (recoCollection.isValid()) {  //loop over photons in reco::PhotonCollection to store ISO values from ValueMap. 
            
      unsigned nphot=recoCollection->size();
      for(unsigned iphot=0; iphot<nphot;++iphot) {

	edm::Handle<edm::ValueMap<double> > isoValch;
	iEvent.getByLabel(chIsoInputTag_, isoValch);
	edm::ValueMap<double> chIsoValues = *(isoValch.product());

	edm::Handle<edm::ValueMap<double> > isoValph;
	iEvent.getByLabel(phIsoInputTag_, isoValph);
	edm::ValueMap<double> phIsoValues = *(isoValph.product());

	edm::Handle<edm::ValueMap<double> > isoValnh;
	iEvent.getByLabel(nhIsoInputTag_, isoValnh);
	edm::ValueMap<double> nhIsoValues = *(isoValnh.product());

	reco::PhotonRef myPhotonRef(recoCollection, iphot); 
	
	chPFIso->push_back(chIsoValues[myPhotonRef]);
	phPFIso->push_back(phIsoValues[myPhotonRef]);
	nhPFIso->push_back(nhIsoValues[myPhotonRef]);

      }
  }
    
  iEvent.put(isHandleValid             , prefix + "HandleValid"                + suffix);
  iEvent.put(p4                        , prefix + "P4"                         + suffix);
  iEvent.put(caloPosition              , prefix + "CaloPosition"               + suffix);
  iEvent.put(hadronicDepth1OverEm      , prefix + "HadronicDepth1OverEm"       + suffix);
  iEvent.put(hadronicDepth2OverEm      , prefix + "HadronicDepth2OverEm"       + suffix);
  iEvent.put(hadronicOverEm            , prefix + "HadronicOverEm"             + suffix);
  iEvent.put(hasPixelSeed              , prefix + "HasPixelSeed"               + suffix);
  iEvent.put(isEB                      , prefix + "IsEB"                       + suffix);
  iEvent.put(isEE                      , prefix + "IsEE"                       + suffix);
  iEvent.put(isEBGap                   , prefix + "IsEBGap"                    + suffix);
  iEvent.put(isEEGap                   , prefix + "IsEEGap"                    + suffix);
  iEvent.put(isEBEEGap                 , prefix + "IsEBEEGap"                  + suffix);
  				     
  iEvent.put(trkSumPtHolConeDR03       , prefix + "TrkSumPtHollowConeDR03"     + suffix);
  iEvent.put(EcalrechitEtConeDR03      , prefix + "EcalRecHitEtConeDR03"       + suffix);
  iEvent.put(HcalDR03                  , prefix + "HcalTowSumEtConeDR03"       + suffix);

  iEvent.put(trkSumPtHolConeDR04       , prefix + "TrkSumPtHollowConeDR04"     + suffix);
  iEvent.put(EcalrechitEtConeDR04      , prefix + "EcalRecHitEtConeDR04"       + suffix);
  iEvent.put(HcalDR04                  , prefix + "HcalTowSumEtConeDR04"       + suffix);
  iEvent.put(HcalDepth1DR04            , prefix + "HcalDepth1TowSumEtConeDR04" + suffix);
  iEvent.put(HcalDepth2DR04            , prefix + "HcalDepth2TowSumEtConeDR04" + suffix);

  iEvent.put(R9                        , prefix + "R9"                         + suffix);
  iEvent.put(e1x5                      , prefix + "e1x5"                       + suffix);
  iEvent.put(e2x5                      , prefix + "e2x5"                       + suffix);
  iEvent.put(e3x3                      , prefix + "e3x3"                       + suffix);
  iEvent.put(e5x5                      , prefix + "e5x5"                       + suffix);
  iEvent.put(SCenergy                  , prefix + "SuperClusterEnergy"         + suffix);
  iEvent.put(SCpoint                   , prefix + "SuperClusterPos"            + suffix);
  iEvent.put(SCetaWidth                , prefix + "SuperClusterEtaWidth"       + suffix);
  iEvent.put(SCphiWidth                , prefix + "SuperClusterPhiWidth"       + suffix);

  iEvent.put(nConversions              , prefix + "NConversions"               + suffix);
  iEvent.put(allConversionTracksSumPt  , prefix + "AllConversionTracksSumPt"   + suffix);
  iEvent.put(bestConvTrack0P3          , prefix + "BestConversionTrack0P3"     + suffix);
  iEvent.put(bestConvTrack1P3          , prefix + "BestConversionTrack1P3"     + suffix);
  iEvent.put(bestConvVertex            , prefix + "BestConversionVertex"       + suffix);
  iEvent.put(bestConvEoverP            , prefix + "BestConversionEoverP"       + suffix);
  iEvent.put(bestConvMass              , prefix + "BestConversionMass"         + suffix);
  iEvent.put(chPFIso                   , prefix + "ChPFIso"                    + suffix);
  iEvent.put(phPFIso                   , prefix + "PhPFIso"                    + suffix);
  iEvent.put(nhPFIso                   , prefix + "NhPFIso"                    + suffix);
  iEvent.put(HcalIsoConeDR03           , prefix + "HcalIsoConeDR03"            + suffix);
  iEvent.put(HcalIsoConeDR04           , prefix + "HcalIsoConeDR04"            + suffix);
  iEvent.put(passConvSafeElectronVeto  , prefix + "PassConvSafeElectronVeto"   + suffix);
  iEvent.put(hadronicSingleTowerOverEm , prefix + "HadronicSingleTowerOverEm"  + suffix);
}  


// extra information stored for PAT data
template< typename T >
void SusyCAF_Photon<T>::
producePAT(edm::Event& iEvent, const edm::EventSetup& iSetup, edm::Handle<std::vector<T> >& collection) {
  std::auto_ptr<std::vector<int> >    idLoose       ( new std::vector<int>() );
  std::auto_ptr<std::vector<int> >    idTight       ( new std::vector<int>() );
  std::auto_ptr<std::vector<float> >  trkIso        ( new std::vector<float>() );
  std::auto_ptr<std::vector<float> >  caloIso       ( new std::vector<float>() );
  std::auto_ptr<std::vector<float> >  ecalIso       ( new std::vector<float>() );
  std::auto_ptr<std::vector<float> >  hcalIso       ( new std::vector<float>() );
  std::auto_ptr<std::vector<float> >  sigmaEtaEta   ( new std::vector<float>() );
  std::auto_ptr<std::vector<float> >  sigmaIetaIeta ( new std::vector<float>() );

  if (collection.isValid()){
    for (typename std::vector<T>::const_iterator it = collection->begin(); it != collection->end(); ++it) {
      const pat::Photon& photon = *it;
      idLoose      ->push_back(photon.photonID("PhotonCutBasedIDLoose"));
      idTight      ->push_back(photon.photonID("PhotonCutBasedIDTight"));
      trkIso       ->push_back(photon.trackIso     ());
      caloIso      ->push_back(photon.caloIso      ());
      ecalIso      ->push_back(photon.ecalIso      ());
      hcalIso      ->push_back(photon.hcalIso      ());
      sigmaEtaEta  ->push_back(photon.sigmaEtaEta  ());
      sigmaIetaIeta->push_back(photon.sigmaIetaIeta());
    }
  }
  
  iEvent.put(idLoose      , prefix + "IDLoose"        + suffix);
  iEvent.put(idTight      , prefix + "IDTight"        + suffix);
  iEvent.put(trkIso       , prefix + "TrkIso"         + suffix);
  iEvent.put(caloIso      , prefix + "CaloIso"        + suffix);
  iEvent.put(ecalIso      , prefix + "EcalIso"        + suffix);
  iEvent.put(hcalIso      , prefix + "HcalIso"        + suffix);
  iEvent.put(sigmaEtaEta  , prefix + "SigmaEtaEta"    + suffix);
  iEvent.put(sigmaIetaIeta, prefix + "SigmaIetaIeta"  + suffix);
}

template< typename T >
void SusyCAF_Photon<T>::
produceExtra(edm::Event& iEvent, const edm::EventSetup& iSetup, edm::Handle<std::vector<T> >& photons) 
{

  edm::Handle<EBRecHitCollection> ebRecHits;
  edm::Handle<EERecHitCollection> eeRecHits;

  if (produceExtraSpikeVars) {
    iEvent.getByLabel(ebRecHitCollection, ebRecHits);
    iEvent.getByLabel(eeRecHitCollection, eeRecHits);
  }

  if (produceExtraSpikeVars) {
      edm::ESHandle<CaloTopology> topology;
      iSetup.get<CaloTopologyRecord>().get(topology);

      produceExtraSpikeVarsFunc(iEvent, iSetup, photons, topology, ebRecHits, eeRecHits);
  }
}


template< typename T >
void SusyCAF_Photon<T>::
produceExtraSpikeVarsFunc(edm::Event& iEvent, const edm::EventSetup& iSetup,
			  edm::Handle<std::vector<T> >& photons,
			  edm::ESHandle<CaloTopology>& topology,
			  edm::Handle<EBRecHitCollection>& ebRecHits,
			  edm::Handle<EERecHitCollection>& eeRecHits
			  )

{
  std::auto_ptr<std::vector<float> > e2overE9   (new std::vector<float>());
  std::auto_ptr<std::vector<float> > SeedTime   (new std::vector<float>());
  std::auto_ptr<std::vector<float> > Time2   (new std::vector<float>());
  std::auto_ptr<std::vector<float> > SeedEnergy   (new std::vector<float>());
  std::auto_ptr<std::vector<float> > Energy2   (new std::vector<float>());

  std::auto_ptr<std::vector<float> > e2x2 (new std::vector<float>());
  std::auto_ptr<std::vector<float> > e4x4 (new std::vector<float>());
  std::auto_ptr<std::vector<double> > SCetaPhiWidth (new std::vector<double>());
  std::auto_ptr<std::vector<int> > SCnXtals (new std::vector<int>());

  const CaloTopology *topo = topology.product();

  if ( photons.isValid() && topology.isValid() && ebRecHits.isValid() && eeRecHits.isValid() ) {

    for (typename std::vector<T>::const_iterator it = photons->begin(); 
	 it != photons->end(); ++it) {
      const reco::Photon& photon = *it;
      const EcalRecHitCollection* ecalRecHits = 0;
      
      reco::SuperClusterRef scluster = photon.superCluster();
      int subdet = scluster->seed()->hitsAndFractions()[0].first.subdetId();
      if (subdet == EcalBarrel) ecalRecHits = ebRecHits.product();
      if (subdet == EcalEndcap) ecalRecHits = eeRecHits.product();
      DetId id = scluster->seed()->seed();
      SeedTime   -> push_back(ecalRecHits->find(id)->time());
      float e2 = -1;
      EBDetId id2 = 0;
      //int e2eta = 0; //unused
      //int e2phi = 0; //unused
      for ( int deta = -1; deta <= +1; ++deta ) {
        for ( int dphi = -1; dphi <= +1; ++dphi ) {
          EBDetId idtmp = EBDetId::offsetBy(id,deta,dphi);
	  if(ecalRecHits->find(idtmp) == ecalRecHits->end()) continue;
          float etmp = ecalRecHits->find(idtmp)->energy();
          float eapproxet = etmp / cosh( EBDetId::approxEta(idtmp) );
          if (etmp>e2 && eapproxet>1. && !(deta==0 && dphi==0)) {
            e2=etmp;
            id2=idtmp;
            //e2eta=deta;
            //e2phi=dphi;
          }
        }
      }
      if(ecalRecHits->find(id2) == ecalRecHits->end())
	Time2    -> push_back(-9999.);
      else
        Time2      -> push_back(ecalRecHits->find(id2)->time());
      SeedEnergy   -> push_back(ecalRecHits->find(id)->energy());
      Energy2      -> push_back(e2);
      e2overE9   -> push_back(e2/photon.e3x3());
      e2x2       -> push_back(EcalClusterTools::e2x2(*(scluster->seed()), ecalRecHits, topo)); 
      e4x4       -> push_back(EcalClusterTools::e4x4(*(scluster->seed()), ecalRecHits, topo)); 
      std::vector<float> cov = EcalClusterTools::localCovariances(*(scluster->seed()), &(*ecalRecHits), &(*topo));
      SCetaPhiWidth -> push_back(sqrt(cov[1]));
      int nXtals = 0;
      for (reco::CaloCluster_iterator cluster = scluster->clustersBegin(); cluster != scluster->clustersEnd(); ++cluster) {
	nXtals += (*cluster)->hitsAndFractions().size();
      }
      SCnXtals   -> push_back(nXtals);
    }//end loop over photons
  }//end if photons valid

  iEvent.put(e2overE9                , prefix + "E2overE9"                   + suffix);
  iEvent.put(SeedTime                , prefix + "SeedTime"                   + suffix);
  iEvent.put(Time2                   , prefix + "Time2"                      + suffix);
  iEvent.put(SeedEnergy              , prefix + "SeedEnergy"                 + suffix);
  iEvent.put(Energy2                 , prefix + "Energy2"                    + suffix);

  iEvent.put(e2x2                    , prefix + "e2x2"                       + suffix);
  iEvent.put(e4x4                    , prefix + "e4x4"                       + suffix);
  iEvent.put(SCetaPhiWidth           , prefix + "SuperClusterEtaPhiWidth"    + suffix);
  iEvent.put(SCnXtals                , prefix + "SuperClusterNXtals"         + suffix);
}


#endif
