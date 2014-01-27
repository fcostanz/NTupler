#ifndef VERTEX_MATCH
#define VERTEX_MATCH

#include "DataFormats/VertexReco/interface/VertexFwd.h"
#include "DataFormats/VertexReco/interface/Vertex.h"

struct SusyCAF_functions {

  static math::XYZPoint closestDzPrimaryVertexPosition(const reco::Track* track,const reco::VertexCollection& vertices) {
    reco::VertexCollection::const_iterator 
      it(vertices.begin()), 
      end(vertices.end()), 
      closest(it);
    while(++it!=end) 
      if(fabs(track->dz(it->position())) < fabs(track->dz(closest->position())) ) 
	closest = it;
    return closest->position();
  }
  
};

#endif
