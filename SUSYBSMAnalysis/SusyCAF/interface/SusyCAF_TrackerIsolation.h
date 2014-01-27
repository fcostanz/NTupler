#ifndef SUSY_CAF_TRACKERISOLATION
#define SUSY_CAF_TRACKERISOLATION

//-----------------------------------------------------------------------------------------
//
// Computation of the trackIsolation, for use with the isolated track veto 
// used for the stop quark search in the single lepton channel
// Author: Ben Hooberman
//
// For each PFCandidate above threshold minPt_PFCandidate store 4 quantities:
// pT of PFCandidate
// charge of PFCandidate
// dz of PFCandidate w.r.t. the 1st good vertex
// the trackIsolation value
//
// In the analysis, we veto any event containing IN ADDITION TO the selected lepton a charged PFCandidate with:
// pT > 10 GeV, dz < 0.05 cm, and trackIso/pT < 0.1
//
//-----------------------------------------------------------------------------------------



// system include files
#include <memory>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Framework/interface/ESHandle.h"

#include "DataFormats/ParticleFlowCandidate/interface/PFCandidate.h"
#include "DataFormats/ParticleFlowCandidate/interface/PFCandidateFwd.h"
#include "DataFormats/VertexReco/interface/VertexFwd.h"
#include "RecoParticleFlow/PFProducer/interface/PFMuonAlgo.h"
#include "DataFormats/ParticleFlowReco/interface/PFBlock.h"
#include "DataFormats/Common/interface/ValueMap.h"

#include "DataFormats/Math/interface/deltaR.h"
#include "DataFormats/Math/interface/deltaPhi.h"
#include "TMath.h"

#include "SUSYBSMAnalysis/SusyCAF/interface/SusyCAF_functions.h"

#include <string>
#include <vector>

typedef math::XYZTLorentzVectorF LorentzVector;
typedef math::XYZPoint Point;
using namespace reco;
using namespace edm;
using namespace std;



class SusyCAF_TrackerIsolation : public edm::EDProducer {
 public: 
  explicit SusyCAF_TrackerIsolation(const edm::ParameterSet& conf) :
    inputTag(conf.getParameter<edm::InputTag>("InputTag")),
    vertexInputTag_(conf.getParameter<edm::InputTag>("vertexInputTag")),
    dR_(conf.getParameter<double>("dR_ConeSize")), // dR value used to define the isolation cone (default 0.3 )
    dzcut_(conf.getParameter<double>("dz_CutValue")), // cut value for dz(trk,vtx) for track to include in iso sum (default 0.05)
    minPt_(conf.getParameter<double>("minPt_PFCandidate")) // store PFCandidates with pt above this cut
      {
	produces <std::vector<LorentzVector> > ( Prefix + "PFCandsP4" + Suffix );
        produces <std::vector<int> > ("PFCandsParticleId");
	produces <std::vector<float> > ("PFCandsTrkIso");
	produces <std::vector<float> > ("PFCandsDzPV");
	produces <std::vector<float> > ("PFCandsPt");
	produces <std::vector<int> > ("PFCandsChrg");
      }

 private: 
  typedef math::XYZVector Vector;
  typedef reco::Candidate::LorentzVector LorentzVector;
  const edm::InputTag inputTag;
  //  const edm::InputTag pfCandidatesTag_;
  const edm::InputTag vertexInputTag_;
  const std::string Prefix,Suffix;
  double dR_, dzcut_, minPt_;
  const reco::PFCandidateCollection *pfCandidates;


