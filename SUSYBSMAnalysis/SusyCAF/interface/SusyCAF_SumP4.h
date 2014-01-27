#include <memory>
#include <string>
#include <vector>
#include <map>

#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDProducer.h"

#include "FWCore/Framework/interface/Event.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "DataFormats/Candidate/interface/Candidate.h"

class SusyCAF_SumP4 : public edm::EDProducer {
 public:
  explicit SusyCAF_SumP4(const edm::ParameterSet& cfg) :
    Prefix(cfg.getParameter<std::string>("Prefix")),
    Suffix(cfg.getParameter<std::string>("Suffix")),
    inputTag(cfg.getParameter<edm::InputTag>("InputTag")) 
      {	produces<reco::Candidate::LorentzVector>(Prefix+"SumP4"+Suffix);}

 private:
  std::string Prefix;
  std::string Suffix;
  edm::InputTag inputTag;

  typedef edm::View<reco::Candidate> collection;
  void produce(edm::Event& evt, const edm::EventSetup&) {
    edm::Handle<collection> candidates;
    evt.getByLabel(inputTag,candidates);

    std::auto_ptr<reco::Candidate::LorentzVector> sumP4( new reco::Candidate::LorentzVector());
    for(collection::const_iterator it = candidates->begin(); it != candidates->end() ; ++it)
      *sumP4 += it->p4();
    evt.put(sumP4, Prefix+"SumP4" + Suffix);
  }
};
