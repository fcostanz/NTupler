#include "SUSYBSMAnalysis/SusyCAF/interface/SusyCAF_BeamSpot.h"
#include "DataFormats/BeamSpot/interface/BeamSpot.h"
#include "FWCore/Framework/interface/Event.h"
#include "DataFormats/Math/interface/Vector3D.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

SusyCAF_BeamSpot::SusyCAF_BeamSpot(const edm::ParameterSet& iConfig) 
  : inputTag(iConfig.getParameter<edm::InputTag>("InputTag"))
  , prefix  (iConfig.getParameter<std::string>  ("Prefix"  ))
  , suffix  (iConfig.getParameter<std::string>  ("Suffix"  ))
{
  produces <math::XYZPoint>  (prefix + "Position"      + suffix);
  produces <math::XYZVector> (prefix + "PositionError" + suffix);
  produces <math::XYZVector> (prefix + "Sigma"         + suffix);
  produces <math::XYZVector> (prefix + "SigmaError"    + suffix);
  produces <double>          (prefix + "DxDz"          + suffix);
  produces <double>          (prefix + "DyDz"          + suffix);
  produces <double>          (prefix + "DxDzError"     + suffix);
  produces <double>          (prefix + "DyDzError"     + suffix);
}

void SusyCAF_BeamSpot::
produce(edm::Event& event, const edm::EventSetup& ) 
{
  edm::Handle<reco::BeamSpot> bs;  event.getByLabel(inputTag, bs);

  event.put( std::auto_ptr<math::XYZPoint>( new math::XYZPoint( bs->position() ) ), prefix+"Position"+suffix);
  event.put( std::auto_ptr<math::XYZVector>( new math::XYZVector(bs->x0Error(), bs->y0Error(), bs->z0Error())), prefix+"PositionError"+suffix);
  event.put( std::auto_ptr<math::XYZVector>( new math::XYZVector(bs->BeamWidthX(), bs->BeamWidthY(), bs->sigmaZ())), prefix+"Sigma"+suffix);
  event.put( std::auto_ptr<math::XYZVector>( new math::XYZVector(bs->BeamWidthXError(), bs->BeamWidthYError(), bs->sigmaZ0Error())), prefix+"SigmaError"+suffix);
  event.put( std::auto_ptr<double>( new double(bs->dxdz())), prefix+"DxDz"+suffix);
  event.put( std::auto_ptr<double>( new double(bs->dydz())), prefix+"DyDz"+suffix);
  event.put( std::auto_ptr<double>( new double(bs->dxdzError())), prefix+"DxDzError"+suffix);
  event.put( std::auto_ptr<double>( new double(bs->dydzError())), prefix+"DyDzError"+suffix);

}
