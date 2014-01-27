#ifndef SUSY_CAF_DQMFLAGS
#define SUSY_CAF_DQMFLAGS

#include "Python.h"
#include <sstream>

#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/Framework/interface/Frameworkfwd.h"

#include <vector>

using namespace std;

class SusyCAF_DQMFlags : public edm::EDProducer {

public: 
	explicit SusyCAF_DQMFlags(const edm::ParameterSet&);

private: 
	void produce( edm::Event &, const edm::EventSetup & );
	void beginJob(); 

	vector<string> flagNames;			// flgIdx -> Name
	map<int, vector<int> > runFlagMap;		// run -> vector of flags(flgIdx)
	bool AskRR(vector<int>& flags,int runNo);	// ask the run registry and fill flags	
        string server;                                  // xmlrpc server (aka run registry)
};

#endif
