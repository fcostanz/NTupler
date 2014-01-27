#ifndef SUSY_CAF_HCALRECHIT
#define SUSY_CAF_HCALRECHIT

#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "CalibCalorimetry/HcalAlgos/interface/HcalLogicalMapGenerator.h"
#include "DataFormats/HcalRecHit/interface/HcalRecHitCollections.h"
#include "RecoLocalCalo/HcalRecAlgos/interface/HcalCaloFlagLabels.h"
#include "TMath.h"

#include "CondFormats/HcalObjects/interface/HcalChannelQuality.h"
#include "CondFormats/DataRecord/interface/HcalChannelQualityRcd.h"
#include "RecoLocalCalo/HcalRecAlgos/interface/HcalSeverityLevelComputer.h"
#include "RecoLocalCalo/HcalRecAlgos/interface/HcalSeverityLevelComputerRcd.h"

#include "Geometry/Records/interface/CaloGeometryRecord.h"
#include "Geometry/CaloGeometry/interface/CaloGeometry.h"

template< typename T >
class SusyCAF_HcalRecHit : public edm::EDProducer {
 public: 
  explicit SusyCAF_HcalRecHit(const edm::ParameterSet&);
  ~SusyCAF_HcalRecHit();
 private: 
  void init        ();
  void produce     (edm::Event&,const edm::EventSetup&);
  void setupCrudeGeometry();
  double getEta(int iEta,int zSide);
  double getPhi(int iPhi);

  const edm::InputTag inputTag;
  const std::string Prefix,Suffix;
  const int severityLevelCut;
  const bool produceExtraVariables;

  //for extra variables
  void initExtra   ();
  void produceExtra(edm::Event&,const edm::EventSetup&);

  int getRbxSector(const int iRbx) const {return (iRbx%18)+1;}
  int getRbxZ(const int iRbx) const;

  std::vector<double> singleRbxThresholds;
  std::vector<double> singleRmThresholds;
  double singleChannelThreshold;

  HcalLogicalMap *logicalMap;
  double rbxEnergies      [HcalFrontEndId::maxRbxIndex+1]; //energy sum for each readout box
  int    rbxMultiplicities[HcalFrontEndId::maxRbxIndex+1]; //# of hits in each RBX

  double rmEnergies       [HcalFrontEndId::maxRmIndex+1]; //energy sum for each readout module
  int    rmMultiplicities [HcalFrontEndId::maxRmIndex+1]; //# of hits in each RM

  //for crude geometry
  static const int nEtaAbs=42; //41+1
  static const int nPhi=73; //72+1
  double etaabsLookup[nEtaAbs];
  double phiLookup[nPhi];

};

template< typename T >
SusyCAF_HcalRecHit<T>::SusyCAF_HcalRecHit(const edm::ParameterSet& iConfig) :
  inputTag(iConfig.getParameter<edm::InputTag>("InputTag")),
  Prefix(iConfig.getParameter<std::string>("Prefix")),
  Suffix(iConfig.getParameter<std::string>("Suffix")),
  severityLevelCut(iConfig.getParameter<int>("SeverityLevelCut")),
  produceExtraVariables(iConfig.getParameter<bool>("ProduceExtraVariables")),
  singleRbxThresholds(iConfig.getParameter<std::vector<double> >("SingleRbxThresholds")),
  singleRmThresholds(iConfig.getParameter<std::vector<double> >("SingleRmThresholds")),
  singleChannelThreshold(iConfig.getParameter<double>("SingleChannelThreshold"))
{
  init();
  if (produceExtraVariables) initExtra();
}

template< typename T >
SusyCAF_HcalRecHit<T>::~SusyCAF_HcalRecHit()
{
  if (produceExtraVariables) delete logicalMap;
}

