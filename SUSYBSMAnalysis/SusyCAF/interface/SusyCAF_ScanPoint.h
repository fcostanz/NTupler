#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "boost/foreach.hpp"
#include <map>
#include <string>

class ScanPoint
{
 public:
  typedef std::map<const std::string, double> PointMap;
  ScanPoint(const std::vector<std::string>& names, const edm::ParameterSet& cfg)
    {
      BOOST_FOREACH(const std::string name, names){
	point_[name] = cfg.getParameter<double>(name);
      }
    }
  ScanPoint(const PointMap& point):point_(point){}
    
    inline bool operator==(const ScanPoint& rhs) const {return rhs.equalTo(point_); } 
    inline bool operator!=(const ScanPoint& rhs) const {return ! ((*this) == rhs);} 
    inline bool operator< (const ScanPoint& rhs) const {return rhs.notSmallerThan(point_);}
    inline bool operator<=(const ScanPoint& rhs) const {return !( (*this) > rhs );} 
    inline bool operator>=(const ScanPoint& rhs) const {return !( (*this) < rhs);}
    inline bool operator> (const ScanPoint& rhs) const {return rhs.operator<(*this);} 

    // could add rounding circumvention here a la fabs(a -a_)/a < 1e-6
    bool equalTo(const PointMap& point) const
    {
      bool result = true;
      BOOST_FOREACH(const PointMap::value_type param, point)
	result &= (param.second == point_.find(param.first)->second);
      return result;
    }

    bool notSmallerThan(const PointMap& point) const
    {
      bool smaller = false;
      BOOST_FOREACH(const PointMap::value_type param, point_)
	smaller |= (!smaller && param.second < point.find(param.first)->second);
      return this->equalTo(point) || (!smaller);	
    }

 private:
    PointMap point_;
};
