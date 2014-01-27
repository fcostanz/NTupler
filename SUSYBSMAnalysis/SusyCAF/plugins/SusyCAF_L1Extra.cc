#include "SUSYBSMAnalysis/SusyCAF/interface/SusyCAF_L1Extra.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "DataFormats/L1Trigger/interface/L1EtMissParticle.h"
#include "DataFormats/L1Trigger/interface/L1EtMissParticleFwd.h"
#include "DataFormats/L1Trigger/interface/L1EmParticleFwd.h"
#include "DataFormats/L1Trigger/interface/L1EmParticle.h"
#include "DataFormats/L1Trigger/interface/L1JetParticleFwd.h"
#include "DataFormats/L1Trigger/interface/L1JetParticle.h"


SusyCAF_L1Extra::SusyCAF_L1Extra(const edm::ParameterSet& iConfig) :
  inputTag(iConfig.getParameter<edm::InputTag>("InputTag")),
  Prefix(iConfig.getParameter<std::string>("Prefix")),
  Suffix(iConfig.getParameter<std::string>("Suffix"))
{
  produces <std::vector<double> > ( Prefix + "ht"         + Suffix);
  produces <std::vector<double> > ( Prefix + "et"         + Suffix);
  produces <std::vector<double> > ( Prefix + "met"        + Suffix);
  produces <std::vector<double> > ( Prefix + "metphi"     + Suffix);
  produces <std::vector<double> > ( Prefix + "mht"        + Suffix);
  produces <std::vector<double> > ( Prefix + "mhtphi"     + Suffix);
  produces <std::vector<double> > ( Prefix + "cenJetet"   + Suffix);
  produces <std::vector<double> > ( Prefix + "cenJeteta"   + Suffix);
  produces <std::vector<double> > ( Prefix + "cenJetphi"   + Suffix);
  produces <std::vector<double> > ( Prefix + "cenJetbx"   + Suffix);
  produces <std::vector<double> > ( Prefix + "tauJetet"   + Suffix);
  produces <std::vector<double> > ( Prefix + "tauJeteta"   + Suffix);
  produces <std::vector<double> > ( Prefix + "tauJetphi"   + Suffix);
  produces <std::vector<double> > ( Prefix + "tauJetbx"   + Suffix);
  produces <std::vector<double> > ( Prefix + "fwdJetet"   + Suffix);
  produces <std::vector<double> > ( Prefix + "fwdJeteta"   + Suffix);
  produces <std::vector<double> > ( Prefix + "fwdJetphi"   + Suffix);
  produces <std::vector<double> > ( Prefix + "fwdJetbx"   + Suffix);
  produces <std::vector<double> > ( Prefix + "isoEmet"   + Suffix);
  produces <std::vector<double> > ( Prefix + "isoEmeta"   + Suffix);
  produces <std::vector<double> > ( Prefix + "isoEmphi"   + Suffix);
  produces <std::vector<double> > ( Prefix + "isoEmbx"   + Suffix);
  produces <std::vector<double> > ( Prefix + "nonisoEmet"   + Suffix);
  produces <std::vector<double> > ( Prefix + "nonisoEmeta"   + Suffix);
  produces <std::vector<double> > ( Prefix + "nonisoEmphi"   + Suffix);
  produces <std::vector<double> > ( Prefix + "nonisoEmbx"   + Suffix);

}