template< typename T >
void SusyCAF_HcalRecHit<T>::init() {
  produces <bool>                                         ( Prefix + "HandleValid"   + Suffix );
  produces <std::vector<float> >                          ( Prefix + "Time"          + Suffix );
  produces <std::vector<unsigned> >                       ( Prefix + "FlagWord"      + Suffix );
  produces <std::vector<int> >                            ( Prefix + "SeverityLevel" + Suffix );
  produces <std::vector<ROOT::Math::LorentzVector<ROOT::Math::PtEtaPhiE4D<double> > > > ( Prefix + "P4" + Suffix );
}

template< typename T >
void SusyCAF_HcalRecHit<T>::produce(edm::Event& iEvent, const edm::EventSetup& iSetup) {
  std::auto_ptr<std::vector<ROOT::Math::LorentzVector<ROOT::Math::PtEtaPhiE4D<double> > > > p4(new std::vector<ROOT::Math::LorentzVector<ROOT::Math::PtEtaPhiE4D<double> > >() );
  std::auto_ptr<std::vector<float> >                          time          (new std::vector<float>    () );
  std::auto_ptr<std::vector<unsigned> >                       flagWord      (new std::vector<unsigned> () );
  std::auto_ptr<std::vector<int> >                            severityLevel (new std::vector<int>      () );
  //get geometry
  edm::ESHandle<CaloGeometry> caloGeometryHandle;
  iSetup.get<CaloGeometryRecord>().get(caloGeometryHandle);
  const CaloGeometry* caloGeometry = caloGeometryHandle.product();

  //get severity level computer
  edm::ESHandle<HcalSeverityLevelComputer> computerHandle;
  iSetup.get<HcalSeverityLevelComputerRcd>().get(computerHandle);
  const HcalSeverityLevelComputer* computer = computerHandle.product();

  //get channel status handle
  edm::ESHandle<HcalChannelQuality> channelQualityHandle;
  iSetup.get<HcalChannelQualityRcd>().get(channelQualityHandle);
  const HcalChannelQuality* channelQuality = channelQualityHandle.product();

  //get rechits
  edm::Handle<T> collection;
  iEvent.getByLabel(inputTag, collection);
  std::auto_ptr<bool> isHandleValid ( new bool(collection.isValid()) );

  ROOT::Math::LorentzVector<ROOT::Math::PtEtaPhiE4D<double> > thisP4(0.0,0.0,0.0,0.0);

  if (collection.isValid()) {
    //loop over rechits
    for(typename T::const_iterator it = collection->begin(); it != collection->end(); ++it) {

      uint32_t channelStatus=channelQuality->getValues(it->id().rawId())->getValue();
      int theSeverityLevel = computer->getSeverityLevel(it->id(),it->flags(),channelStatus);
      if (theSeverityLevel>=severityLevelCut) {

	const GlobalPoint& point=caloGeometry->getPosition(it->detid());
	//std::cout << point << std::endl;
	double eta=point.eta();
	double phi=point.phi();
	double energy=fabs(it->energy());//absolute value
	
  	thisP4.SetCoordinates(energy/cosh(eta),eta,phi,energy);
  	//std::cout 
	//  << "pT:  " << energy/cosh(eta) << " " << thisP4.pt()  << std::endl
	//  << "eta: " << eta     << " " << thisP4.eta() << std::endl
	//  << "phi: " << phi     << " " << thisP4.phi() << std::endl
	//  << "e:   " << energy  << " " << thisP4.e()   << std::endl;
	p4->push_back(thisP4);
	time->push_back(it->time());
	flagWord->push_back(it->flags());
  	severityLevel->push_back(theSeverityLevel);
      }
    } //end loop over rechits

  }//end if handle valid

  iEvent.put( isHandleValid,  Prefix + "HandleValid"   + Suffix );
  iEvent.put( p4,             Prefix + "P4"            + Suffix ); 
  iEvent.put( time,           Prefix + "Time"          + Suffix ); 
  iEvent.put( flagWord,       Prefix + "FlagWord"      + Suffix ); 
  iEvent.put( severityLevel,  Prefix + "SeverityLevel" + Suffix ); 
  
  if (produceExtraVariables) produceExtra(iEvent,iSetup);
}

