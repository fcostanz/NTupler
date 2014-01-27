#include "SUSYBSMAnalysis/SusyCAF/interface/SusyCAF_L1GlobalTrigger.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "DataFormats/L1GlobalTrigger/interface/L1GlobalTriggerReadoutRecord.h"

//to access trigger names
#include "FWCore/Framework/interface/ESHandle.h"
#include "CondFormats/L1TObjects/interface/L1GtTriggerMenu.h"
#include "CondFormats/DataRecord/interface/L1GtTriggerMenuRcd.h"

template <typename T>
void fillIntVector(std::auto_ptr<std::vector<int> >& destination,const T& source )
{
  destination->clear();
  for (unsigned int i=0;i<source.size();++i) {
    destination->push_back(source[i]);
  }
}

SusyCAF_L1GlobalTrigger::SusyCAF_L1GlobalTrigger(const edm::ParameterSet& iConfig) :
  inputTag(iConfig.getParameter<edm::InputTag>("InputTag")),
  nBxOutput(iConfig.getParameter<int>         ("NBxOutput")),
  storeByName(iConfig.getParameter<bool>       ("StoreByName")),
  algoNameList(iConfig.getParameter<std::vector<std::string> > ("ListOfTriggerNames"))
{
  //details here: https://twiki.cern.ch/twiki/bin/view/CMS/SWGuideL1TriggerFAQ

  produces <unsigned int>      ( "bx"         );
  produces <std::vector<int> > ( "l1physbits" );
  produces <std::vector<int> > ( "l1techbits" );

  if (nBxOutput==3) {
    produces <std::vector<int> > ( "l1physbitsm1" );
    produces <std::vector<int> > ( "l1techbitsm1" );
    produces <std::vector<int> > ( "l1physbitsp1" );
    produces <std::vector<int> > ( "l1techbitsp1" );
  }

  if (storeByName) {
    fillAlgoNameListNoUnderScores();
    for (std::vector<std::string>::const_iterator it=algoNameListNoUnderScores.begin(); it!=algoNameListNoUnderScores.end();++it) {
      produces <bool> ( *it );
    }
  }

}

void SusyCAF_L1GlobalTrigger::endJob()
{

  //make set
  std::set<std::string> algoNameSet;
  for (std::vector<std::string>::const_iterator it=algoNameList.begin();it!=algoNameList.end();++it) {
    algoNameSet.insert(*it);
  }

  //decide whether to print a report
  bool printReport=false;
  for (std::set<std::string>::const_iterator it=algosPresentInData.begin();it!=algosPresentInData.end();++it) {
    if (algoNameSet.count(*it)==0) printReport=true;
  }
  if (!printReport) return;

  //print a report
  std::cout << "/------------------------------------------\\" << std::endl;
  std::cout << "|      SusyCAF_L1GlobalTrigger report      |" << std::endl;
  std::cout << "|------------------------------------------|" << std::endl;
  std::cout << "| There are triggers in the data that are  |" << std::endl;
  std::cout << "| not in the config file.  Please paste    |" << std::endl;
  std::cout << "| this code into ListOfTriggerNames in     |" << std::endl;
  std::cout << "| SusyCAF_L1GlobalTrigger_cfi.py           |" << std::endl;
  std::cout << std::endl;
  for (std::set<std::string>::const_iterator it=algosPresentInData.begin();it!=algosPresentInData.end();++it) {
    if (algoNameSet.count(*it)==0) std::cout << "\"" << *it << "\"," << std::endl;
  }
  std::cout << std::endl;
  std::cout << "|------------------------------------------|" << std::endl;  
  std::cout << "|    end SusyCAF_L1GlobalTrigger report    |" << std::endl;
  std::cout <<"\\------------------------------------------/" << std::endl;
}