void SusyCAF_L1Extra::
produce(edm::Event& iEvent, const edm::EventSetup& iSetup) {
  std::auto_ptr<std::vector<double> > ht      ( new std::vector<double>() );
  std::auto_ptr<std::vector<double> > et      ( new std::vector<double>() );
  std::auto_ptr<std::vector<double> > met     ( new std::vector<double>() );
  std::auto_ptr<std::vector<double> > metphi  ( new std::vector<double>() );
  std::auto_ptr<std::vector<double> > mht     ( new std::vector<double>() );
  std::auto_ptr<std::vector<double> > mhtphi  ( new std::vector<double>() );
  std::auto_ptr<std::vector<double> > cenJetet ( new std::vector<double>() );
  std::auto_ptr<std::vector<double> > cenJeteta ( new std::vector<double>() );
  std::auto_ptr<std::vector<double> > cenJetphi( new std::vector<double>() );
  std::auto_ptr<std::vector<double> > cenJetbx ( new std::vector<double>() );
  std::auto_ptr<std::vector<double> > tauJetet ( new std::vector<double>() );
  std::auto_ptr<std::vector<double> > tauJeteta ( new std::vector<double>() );
  std::auto_ptr<std::vector<double> > tauJetphi( new std::vector<double>() );
  std::auto_ptr<std::vector<double> > tauJetbx ( new std::vector<double>() );
  std::auto_ptr<std::vector<double> > fwdJetet ( new std::vector<double>() );
  std::auto_ptr<std::vector<double> > fwdJeteta ( new std::vector<double>() );
  std::auto_ptr<std::vector<double> > fwdJetphi( new std::vector<double>() );
  std::auto_ptr<std::vector<double> > fwdJetbx ( new std::vector<double>() );
  std::auto_ptr<std::vector<double> > isoEmet ( new std::vector<double>() );
  std::auto_ptr<std::vector<double> > isoEmeta ( new std::vector<double>() );
  std::auto_ptr<std::vector<double> > isoEmphi( new std::vector<double>() );
  std::auto_ptr<std::vector<double> > isoEmbx ( new std::vector<double>() );
  std::auto_ptr<std::vector<double> > nonisoEmet ( new std::vector<double>() );
  std::auto_ptr<std::vector<double> > nonisoEmeta ( new std::vector<double>() );
  std::auto_ptr<std::vector<double> > nonisoEmphi( new std::vector<double>() );
  std::auto_ptr<std::vector<double> > nonisoEmbx ( new std::vector<double>() );

  //todo (possibly):
  //include the rest of the extra collections (see UserCode/L1TriggerDPG)
  //use type instead of label to distinguish between ET and HT
  //use floats instead of doubles

  //MET
  edm::Handle<l1extra::L1EtMissParticleCollection> l1EtMisss;
  iEvent.getByLabel(inputTag.label(),"MET",l1EtMisss);
  for(l1extra::L1EtMissParticleCollection::const_iterator it = l1EtMisss->begin(); it != l1EtMisss->end(); ++it) {
    et->push_back(it->etTotal());
    met->push_back(it->et());
    metphi->push_back(it->phi());
  }
  
  //MHT
  edm::Handle<l1extra::L1EtMissParticleCollection> l1HtMisss;
  iEvent.getByLabel(inputTag.label(),"MHT",l1HtMisss);
  for(l1extra::L1EtMissParticleCollection::const_iterator it = l1HtMisss->begin(); it != l1HtMisss->end(); ++it) {
    ht->push_back(it->etTotal());
    mht->push_back(it->et());
    mhtphi->push_back(it->phi());
  }
  
  //FwdJets
  edm::Handle<l1extra::L1JetParticleCollection> fwdJet;
  iEvent.getByLabel(inputTag.label(),"Forward",fwdJet);
  for(l1extra::L1JetParticleCollection::const_iterator it = fwdJet->begin(); it != fwdJet->end(); ++it){
    fwdJetet->push_back(it->et());
    fwdJeteta->push_back(it->eta());
    fwdJetphi->push_back(it->phi());
    fwdJetbx->push_back(it->bx());
    }
   
  //TauJets
  edm::Handle<l1extra::L1JetParticleCollection> tauJet;
  iEvent.getByLabel(inputTag.label(),"Tau",tauJet);
  for(l1extra::L1JetParticleCollection::const_iterator it = tauJet->begin(); it != tauJet->end(); ++it){
    tauJetet->push_back(it->et());
    tauJeteta->push_back(it->eta());
    tauJetphi->push_back(it->phi());
    tauJetbx->push_back(it->bx());
    }

  //CentralJets
  edm::Handle<l1extra::L1JetParticleCollection> cenJet;
  iEvent.getByLabel(inputTag.label(),"Central",cenJet);
  for(l1extra::L1JetParticleCollection::const_iterator it = cenJet->begin(); it != cenJet->end(); ++it){
    cenJetet->push_back(it->et());
    cenJeteta->push_back(it->eta());
    cenJetphi->push_back(it->phi());
    cenJetbx->push_back(it->bx());
    }
  
  //IsoEMJets
  edm::Handle<l1extra::L1EmParticleCollection> isoEm;
  iEvent.getByLabel(inputTag.label(),"Isolated",isoEm);
  for(l1extra::L1EmParticleCollection::const_iterator it = isoEm->begin(); it != isoEm->end(); ++it){
    isoEmet->push_back(it->et());
    isoEmeta->push_back(it->eta());
    isoEmphi->push_back(it->phi());
    isoEmbx->push_back(it->bx());
    }
  
  //nonIsoEMJets
  edm::Handle<l1extra::L1EmParticleCollection> nonisoEm;
  iEvent.getByLabel(inputTag.label(),"NonIsolated",nonisoEm);
  for(l1extra::L1EmParticleCollection::const_iterator it = nonisoEm->begin(); it != nonisoEm->end(); ++it){
    nonisoEmet->push_back(it->et());
    nonisoEmeta->push_back(it->eta());
    nonisoEmphi->push_back(it->phi());
    nonisoEmbx->push_back(it->bx());
    }
    
  
  iEvent.put( ht,     Prefix + "ht"         + Suffix);
  iEvent.put( et,     Prefix + "et"         + Suffix);
  iEvent.put( met,    Prefix + "met"        + Suffix);
  iEvent.put( metphi, Prefix + "metphi"     + Suffix);
  iEvent.put( mht,    Prefix + "mht"        + Suffix);
  iEvent.put( mhtphi, Prefix + "mhtphi"     + Suffix);
  iEvent.put( cenJetet, Prefix + "cenJetet" + Suffix);
  iEvent.put( cenJeteta, Prefix + "cenJeteta" + Suffix);
  iEvent.put( cenJetphi, Prefix + "cenJetphi" + Suffix);
  iEvent.put( cenJetbx, Prefix + "cenJetbx" + Suffix);
  iEvent.put( tauJetet, Prefix + "tauJetet" + Suffix);
  iEvent.put( tauJeteta, Prefix + "tauJeteta" + Suffix);
  iEvent.put( tauJetphi, Prefix + "tauJetphi" + Suffix);
  iEvent.put( tauJetbx, Prefix + "tauJetbx" + Suffix);
  iEvent.put( fwdJetet, Prefix + "fwdJetet" + Suffix);
  iEvent.put( fwdJeteta, Prefix + "fwdJeteta" + Suffix);
  iEvent.put( fwdJetphi, Prefix + "fwdJetphi" + Suffix);
  iEvent.put( fwdJetbx, Prefix + "fwdJetbx" + Suffix);
  iEvent.put( isoEmet, Prefix + "isoEmet" + Suffix);
  iEvent.put( isoEmeta, Prefix + "isoEmeta" + Suffix);
  iEvent.put( isoEmphi, Prefix + "isoEmphi" + Suffix);
  iEvent.put( isoEmbx, Prefix + "isoEmbx" + Suffix);
  iEvent.put( nonisoEmet, Prefix + "nonisoEmet" + Suffix);
  iEvent.put( nonisoEmeta, Prefix + "nonisoEmeta" + Suffix);
  iEvent.put( nonisoEmphi, Prefix + "nonisoEmphi" + Suffix);
  iEvent.put( nonisoEmbx, Prefix + "nonisoEmbx" + Suffix);

}