template< typename T >
double SusyCAF_HcalRecHit<T>::getEta(int iEta,int zSide) {
  double badCode=-50.0;
  int iEtaAbs=abs(iEta);
  if (iEtaAbs<1 || iEtaAbs>=nEtaAbs) return badCode;
  if (abs(zSide)!=1) return badCode;

  return etaabsLookup[iEtaAbs]*zSide;
}

template< typename T >
double SusyCAF_HcalRecHit<T>::getPhi(int iPhi) {
  double badCode=-5.0;
  if (iPhi<1 || iPhi>=nPhi) return badCode;
  return phiLookup[iPhi];
}

template< typename T >
void SusyCAF_HcalRecHit<T>::setupCrudeGeometry() {
  //from http://www.hep.wisc.edu/cms/trig/RCTDocu/towers_ieta_iphi.pdf

  phiLookup[0]=0.0; //iPhi=0 not defined
  for (int iPhi=1;iPhi<nPhi;++iPhi) {
    phiLookup[iPhi]=2.0*TMath::Pi()*(iPhi-0.5)/nPhi;
    if (phiLookup[iPhi]>TMath::Pi()) phiLookup[iPhi]-=2.0*TMath::Pi();
  }

  etaabsLookup[ 0]=0.0; //iEta=0 not defined

  //numbers from CMS NOTE 2005/016 (Sunanda Banerjee)
  etaabsLookup[ 1]=(0.000 + 0.087)/2.0; //HB
  etaabsLookup[ 2]=(0.087 + 0.174)/2.0; //HB
  etaabsLookup[ 3]=(0.174 + 0.261)/2.0; //HB
  etaabsLookup[ 4]=(0.261 + 0.348)/2.0; //HB
  etaabsLookup[ 5]=(0.348 + 0.435)/2.0; //HB
  etaabsLookup[ 6]=(0.435 + 0.522)/2.0; //HB
  etaabsLookup[ 7]=(0.522 + 0.609)/2.0; //HB
  etaabsLookup[ 8]=(0.609 + 0.696)/2.0; //HB
  etaabsLookup[ 9]=(0.696 + 0.783)/2.0; //HB
  etaabsLookup[10]=(0.783 + 0.870)/2.0; //HB
  etaabsLookup[11]=(0.879 + 0.957)/2.0; //HB
  etaabsLookup[12]=(0.957 + 1.044)/2.0; //HB
  etaabsLookup[13]=(1.044 + 1.131)/2.0; //HB
  etaabsLookup[14]=(1.131 + 1.218)/2.0; //HB
  etaabsLookup[15]=(1.218 + 1.305)/2.0; //HB
  etaabsLookup[16]=(1.305 + 1.392)/2.0; //HB
  etaabsLookup[17]=(1.392 + 1.479)/2.0; //HE
  etaabsLookup[18]=(1.479 + 1.566)/2.0; //HE
  etaabsLookup[19]=(1.566 + 1.653)/2.0; //HE
  etaabsLookup[20]=(1.653 + 1.740)/2.0; //HE
  etaabsLookup[21]=(1.740 + 1.830)/2.0; //HE
  etaabsLookup[22]=(1.830 + 1.930)/2.0; //HE
  etaabsLookup[23]=(1.930 + 2.043)/2.0; //HE
  etaabsLookup[24]=(2.043 + 2.172)/2.0; //HE
  etaabsLookup[25]=(2.172 + 2.322)/2.0; //HE
  etaabsLookup[26]=(2.322 + 2.500)/2.0; //HE
  etaabsLookup[27]=(2.500 + 2.650)/2.0; //HE
  etaabsLookup[28]=(2.650 + 3.000)/2.0; //HE
  etaabsLookup[29]=(2.853 + 2.964)/2.0; //HF
  etaabsLookup[30]=(2.964 + 3.139)/2.0; //HF
  etaabsLookup[31]=(3.139 + 3.314)/2.0; //HF
  etaabsLookup[32]=(3.314 + 3.489)/2.0; //HF
  etaabsLookup[33]=(3.489 + 3.664)/2.0; //HF
  etaabsLookup[34]=(3.664 + 3.839)/2.0; //HF
  etaabsLookup[35]=(3.839 + 4.013)/2.0; //HF
  etaabsLookup[36]=(4.013 + 4.191)/2.0; //HF
  etaabsLookup[37]=(4.191 + 4.363)/2.0; //HF
  etaabsLookup[38]=(4.363 + 4.538)/2.0; //HF
  etaabsLookup[39]=(4.538 + 4.716)/2.0; //HF
  etaabsLookup[40]=(4.716 + 4.889)/2.0; //HF
  etaabsLookup[41]=(4.889 + 5.191)/2.0; //HF
}