  void produce( edm::Event& event, const edm::EventSetup& setup) {

    Handle<PFCandidateCollection> pfCandidatesHandle;
    event.getByLabel(inputTag, pfCandidatesHandle);
    pfCandidates  = pfCandidatesHandle.product();

    Handle<reco::VertexCollection> vertex_h;
    event.getByLabel(vertexInputTag_, vertex_h);
    const reco::VertexCollection *vertices = vertex_h.product();

    std::auto_ptr<std::vector<LorentzVector> > pfCandsP4 ( new std::vector<LorentzVector>() );
    std::auto_ptr<std::vector<int> >           pfCandsParticleId ( new std::vector<int>() );
    std::auto_ptr<std::vector<float> >         pfCandsTrkIso ( new std::vector<float>() );
    std::auto_ptr<std::vector<float> >         pfCandsDzPV ( new std::vector<float>() );
    std::auto_ptr<std::vector<float> >         pfCandsPt ( new std::vector<float>() );
    std::auto_ptr<std::vector<int> >           pfCandsChrg ( new std::vector<int>() );



  VertexCollection::const_iterator firstGoodVertex = vertices->end();

  int firstGoodVertexIdx = 0;

  for (VertexCollection::const_iterator vtx = vertices->begin(); vtx != vertices->end(); ++vtx, ++firstGoodVertexIdx) {
    if (  !vtx->isFake() && vtx->ndof()>=4. && vtx->position().Rho()<=2.0 && fabs(vtx->position().Z())<=24.0) {
      firstGoodVertex = vtx;
      break;
    }
  }
  

  //-------------------------------------------------------------------------------------------------
  // loop over PFCandidates and calculate the trackIsolation and dz w.r.t. 1st good PV for each one
  // for neutral PFCandidates, store trkiso = 999 and dzpv = 999
  //-------------------------------------------------------------------------------------------------

  for( PFCandidateCollection::const_iterator pf_it = pfCandidates->begin(); pf_it != pfCandidates->end(); pf_it++ ) {

    //-------------------------------------------------------------------------------------
    // only store PFCandidate values if pt > minPt
    //-------------------------------------------------------------------------------------

    if ((pf_it->pt())>5.) {
      pfCandsP4->push_back(pf_it->p4());
      pfCandsParticleId->push_back(pf_it->particleId());
    }


    if( pf_it->pt() < minPt_ ) continue;


    //-------------------------------------------------------------------------------------
    // store pt and charge of PFCandidate
    //-------------------------------------------------------------------------------------

    pfCandsPt->push_back(pf_it->pt());
    pfCandsChrg->push_back(pf_it->charge());

    //-------------------------------------------------------------------------------------
    // if there's no good vertex in the event, we can't calculate anything so store 999999
    //-------------------------------------------------------------------------------------
    
    if ( firstGoodVertex==vertices->end() ) {
      pfCandsTrkIso->push_back(999);
      pfCandsDzPV->push_back(999);
      continue;
    }



    //-------------------------------------------------------
    // require PFCandidate is charged, otherwise store 999 
    //-------------------------------------------------------

    if( pf_it->charge() != 0 ){

      //----------------------------------------------------------------------------
      // now loop over other PFCandidates in the event to calculate trackIsolation
      //----------------------------------------------------------------------------

      float trkiso = 0.0;

      for( PFCandidateCollection::const_iterator pf_other = pfCandidates->begin(); pf_other != pfCandidates->end(); pf_other++ ) {

	// don't count the PFCandidate in its own isolation sum
	if( pf_it == pf_other       ) continue;

	// require the PFCandidate to be charged
	if( pf_other->charge() == 0 ) continue;

        // cut on dR between the PFCandidates
        float dR = deltaR(pf_it->eta(), pf_it->phi(), pf_other->eta(), pf_other->phi());
        if( dR > dR_ ) continue;

	// cut on the PFCandidate dz
	float dz_other = 100;

	if ( pf_other->trackRef().isNonnull()) {
	  dz_other = pf_other->trackRef()->dz( firstGoodVertex->position() );
	}

	if( fabs(dz_other) > dzcut_ ) continue;

	trkiso += pf_other->pt();
      }

      // calculate the dz of this candidate
      float dz_it = 100; //
      
      if ( pf_it->trackRef().isNonnull()) { dz_it = pf_it->trackRef()->dz( firstGoodVertex->position() ); }
      
      // store trkiso and dz values
      pfCandsTrkIso->push_back(trkiso);
      pfCandsDzPV->push_back(dz_it);
      
    }

    else {
      //neutral particle, set trkiso and dzpv to 9999
      pfCandsTrkIso->push_back(9999);
      pfCandsDzPV->push_back(9999);
    }
    
  }

  event.put(pfCandsP4,"PFCandsP4");
  event.put(pfCandsParticleId,"PFCandsParticleId");
  event.put(pfCandsTrkIso,"PFCandsTrkIso");
  event.put(pfCandsDzPV,"PFCandsDzPV");
  event.put(pfCandsPt,"PFCandsPt");
  event.put(pfCandsChrg,"PFCandsChrg");


  }


};

#endif
