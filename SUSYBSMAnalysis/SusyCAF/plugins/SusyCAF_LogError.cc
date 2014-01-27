#include "SUSYBSMAnalysis/SusyCAF/interface/SusyCAF_LogError.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/MessageLogger/interface/ErrorSummaryEntry.h"
#include "boost/foreach.hpp"

std::set<std::string> SusyCAF_LogError::setFromVector(std::vector<std::string> vec) {return std::set<std::string>(vec.begin(),vec.end());}

SusyCAF_LogError::SusyCAF_LogError(const edm::ParameterSet& iConfig) :
  inputTag(iConfig.getParameter<edm::InputTag>("InputTag")),
  Prefix(iConfig.getParameter<std::string>("Prefix")),
  Suffix(iConfig.getParameter<std::string>("Suffix")),
  modulesOfInterest(setFromVector(iConfig.getParameter<std::vector<std::string> >("ModulesOfInterest"))),
  categoriesOfInterest(setFromVector(iConfig.getParameter<std::vector<std::string> >("CategoriesOfInterest")))
{
  BOOST_FOREACH(std::string err, modulesOfInterest)    assert(categoriesOfInterest.count(err)==0);
  BOOST_FOREACH(std::string err, modulesOfInterest)    produces<bool>(Prefix + err + Suffix);
  BOOST_FOREACH(std::string err, categoriesOfInterest) produces<bool>(Prefix + err + Suffix);
  produces<bool> (Prefix + "Any" + Suffix);
  produces<bool> (Prefix + "HandleValid" +Suffix);
}

void SusyCAF_LogError::
produce(edm::Event& iEvent, const edm::EventSetup& iSetup) {

  std::set<std::string>
    modules(modulesOfInterest),
    categories(categoriesOfInterest);

  typedef edm::ErrorSummaryEntry       Error;
  typedef std::vector<Error> Errors;

  edm::Handle<Errors> errors; iEvent.getByLabel(inputTag,errors);
  
  if(errors.isValid()) {
    BOOST_FOREACH(const Error &err, *errors) {
      if(modules.count(err.module)) {
	iEvent.put(std::auto_ptr<bool>( new bool(true)), Prefix + err.module   + Suffix ); modules.erase(err.module); }
      if(categories.count(err.category)) {
	iEvent.put(std::auto_ptr<bool>( new bool(true)), Prefix + err.category + Suffix ); categories.erase(err.category); }
    }
  }
  BOOST_FOREACH(std::string err, modules)    iEvent.put(std::auto_ptr<bool>( new bool(false)), Prefix + err + Suffix );
  BOOST_FOREACH(std::string err, categories) iEvent.put(std::auto_ptr<bool>( new bool(false)), Prefix + err + Suffix );
  iEvent.put(std::auto_ptr<bool> (new bool(errors.isValid() && errors->size())), Prefix + "Any" + Suffix);
  iEvent.put(std::auto_ptr<bool>( new bool(errors.isValid())), Prefix + "HandleValid" + Suffix);
}
