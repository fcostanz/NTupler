#include "SUSYBSMAnalysis/SusyCAF/interface/ProductReducer.h"
#include "SUSYBSMAnalysis/SusyCAF/interface/fTypes.h"

#include "FWCore/Framework/interface/ConstProductRegistry.h" 
#include "FWCore/Framework/interface/GroupSelector.h"
#include "FWCore/Framework/interface/GroupSelectorRules.h"
#include "DataFormats/Provenance/interface/Selections.h"

#include "boost/foreach.hpp"

void ProductReducer::
produce(edm::Event& iEvent, const edm::EventSetup& iSetup) {
  BOOST_FOREACH( Reducer* reducer, reducers)
    reducer->reduce(iEvent);
}

template<class A, class B> 
void ProductReducer::registerReducer(edm::BranchDescription const * desc) {
  const std::string name = desc->productInstanceName()==""?desc->moduleLabel():desc->productInstanceName();
  produces<B>(name);
  typedef typename boost::mpl::if_<my_black_magic::has_iterator<A>, 
    VectorTypedReducer<A,B>, 
    UnitTypedReducer<A,B> >::type   tr_type;
  reducers.push_back(new tr_type(desc));
}

template <class A, class B>
void ProductReducer::TypedReducer<A,B>::
reduce(edm::Event& iEvent) {
  edm::Handle<A> handle_;
  iEvent.getByLabel(ml, pin, handle_);
  iEvent.put( convert(*handle_), name);
}

template <class A, class B>
std::auto_ptr<B> ProductReducer::UnitTypedReducer<A,B>::
static_convert(const A& a) { return std::auto_ptr<B>(new B(a));}

template <class A, class B>
std::auto_ptr<B> ProductReducer::VectorTypedReducer<A,B>::
convert(const A& a) {
  std::auto_ptr<B> b(new B());
  for( typename A::const_iterator it(a.begin()), end(a.end()); it!=end; ++it) {
    b->push_back(*((UnitTypedReducer<typename A::value_type, typename B::value_type>::static_convert(*it)).release()));
  }
  return b;
}

template <>
std::auto_ptr<fTypes::fPolarLorentzV> ProductReducer::UnitTypedReducer<fTypes::dXYZLorentzV,fTypes::fPolarLorentzV>::
static_convert(const fTypes::dXYZLorentzV& a) {
  return std::auto_ptr<fTypes::fPolarLorentzV>(new fTypes::fPolarLorentzV(a.Pt(), a.Eta(), a.Phi(), a.M()));
}

template<>
std::auto_ptr<fTypes::fPolarLorentzV> ProductReducer::UnitTypedReducer<fTypes::dPolarLorentzV,fTypes::fPolarLorentzV>::
static_convert(const fTypes::dPolarLorentzV& a) {
  return std::auto_ptr<fTypes::fPolarLorentzV>(new fTypes::fPolarLorentzV(a.Pt(), a.Eta(), a.Phi(), a.M()));
}

template<>
std::auto_ptr<fTypes::fPoint> ProductReducer::UnitTypedReducer<fTypes::dPoint,fTypes::fPoint>::
static_convert(const fTypes::dPoint& a) {
  return std::auto_ptr<fTypes::fPoint>(new fTypes::fPoint(a.X(),a.Y(),a.Z()));
}

template<>
std::auto_ptr<fTypes::fVector> ProductReducer::UnitTypedReducer<fTypes::dVector,fTypes::fVector>::
static_convert(const fTypes::dVector& a) {
  return std::auto_ptr<fTypes::fVector>(new fTypes::fVector(a.X(),a.Y(),a.Z()));
}
  
ProductReducer::ProductReducer(const edm::ParameterSet& conf) : pset(conf) {

  edm::Service<edm::ConstProductRegistry> reg;
  edm::Selections allBranches = reg->allBranchDescriptions();
  edm::GroupSelector groupSelector_;
  groupSelector_.initialize(edm::GroupSelectorRules(pset, "selectionCommands", "ProductReducer"), allBranches);
  
  std::map<std::string,fTypes::LEAFTYPE> dict = fTypes::dict();
  std::set<std::string> branchnames;
  BOOST_FOREACH( const edm::Selections::value_type& selection, allBranches) {
    if(groupSelector_.selected(*selection)) {

      //Check for duplicate branch names
      const std::string name = selection->productInstanceName()==""? selection->moduleLabel() : selection->productInstanceName();
      if (branchnames.find(name) != branchnames.end() )
	throw edm::Exception(edm::errors::Configuration)
	  << "More than one branch named: " << name << std::endl
	  << "Exception thrown from ProductReducer::beginJob" << std::endl;
      else 
	branchnames.insert(name);
      
      switch(dict.find(selection->friendlyClassName())->second) {
#define EXPAND(enumT,T1,T2) case fTypes::enumT: registerReducer<T1,T2 >(selection); break
	EXPAND(LONG       , long,   int  );
	EXPAND(DOUBLE     , double, float);
	EXPAND(POINTD     , fTypes::dPoint, fTypes::fPoint );
	EXPAND(VECTORD    , fTypes::dVector,fTypes::fVector);
	EXPAND(LORENTZVD  , fTypes::dXYZLorentzV,  fTypes::fPolarLorentzV );
	EXPAND(LORENTZVP  , fTypes::dPolarLorentzV,fTypes::fPolarLorentzV);

	EXPAND(BOOL_V     , std::vector<bool>,   std::vector<int>  );
	EXPAND(DOUBLE_V   , std::vector<double>, std::vector<float>);
	EXPAND(POINTD_V   , std::vector<fTypes::dPoint>,  std::vector<fTypes::fPoint>  );
	EXPAND(VECTORD_V  , std::vector<fTypes::dVector>, std::vector<fTypes::fVector> );
	EXPAND(LORENTZVD_V, std::vector<fTypes::dXYZLorentzV>,   std::vector<fTypes::fPolarLorentzV> );
	EXPAND(LORENTZVP_V, std::vector<fTypes::dPolarLorentzV>, std::vector<fTypes::fPolarLorentzV> );
#undef EXPAND
      case fTypes::BOOL: case fTypes::SHORT: case fTypes::INT: case fTypes::FLOAT: case fTypes::STRING : 
      case fTypes::U_LONG : case fTypes::U_SHORT: case fTypes::U_INT: 
      case fTypes::POINTF: case fTypes::VECTORF: case fTypes::LORENTZVF  : 
      case fTypes::SHORT_V: case fTypes::INT_V: case fTypes::FLOAT_V:
      case fTypes::U_INT_V: case fTypes::U_LONG_V: case fTypes::U_SHORT_V:
      case fTypes::POINTF_V: case fTypes::VECTORF_V: case fTypes::LORENTZVF_V: 
      case fTypes::STRING_BOOL_M: case fTypes::STRING_INT_M: case fTypes::STRING_STRING_M:
      default : 
	throw edm::Exception(edm::errors::Configuration) << "ProductReducer does not handle leaves of type " << selection->className();
      }
    }
  }
}
