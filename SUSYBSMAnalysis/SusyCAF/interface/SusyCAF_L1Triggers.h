#ifndef SUSY_CAF_L1TRIGGERS
#define SUSY_CAF_L1TRIGGERS

#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Utilities/interface/InputTag.h"

#include "FWCore/Framework/interface/ESHandle.h"
#include "CondFormats/L1TObjects/interface/L1GtTriggerMenu.h"
#include "CondFormats/DataRecord/interface/L1GtTriggerMenuRcd.h"
#include "DataFormats/L1GlobalTrigger/interface/L1GlobalTriggerReadoutRecord.h"
#include "CondFormats/L1TObjects/interface/L1GtPrescaleFactors.h"
#include "CondFormats/DataRecord/interface/L1GtPrescaleFactorsAlgoTrigRcd.h"
#include "CondFormats/DataRecord/interface/L1GtPrescaleFactorsTechTrigRcd.h"
#include "boost/lexical_cast.hpp"

class SusyCAF_L1Triggers : public edm::EDProducer 
{
 public: 
  explicit SusyCAF_L1Triggers(const edm::ParameterSet& conf) : 
    nBx(conf.getParameter<int>("NBx")),
    inputTag(conf.getParameter<edm::InputTag>("InputTag")),
    outputName( (nBx==0? "" : ( nBx<0 ? "M" : "P") + boost::lexical_cast<std::string>(abs(nBx)) ) )
      {
	produces <bool>                        ( "L1HandleValid"  +outputName );
	produces <unsigned int>                ( "physicsDeclared"+outputName );
	produces <std::map<std::string,bool> > ( "L1triggered"    +outputName );
	produces <std::map<std::string,int> >  ( "L1prescaled"    +outputName );
      }
    
 private: 
  const int nBx;
  const edm::InputTag inputTag;
  const std::string outputName;
  
  void produce( edm::Event& event, const edm::EventSetup& setup) {
    edm::Handle<L1GlobalTriggerReadoutRecord> L1record;  event.getByLabel(inputTag, L1record);
    edm::ESHandle<L1GtTriggerMenu> L1menu;               setup.get<L1GtTriggerMenuRcd>().get(L1menu) ;
    edm::ESHandle<L1GtPrescaleFactors> psAlgo;           setup.get<L1GtPrescaleFactorsAlgoTrigRcd>().get(psAlgo);
    edm::ESHandle<L1GtPrescaleFactors> psTech;           setup.get<L1GtPrescaleFactorsTechTrigRcd>().get(psTech);
    
    std::auto_ptr<unsigned int>                 physicsDeclared(new unsigned int());
    std::auto_ptr<std::map<std::string,bool> >  triggered      (new std::map<std::string,bool>());
    std::auto_ptr<std::map<std::string,int> >   prescaled      (new std::map<std::string,int>());
    
    if (L1record.isValid() && L1menu.isValid()) {
      *physicsDeclared.get()=L1record->gtFdlWord(nBx).physicsDeclared();
      
      record( L1menu->gtAlgorithmMap(),        
	      triggered, L1record->decisionWord(nBx),         
	      prescaled, psAlgo->gtPrescaleFactors().at(L1record->gtFdlWord(nBx).gtPrescaleFactorIndexAlgo()));
      
      record( L1menu->gtTechnicalTriggerMap(), 
	      triggered, L1record->technicalTriggerWord(nBx), 
	      prescaled, psTech->gtPrescaleFactors().at(L1record->gtFdlWord(nBx).gtPrescaleFactorIndexTech()));
    }
    
    event.put( std::auto_ptr<bool>(new bool(L1record.isValid()&&L1menu.isValid())),  "L1HandleValid"  +outputName );
    event.put( physicsDeclared,"physicsDeclared"+outputName );
    event.put( triggered,      "L1triggered"    +outputName );
    event.put( prescaled,      "L1prescaled"    +outputName );
  }
  
  void record(const AlgorithmMap& algoMap,
	      std::auto_ptr<std::map<std::string,bool> >& mapSb, const std::vector<bool>& vbool, 
	      std::auto_ptr<std::map<std::string,int> >& mapSu, const std::vector<int>& vint
	      ) const {
    for( AlgorithmMap::const_iterator it = algoMap.begin(); it != algoMap.end(); ++it) {
      (*mapSb)[it->first] = vbool.at(it->second.algoBitNumber());
      (*mapSu)[it->first] = vint.at(it->second.algoBitNumber());
    }
  }
  
};

#endif
