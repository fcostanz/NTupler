import FWCore.ParameterSet.Config as cms

susycafdqmflags = cms.EDProducer("SusyCAF_DQMFlags",

		server  = cms.string('http://pccmsdqm04.cern.ch/runregistry/xmlrpc'),
		
		# return values: 
		#  1=GOOD, -1=BAD, 0=EXCL, 10=NOTSET 
		#  99=not accessed by technical reasons (e.g. RR not available)
		ListOfDQMFlags = cms.vstring(
                                "CASTOR",
				"CSC", 
				"DT", 
				"ECAL",
				"EGAM",
				"ES",
				"HCAL",
				"HLT",
				"JMET",
				"L1T",
				"MUON",
				"PIX",
				"RPC",
				"STRIP",
				"TRACK"
                             )
)