void SusyCAF_L1GlobalTrigger::
produce(edm::Event& iEvent, const edm::EventSetup& iSetup) {
  std::auto_ptr<unsigned int>      bx            ( new unsigned int()     );
  std::auto_ptr<std::vector<int> > l1physbits    ( new std::vector<int>() );
  std::auto_ptr<std::vector<int> > l1techbits    ( new std::vector<int>() );
  std::auto_ptr<std::vector<int> > l1physbitsm1  ( new std::vector<int>() );
  std::auto_ptr<std::vector<int> > l1techbitsm1  ( new std::vector<int>() );
  std::auto_ptr<std::vector<int> > l1physbitsp1  ( new std::vector<int>() );
  std::auto_ptr<std::vector<int> > l1techbitsp1  ( new std::vector<int>() );

  //contains decision words before masks
  edm::Handle<L1GlobalTriggerReadoutRecord> l1GtReadoutRecord;
  iEvent.getByLabel(inputTag, l1GtReadoutRecord);

  *bx.get()=l1GtReadoutRecord->gtfeWord().bxNr();

  const DecisionWord dWord=l1GtReadoutRecord->decisionWord();
  fillIntVector(l1physbits,dWord);
  fillIntVector(l1techbits,l1GtReadoutRecord->technicalTriggerWord());

  iEvent.put( bx,         "bx"         );
  iEvent.put( l1physbits, "l1physbits" );
  iEvent.put( l1techbits, "l1techbits" );

  //in case nBxOutput==3
  if (nBxOutput==3 && l1GtReadoutRecord->gtFdlVector().size()>=3) {
    fillIntVector(l1physbitsm1,l1GtReadoutRecord->decisionWord(-1));
    fillIntVector(l1physbitsp1,l1GtReadoutRecord->decisionWord( 1));

    fillIntVector(l1techbitsm1,l1GtReadoutRecord->technicalTriggerWord(-1));
    fillIntVector(l1techbitsp1,l1GtReadoutRecord->technicalTriggerWord( 1));

    iEvent.put( l1physbitsm1, "l1physbitsm1" );
    iEvent.put( l1techbitsm1, "l1techbitsm1" );
    iEvent.put( l1physbitsp1, "l1physbitsp1" );
    iEvent.put( l1techbitsp1, "l1techbitsp1" );
  }

  //in case storeByName
  if (storeByName) {
    edm::ESHandle<L1GtTriggerMenu> menuRcd;
    iSetup.get<L1GtTriggerMenuRcd>().get(menuRcd) ;
    const L1GtTriggerMenu* menu = menuRcd.product();
    
    for (unsigned int i=0;i<algoNameList.size();i++) {
      //if trigger is not found, function returns false => vetoing isn't necessarily reliable.
      std::auto_ptr<int> algoDecision(new int( menu->gtAlgorithmResult(algoNameList[i],dWord) ) );
      iEvent.put(algoDecision,algoNameListNoUnderScores[i]);
    }
    
    //keep track of which triggers are present in the data
    if (menusAlreadyParsed.count(menu->gtTriggerMenuName())==0) {
      const AlgorithmMap& algoMap=menu->gtAlgorithmMap();
      for (AlgorithmMap::const_iterator it=algoMap.begin();it!=algoMap.end();++it) {
	algosPresentInData.insert((*it).first);
      }
      menusAlreadyParsed.insert(menu->gtTriggerMenuName());
    }

  }//end if(StoreByName)

}

void SusyCAF_L1GlobalTrigger::fillAlgoNameListNoUnderScores()
{
  std::string underScore="_";
  for (std::vector<std::string>::const_iterator it=algoNameList.begin();it!=algoNameList.end();++it) {
    std::string nameNoUnderScores;
    for (std::string::const_iterator it2=it->begin();it2!=it->end();++it2) {
      if ((*it2)!=underScore[0]) nameNoUnderScores.push_back(*it2);
    }
    algoNameListNoUnderScores.push_back(nameNoUnderScores);
  }
}
