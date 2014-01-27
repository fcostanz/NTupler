#include "SUSYBSMAnalysis/SusyCAF/interface/SusyCAF_Track.h"
#include "FWCore/Framework/interface/Event.h"
#include "DataFormats/VertexReco/interface/VertexFwd.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

const std::string SusyCAF_Track::vertex_names[2] = {"Primary", "PileUp"};

SusyCAF_Track::SusyCAF_Track(const edm::ParameterSet& iConfig)
  : inputTag        (iConfig.getParameter<edm::InputTag>("InputTag"         ))
  , primaryVertexTag(iConfig.getParameter<edm::InputTag>("PrimaryVertexTag" ))
  , prefix          (iConfig.getParameter<std::string>  ("Prefix"           ))
  , suffix          (iConfig.getParameter<std::string>  ("Suffix"           ))
  , maxD0           (iConfig.getParameter<double>       ("MaxD0"            ))
  , qualities       (iConfig.getParameter<std::vector<int> > ("Qualities"        ))
  , pixelSeedOnly   (iConfig.getParameter<bool>         ("PixelSeedOnly"    ))
  , scraping        (iConfig.getParameter<bool>         ("ScrapingVars"))
{
  for(unsigned q=0; q<qualities.size(); ++q) {
    std::string name = (qualities[q] < 0 ? "All" : reco::Track::qualityNames[qualities[q]]) + (pixelSeedOnly? "PixelTracks" : "Tracks") + suffix;
    name[0] = toupper((unsigned char) name[0]);
    for(unsigned v=VINIT; v<VSIZE; v++) {
      std::string name2 = "with" + vertex_names[v] + name;
      produces<unsigned>(prefix + "Count" + name2 );
      produces<double>  (prefix + "SumPT" + name2 );
      produces<Vector>  (prefix + "SumP3" + name2 );
    }
  }
  if(scraping) {
    produces<unsigned> ("nTracksAll");
    produces<unsigned> ("nTracksHighPurity");
  }
}

void SusyCAF_Track::
produce(edm::Event& iEvent, const edm::EventSetup& iSetup) 
{
  unsigned nTracksAll(0), nTracksHighPurity(0); //without any cut
  std::vector< std::vector< Vector > > sump3(VSIZE, std::vector< Vector >(qualities.size(), Vector(0.,0.,0.) )); 
  std::vector< std::vector< double > > sumpt(VSIZE, std::vector< double >(qualities.size(), 0.));
  std::vector< std::vector<unsigned> > ntrks(VSIZE, std::vector<unsigned>(qualities.size(), 0));
  
  edm::Handle<reco::VertexCollection> vertices;   iEvent.getByLabel(primaryVertexTag, vertices);
  edm::Handle<reco::TrackCollection>  tracks;     iEvent.getByLabel(inputTag        , tracks  );
  if (vertices.isValid() && tracks.isValid()) {
    const reco::Vertex& primaryVertex = vertices->front();
    for(reco::TrackCollection::const_iterator track = tracks->begin(); track!=tracks->end(); ++track ) {

      if( track->quality(reco::Track::highPurity) ) 
	nTracksHighPurity++;
      nTracksAll++;

      if( (    maxD0  < 0 || fabs(track->dxy(primaryVertex.position())) < maxD0 ) &&
	  (!pixelSeedOnly || ( track->algo()==reco::TrackBase::iter0 || 
			       track->algo()==reco::TrackBase::iter1 || 
			       track->algo()==reco::TrackBase::iter2 || 
			       track->algo()==reco::TrackBase::iter3))) {

	std::vector<double> dist; std::transform(vertices->begin(), vertices->end(), back_inserter(dist), dsz(*track) );
	std::vector<double>::const_iterator dmin = min_element(dist.begin(), dist.end());
	unsigned v = (dmin==dist.begin()) ? V_PRIMARY : V_PILEUP;

	for(unsigned q=0; q<qualities.size(); ++q) {
	  ntrks[v][q]++;
	  sump3[v][q] += track->momentum();
	  sumpt[v][q] += track->pt();
	}
      }
    }
  }
  
  for(unsigned q=0; q<qualities.size(); ++q) {
    std::string name = (qualities[q] < 0 ? "All" : reco::Track::qualityNames[qualities[q]]) + (pixelSeedOnly? "PixelTracks" : "Tracks") + suffix;
    name[0] = toupper((unsigned char) name[0]);
    for(unsigned v=VINIT; v<VSIZE; v++) {
      std::string name2 = "with" + vertex_names[v] + name;
      iEvent.put( std::auto_ptr<unsigned>( new unsigned(ntrks[v][q]) ), prefix + "Count" + name2 );
      iEvent.put( std::auto_ptr< double > ( new double( sumpt[v][q]) ), prefix + "SumPT" + name2 );
      iEvent.put( std::auto_ptr< Vector > ( new Vector( sump3[v][q]) ), prefix + "SumP3" + name2 );
    }
  }
  if(scraping) {
    iEvent.put( std::auto_ptr< unsigned>( new unsigned(nTracksAll)), "nTracksAll");
    iEvent.put( std::auto_ptr< unsigned>( new unsigned(nTracksHighPurity)), "nTracksHighPurity");
  }
}