//only extra stuff below here
template< typename T >
int SusyCAF_HcalRecHit<T>::getRbxZ(const int iRbx) const {
  int base=iRbx/18;//integer division
  int theMap[4]={-1,1,-2,2}; //{HBM,HBP,HEM,HEP}
  if (base>=0 && base<=3) return theMap[base];
  else                    return 0;
}

template< typename T >
void SusyCAF_HcalRecHit<T>::initExtra() {
  setupCrudeGeometry();

  HcalLogicalMapGenerator gen;
  logicalMap=new HcalLogicalMap(gen.createMap());

  produces <float>                ( Prefix + "totalEPlus"      + Suffix );
  produces <float>                ( Prefix + "totalEMinus"     + Suffix );
  produces <float>                ( Prefix + "etMax"           + Suffix );
  produces <float>                ( Prefix + "tMax"            + Suffix );
  produces <int>                  ( Prefix + "iEtaMax"         + Suffix );
  produces <int>                  ( Prefix + "iPhiMax"         + Suffix );
  produces <int>                  ( Prefix + "depthMax"        + Suffix );
  
  produces <int>                  ( Prefix + "multFlagCount"   + Suffix );
  produces <int>                  ( Prefix + "psFlagCount"     + Suffix );

  for (unsigned int iRbxThreshold=0;iRbxThreshold<singleRbxThresholds.size();++iRbxThreshold) {
    std::stringstream tempss;
    tempss << iRbxThreshold;

    produces <int>                  ( Prefix + "rbxCount" + tempss.str() + Suffix );
    produces <std::vector<double> > ( Prefix + "rbxE"     + tempss.str() + Suffix );
    produces <std::vector<int> >    ( Prefix + "rbxMult"  + tempss.str() + Suffix );
    produces <std::vector<int> >    ( Prefix + "rbxSector"+ tempss.str() + Suffix );
    produces <std::vector<int> >    ( Prefix + "rbxZ"     + tempss.str() + Suffix );
  }

  for (unsigned int iRmThreshold=0;iRmThreshold<singleRmThresholds.size();++iRmThreshold) {
    std::stringstream tempss;
    tempss << iRmThreshold;

    produces <int>                  ( Prefix + "rmCount" + tempss.str() + Suffix );
    produces <std::vector<double> > ( Prefix + "rmE"     + tempss.str() + Suffix );
    produces <std::vector<int> >    ( Prefix + "rmMult"  + tempss.str() + Suffix );
  }

}

