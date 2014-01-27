#include "DataFormats/METReco/interface/SigInputObj.h"
// -*- C++ -*-
//
// Package:    METAlgorithms
// Class:      SigInputObj
// 
/**\class METSignificance SigInputObj.cc DataFormats/METReco/src/SigInputObj.cc

 Description: <one line class summary>

 Implementation:
     <Notes on implementation>
*/
//
// Original Author:  Kyle Story, Freya Blekman (Cornell University)
//         Created:  Fri Apr 18 11:58:33 CEST 2008
// $Id: SigInputObj.cc,v 1.2 2012/09/17 17:55:18 veelken Exp $
//
//

//=== Constructors ===============================//
metsig::SigInputObj::SigInputObj(const std::string& m_type, double m_energy, double m_phi, double m_sigma_e, double m_sigma_tan) 
  : type(m_type),
    energy(m_energy),
    phi(m_phi),
    sigma_e(m_sigma_e),
    sigma_tan(m_sigma_tan)
{}
//================================================//

//=== Methods ====================================//
// none yet...
//================================================//

