#include "FWCore/Framework/interface/Event.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "SUSYBSMAnalysis/SusyCAF/interface/SusyCAF_DQMFlags.h"

SusyCAF_DQMFlags::SusyCAF_DQMFlags( const edm::ParameterSet& iConfig ) 
		: flagNames( iConfig.getParameter<vector<string> > ("ListOfDQMFlags") ),
		  server( iConfig.getParameter<string> ("server") )
{
	produces <std::map<std::string,int> > ( "DQMFlags" );
}

void SusyCAF_DQMFlags::produce( edm::Event& iEvent, const edm::EventSetup& iSetup ) {

	int runNo = iEvent.id().run();

	// check the map if run# exists
	if( runFlagMap.find(runNo) == runFlagMap.end() ){
		// a new run# we are going to access the run registry
		// accessing runFlagMap[runNo] automatically creates a new flag vector
		if( !AskRR( runFlagMap[runNo], runNo ) )
			 edm::LogError("Run Registry access error");
	}


	std::auto_ptr<std::map<std::string,int> > dqmflags (new std::map<std::string,int>());


	// export flags into iEvent
	for( unsigned int i=0; i<flagNames.size(); i++ )
	  (*dqmflags)[flagNames.at(i)] = runFlagMap[runNo][i];

	iEvent.put( dqmflags,        "DQMFlags" );
	
}

void SusyCAF_DQMFlags::beginJob(){}


bool SusyCAF_DQMFlags::AskRR( vector<int>& flags, int runNo ) {

	if( flagNames.size()==0 ) return false;
	flags.resize( flagNames.size() );

	Py_Initialize();
	PyObject * module;
	PyObject * dict;

	module = PyImport_AddModule( "__main__" );
	dict   = PyModule_GetDict( module );

	// library for xmlrpc 
	PyRun_SimpleString( "import xmlrpclib\n" );

	// open server and ask for all flags
	string serverstring = "server = xmlrpclib.ServerProxy('" + server + "')\n";
	PyRun_SimpleString( serverstring.c_str() );
	char request[100];
	sprintf( request, "data = server.DataExporter.export('RUN','GLOBAL', 'xml_datasets', {'runNumber': '%i'})\n", runNo );
	
	PyRun_SimpleString( request );

	// library for xml parsing
	PyRun_SimpleString( "from xml.sax import make_parser, SAXException, parseString\n" );
	PyRun_SimpleString( "from xml.sax.handler import ContentHandler\n" );

	stringstream flagstringstream;

	// python allFlags list
	flagstringstream << "allFlags=[\"CMP_" << flagNames[0];
	for( unsigned int i=1; i<flagNames.size(); i++ ) 
		flagstringstream << "\",\"CMP_" << flagNames[i];
	flagstringstream << "\"]\n";
	
	// python allFlagValuesi list
	flagstringstream <<"allFlagValuesi="<<flagNames.size()<<"*[99]"<<"\n"<<endl;

	PyRun_SimpleString( flagstringstream.str().c_str() );

	// python class def of ContentHandler used in parseing
	PyRun_SimpleString(
		"class FindFlags(ContentHandler):\n"
		"\tdef __init__(self, flagN):\n"
		"\t\tself.Index=flagN\n"
		"\t\tself.search_flag = allFlags[self.Index]\n"
		"\t\tself.isflag = 0\n"
		"\t\tself.flagvalue = \"\"\n"
		"\tdef startElement(self, name, attrs):\n"
		"\t\tself.isflag=0\n"
		"\t\tif name == self.search_flag:\n"
		"\t\t\tself.isflag = 1\n"
		"\tdef characters(self, text):\n"
		"\t\tif self.isflag ==1:\n"
		"\t\t\tself.flagvalue=text\n"
		"\t\t\tif self.flagvalue==\"GOOD\":\n"
		"\t\t\t\tallFlagValuesi[self.Index]=1\n"
		"\t\t\tif self.flagvalue==\"BAD\":\n"
		"\t\t\t\tallFlagValuesi[self.Index]=-1\n"
		"\t\t\tif self.flagvalue==\"EXCL\":\n"
		"\t\t\t\tallFlagValuesi[self.Index]=0\n"
		"\t\t\tif self.flagvalue==\"NOTSET\":\n"
		"\t\t\t\tallFlagValuesi[self.Index]=10\n"
	);
	
	// parse
	PyRun_SimpleString(
		"for inte in range(len(allFlags)):\n"
		"\tparseString(data,FindFlags(inte))\n"
	);
	
	stringstream setVariables;
	// fli neccessary?
	setVariables << "for inte in range(len(allFlags)):\n";
	for(unsigned int i=0;i<flagNames.size();i++) 
		setVariables << "\tif allFlags[inte]==\"CMP_" << flagNames[i] << "\"  : "
			     << flagNames[i] << "fli  =allFlagValuesi[inte]\n";
	PyRun_SimpleString( setVariables.str().c_str() );



	// read flags from python into flags vector	
	for( unsigned int i=0; i<flagNames.size(); i++ ) {
		char itemName[100];
		strcpy( itemName, flagNames[i].c_str() );
		strcat( itemName, "fli" );
		PyObject* pyItem = PyMapping_GetItemString( dict, itemName );
		if(pyItem != NULL) {
			flags[i]=PyInt_AsLong( pyItem );
			Py_DECREF(pyItem);
		} else  flags[i]=99;
	}

	bool  success=true;
	if(PyErr_Occurred()) success=false;;
	
	Py_Finalize();

	return success;

}
