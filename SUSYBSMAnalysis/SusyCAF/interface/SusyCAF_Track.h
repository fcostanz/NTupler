#ifndef SUSY_CAF_TRACK
#define SUSY_CAF_TRACK

#include <vector>

#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Utilities/interface/InputTag.h"
#include "DataFormats/TrackReco/interface/Track.h"
#include "DataFormats/VertexReco/interface/Vertex.h"

class SusyCAF_Track : public edm::EDProducer {
public: 
  explicit SusyCAF_Track(const edm::ParameterSet&);
private: 
  typedef reco::TrackBase::Vector     Vector;
  
  void produce          (edm::Event&, const edm::EventSetup&);
  void preselectTracks  (const reco::TrackCollection& tracks, const reco::Vertex& primaryVertex, std::vector<bool>& preselectedTracks) const;
  void computeMHT       (const reco::TrackCollection& tracks, const std::vector<bool>& preselectedTracks, reco::Track::TrackQuality quality, Vector& mht, double& ht, const bool pixelSeedOnly) const;
  void countTracks      (const reco::TrackCollection& tracks, reco::Track::TrackQuality quality, int& nEtaLT0p9, int& nEta0p9to1p5, int& nEtaGT1p5) const;

  const edm::InputTag   inputTag;
  const edm::InputTag   primaryVertexTag;
  const std::string     prefix, suffix;
  const double          maxD0;
  const std::vector<int> qualities;
  const bool pixelSeedOnly,scraping;

  enum VERTEX {VINIT=0, V_PRIMARY=0, V_PILEUP=1, VSIZE=2};
  static const std::string vertex_names[];

  struct dsz {
    const reco::Track& track;
    dsz(const reco::Track& t) : track(t) {}
    double operator()(const reco::Vertex& v) {return fabs(track.dsz(v.position()));}
  };
};


#endif