template< typename T >
void SusyCAF_HcalRecHit<T>::
produceExtra(edm::Event& iEvent, const edm::EventSetup& iSetup) {
  std::auto_ptr<float>                totalEPlus     ( new float()               ) ;
  std::auto_ptr<float>                totalEMinus    ( new float()               ) ;
  std::auto_ptr<float>                etMax          ( new float()               ) ;
  std::auto_ptr<float>                tMax           ( new float()               ) ;
  std::auto_ptr<int>                  iEtaMax        ( new int()                 ) ;
  std::auto_ptr<int>                  iPhiMax        ( new int()                 ) ;
  std::auto_ptr<int>                  depthMax       ( new int()                 ) ;

  std::auto_ptr<int>                  multFlagCount  ( new int()                 ) ;
  std::auto_ptr<int>                  psFlagCount    ( new int()                 ) ;

  //initialize variables
  for (int iRbx=0;iRbx<=HcalFrontEndId::maxRbxIndex;++iRbx) {
    rbxEnergies[iRbx]=0.0;
    rbxMultiplicities[iRbx]=0;
  }
  for (int iRm=0;iRm<=HcalFrontEndId::maxRmIndex;++iRm) {
    rmEnergies[iRm]=0.0;
    rmMultiplicities[iRm]=0;
  }
  
  float totalEPlusLocal=0.0;
  float totalEMinusLocal=0.0;
  
  float etMaxLocal=-1000.0;
  typename T::const_iterator itMax;

  int multFlagCountLocal=0;
  int psFlagCountLocal=0;

  //loop over rechits
  edm::Handle<T> hcalrechitcollection;
  iEvent.getByLabel(inputTag, hcalrechitcollection);
  for(typename T::const_iterator it = hcalrechitcollection->begin(); it != hcalrechitcollection->end(); ++it) {
    const float energy=it->energy();
    const int iEtaAbsLocal=it->id().ietaAbs();
    const int zsideLocal=it->id().zside();
    const float etLocal=energy/cosh(etaabsLookup[iEtaAbsLocal]);

    if (etLocal>etMaxLocal) {
      etMaxLocal=etLocal;
      itMax=it;
    }

    const HcalFrontEndId& hFeid=logicalMap->getHcalFrontEndId(it->detid());
    int rbxIndexValue=hFeid.rbxIndex();
    int rmIndexValue=hFeid.rmIndex();
    rbxEnergies[rbxIndexValue]+=energy;
    rmEnergies  [rmIndexValue]+=energy;
    
    if (energy>singleChannelThreshold) {
      rbxMultiplicities[rbxIndexValue]++;
      rmMultiplicities[rmIndexValue]++;
      if (zsideLocal>0) totalEPlusLocal+=energy;
      if (zsideLocal<0) totalEMinusLocal+=energy;
    }

    //std::cout 
    //  << hFeid.rbx() << " " << hFeid.rm() 
    //  << " has rbxIndex=" << rbxIndexValue 
    //  << " and  rmIndex=" << rmIndexValue 
    //  << std::endl;

    //increment flag counters
    HcalSubdetector subdet=it->id().subdet();
    if (subdet==HcalBarrel || subdet==HcalEndcap) {
      if (it->flags()&0x1) {  //eventually, HcalCaloFlagLabels::HBHEHpdHitMultiplicity
	multFlagCountLocal++;
      }
      if (it->flags()&0x2) { //eventually, HcalCaloFlagLabels::HBHEPulseShape
	psFlagCountLocal++;
      }
    }

  }

  //fill flag variables
  *multFlagCount.get()=multFlagCountLocal;
  *psFlagCount.get()=psFlagCountLocal;

  *totalEPlus.get()=totalEPlusLocal;
  *totalEMinus.get()=totalEMinusLocal;

  //fill RBX variables
  for (unsigned int iRbxThreshold=0;iRbxThreshold<singleRbxThresholds.size();++iRbxThreshold) {
    double threshold=singleRbxThresholds.at(iRbxThreshold);

    std::auto_ptr<int>                  rbxCount       ( new int()                 ) ;
    std::auto_ptr<std::vector<double> > rbxE           ( new std::vector<double>() ) ;
    std::auto_ptr<std::vector<int> >    rbxMult        ( new std::vector<int>()    ) ;
    std::auto_ptr<std::vector<int> >    rbxSector      ( new std::vector<int>()    ) ;
    std::auto_ptr<std::vector<int> >    rbxZ           ( new std::vector<int>()    ) ;

    int rbxCountLocal=0;
    for (int iRbx=0;iRbx<=HcalFrontEndId::maxRbxIndex;++iRbx) {
      if (rbxEnergies[iRbx]>threshold) {
	rbxE->push_back(rbxEnergies[iRbx]);
	rbxSector->push_back(getRbxSector(iRbx));
	rbxZ->push_back(getRbxZ(iRbx));
	rbxMult->push_back(rbxMultiplicities[iRbx]);
	rbxCountLocal++;
      }
    }
    *rbxCount.get()=rbxCountLocal;

    std::stringstream tempss;
    tempss << iRbxThreshold;
    iEvent.put( rbxCount           , Prefix + "rbxCount"       + tempss.str() + Suffix );
    iEvent.put( rbxE               , Prefix + "rbxE"           + tempss.str() + Suffix );
    iEvent.put( rbxMult            , Prefix + "rbxMult"        + tempss.str() + Suffix );
    iEvent.put( rbxSector          , Prefix + "rbxSector"      + tempss.str() + Suffix );
    iEvent.put( rbxZ               , Prefix + "rbxZ"           + tempss.str() + Suffix );
  }

  //fill RM variables
  for (unsigned int iRmThreshold=0;iRmThreshold<singleRmThresholds.size();++iRmThreshold) {
    double threshold=singleRmThresholds.at(iRmThreshold);

    std::auto_ptr<int>                  rmCount       ( new int()                 ) ;
    std::auto_ptr<std::vector<double> > rmE           ( new std::vector<double>() ) ;
    std::auto_ptr<std::vector<int> >    rmMult        ( new std::vector<int>()    ) ;

    int rmCountLocal=0;
    for (int iRm=0;iRm<=HcalFrontEndId::maxRmIndex;++iRm) {
      if (rmEnergies[iRm]>threshold) {
	rmE->push_back(rmEnergies[iRm]);
	rmMult->push_back(rmMultiplicities[iRm]);
	rmCountLocal++;
      }
    }
    *rmCount.get()=rmCountLocal;

    std::stringstream tempss;
    tempss << iRmThreshold;
    iEvent.put( rmCount           , Prefix + "rmCount"       + tempss.str() + Suffix );
    iEvent.put( rmE               , Prefix + "rmE"           + tempss.str() + Suffix );
    iEvent.put( rmMult            , Prefix + "rmMult"        + tempss.str() + Suffix );
  }

  //fill max variables
  if (etMaxLocal>-1000.0) {
    *etMax.get()=etMaxLocal;
    *tMax.get()=itMax->time();
    *iEtaMax.get()=itMax->id().ieta();
    *iPhiMax.get()=itMax->id().iphi();
    *depthMax.get()=itMax->id().depth();
  }
  else {  //dummy values
    *etMax.get()=-1000.0;
    *tMax.get()=-1000.0;
    *iEtaMax.get() =-100;
    *iPhiMax.get() =-100;
    *depthMax.get()=-100;
  }

  //put remaining stuff in the event
  iEvent.put( totalEPlus         , Prefix + "totalEPlus"     + Suffix );
  iEvent.put( totalEMinus        , Prefix + "totalEMinus"    + Suffix );

  iEvent.put( etMax              , Prefix + "etMax"          + Suffix );
  iEvent.put( tMax               , Prefix + "tMax"           + Suffix );
  iEvent.put( iEtaMax            , Prefix + "iEtaMax"        + Suffix );
  iEvent.put( iPhiMax            , Prefix + "iPhiMax"        + Suffix );
  iEvent.put( depthMax           , Prefix + "depthMax"       + Suffix );
						             
  iEvent.put( multFlagCount      , Prefix + "multFlagCount"  + Suffix );
  iEvent.put( psFlagCount        , Prefix + "psFlagCount"    + Suffix );
}

#endif
