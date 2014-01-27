#ifndef SUSY_CAF_DCSBITS
#define SUSY_CAF_DCSBITS

#include <sstream>

#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/Framework/interface/Frameworkfwd.h"

#include "DataFormats/Scalers/interface/DcsStatus.h"

#include <vector>

using namespace std;

class SusyCAF_DCSBits : public edm::EDProducer {

public: 
	explicit SusyCAF_DCSBits(const edm::ParameterSet&);

private: 
	void produce( edm::Event &, const edm::EventSetup & );
	void beginJob(); 

	void GiveBits( edm::Handle<DcsStatusCollection> &, std::map<std::string,bool>* ); // fill DCS bits into map
	
};

#endif
