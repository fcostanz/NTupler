import FWCore.ParameterSet.Config as cms

#______________________ Event-Selection _____________________________

from HLTrigger.HLTfilters.hltHighLevel_cfi import hltHighLevel
hltFilter = hltHighLevel.clone(
    andOr = True, ## choose logical OR between Triggerbits
    HLTPaths = ['HLT_ZeroBias'],
    throw = False # tolerate triggers stated above, but not available
    )

#______________________ Object-Selection ____________________________

from PhysicsTools.PatAlgos.selectionLayer1.jetSelector_cfi import selectedPatJets
from PhysicsTools.PatAlgos.selectionLayer1.electronSelector_cfi import selectedPatElectrons
from PhysicsTools.PatAlgos.selectionLayer1.muonSelector_cfi import selectedPatMuons
from PhysicsTools.PatAlgos.selectionLayer1.tauSelector_cfi import selectedPatTaus

patJetSelector = selectedPatJets.clone( filter = cms.bool(False), src = "", cut = "")
patElectronSelector = selectedPatElectrons.clone( filter = cms.bool(False), src = "", cut = "")
patMuonSelector = selectedPatMuons.clone( filter = cms.bool(False), src = "", cut = "")
patTauSelector = selectedPatTaus.clone( filter = cms.bool(False), src = "", cut = "")

candStringSelector = cms.EDFilter("CandViewSelector",
    filter = cms.bool(False),
    src = cms.InputTag(""),
    cut = cms.string("")
)


