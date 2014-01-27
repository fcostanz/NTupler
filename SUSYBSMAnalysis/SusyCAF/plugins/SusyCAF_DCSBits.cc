#include "SUSYBSMAnalysis/SusyCAF/interface/SusyCAF_DCSBits.h"
#include "FWCore/Framework/interface/Event.h"

SusyCAF_DCSBits::SusyCAF_DCSBits(const edm::ParameterSet& iConfig)
{
	produces <bool>                        ( "DCSBitsHandleValid" );
	produces <std::map<std::string,bool> > ( "DCSBits" );
}

void SusyCAF_DCSBits::produce(edm::Event& iEvent, const edm::EventSetup& iSetup) {

	std::auto_ptr<bool>                        handleValid    (new bool(false));
	std::auto_ptr<std::map<std::string,bool> > dcsbits        (new std::map<std::string,bool>());

	edm::Handle<DcsStatusCollection> dcsStatus;
	iEvent.getByLabel("scalersRawToDigi", dcsStatus);
	if(dcsStatus.isValid()){
		*handleValid.get()=true;
		GiveBits(dcsStatus,dcsbits.get());
	}
	iEvent.put( handleValid,    "DCSBitsHandleValid" );
	iEvent.put( dcsbits,        "DCSBits" );
}

void SusyCAF_DCSBits::beginJob(){}

void SusyCAF_DCSBits::GiveBits(edm::Handle<DcsStatusCollection>& dcsStatus, std::map<std::string,bool>* dcsbits) {

	for (DcsStatusCollection::const_iterator dcsStatusItr = dcsStatus->begin(); 
		dcsStatusItr != dcsStatus->end(); ++dcsStatusItr) {
	(*dcsbits)["CSCp"]   = dcsStatusItr->ready(DcsStatus::CSCp); 
	(*dcsbits)["CSCm"]   = dcsStatusItr->ready(DcsStatus::CSCm);   
	(*dcsbits)["DT0"]    = dcsStatusItr->ready(DcsStatus::DT0);
	(*dcsbits)["DTp"]    = dcsStatusItr->ready(DcsStatus::DTp);
	(*dcsbits)["DTm"]    = dcsStatusItr->ready(DcsStatus::DTm);
	(*dcsbits)["EBp"]    = dcsStatusItr->ready(DcsStatus::EBp);
	(*dcsbits)["EBm"]    = dcsStatusItr->ready(DcsStatus::EBm);
	(*dcsbits)["EEp"]    = dcsStatusItr->ready(DcsStatus::EEp);
	(*dcsbits)["EEm"]    = dcsStatusItr->ready(DcsStatus::EEm);
	(*dcsbits)["ESp"]    = dcsStatusItr->ready(DcsStatus::EEp);
	(*dcsbits)["ESm"]    = dcsStatusItr->ready(DcsStatus::ESm); 
	(*dcsbits)["HBHEa"]  = dcsStatusItr->ready(DcsStatus::HBHEa);
	(*dcsbits)["HBHEb"]  = dcsStatusItr->ready(DcsStatus::HBHEb);
	(*dcsbits)["HBHEc"]  = dcsStatusItr->ready(DcsStatus::HBHEc); 
	(*dcsbits)["HF"]     = dcsStatusItr->ready(DcsStatus::HF);
	(*dcsbits)["HO"]     = dcsStatusItr->ready(DcsStatus::HO);
	(*dcsbits)["BPIX"]   = dcsStatusItr->ready(DcsStatus::BPIX);
	(*dcsbits)["FPIX"]   = dcsStatusItr->ready(DcsStatus::FPIX);
	(*dcsbits)["RPC"]    = dcsStatusItr->ready(DcsStatus::RPC);
	(*dcsbits)["TIBTID"] = dcsStatusItr->ready(DcsStatus::TIBTID);
	(*dcsbits)["TOB"]    = dcsStatusItr->ready(DcsStatus::TOB);
	(*dcsbits)["TECp"]   = dcsStatusItr->ready(DcsStatus::TECp);
	(*dcsbits)["TECm"]   = dcsStatusItr->ready(DcsStatus::TECm);
	(*dcsbits)["CASTOR"] = dcsStatusItr->ready(DcsStatus::CASTOR);

  }


}
