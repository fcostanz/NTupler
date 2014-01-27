import FWCore.ParameterSet.Config as cms

def patjets( calo = True, input = '', genInput = '', prefix = '', jecRecord = '', jetid = True, pf = False, jpt = False) :
    switches_on = int(calo) + int(pf) + int(jpt)
    if switches_on != 1 : raise NameError("Only one of Calo:PF:JPT!!")
    return cms.EDProducer( "SusyCAF_PatJet",
                           InputTag = cms.InputTag(input),
                           AllJets = cms.InputTag(input),
                           GenInputTag = cms.InputTag(genInput),
                           Prefix = cms.string(prefix),
                           Suffix = cms.string('Pat'),
                           JecRecord = cms.string(jecRecord),
                           Calo = cms.bool(calo),
                           JPT = cms.bool(jpt),
                           PF = cms.bool(pf),
                           JetID = cms.bool(jetid),
                           GenInfo = cms.bool(False),
                           MPT = cms.bool(True),
                           PrimaryVertexTag = cms.InputTag('offlinePrimaryVertices'),
                           MaxD0Trk = cms.double(5), # wide enough to include most tracks from b-jets?
                           TrackQualities = cms.vint32(2), # highPurity (see reco::TrackBase::TrackQuality)
                           JetCorrections = cms.vstring()
                           )

def jets( calo = True, input = '', genInput = '', prefix = '', jecRecord = '', pf = False, jpt = False) :
    switches_on = int(calo) + int(pf) + int(jpt)
    if switches_on != 1 : raise NameError("Only one of Calo:PF:JPT!!")
    jet_instance = ""
    if calo : jet_instance = "Calo"
    elif jpt : jet_instance = "JPT"
    elif pf : jet_instance = "PF"
    return cms.EDProducer( "SusyCAF_"+jet_instance+"Jet",
                           InputTag = cms.InputTag(input),
                           AllJets = cms.InputTag(input),
                           GenInputTag = cms.InputTag(genInput),
                           Prefix = cms.string(prefix),
                           Suffix = cms.string(jet_instance),
                           JecRecord = cms.string(jecRecord),
                           Calo = cms.bool(calo),
                           JPT = cms.bool(jpt),
                           PF = cms.bool(pf),
                           MPT = cms.bool(False),
                           JetID = cms.bool(False),
                           GenInfo = cms.bool(False),
                           )

# Without SusyPAT
susycafic5calojetreco = jets( input = 'iterativeCone5CaloJets',       prefix = 'ic5Jet',    jecRecord = 'IC5Calo', genInput = 'iterativeCone5GenJets')
susycafak5calojetreco = jets( input = 'ak5CaloJets',                  prefix = 'ak5Jet',    jecRecord = 'AK5Calo', genInput = 'ak5GenJets'  )
susycafak7calojetreco = jets( input = 'ak7CaloJets',                  prefix = 'ak7Jet',    jecRecord = 'AK7Calo', genInput = 'ak7GenJets' )

susycafak5jptjetreco  = jets( input = 'JetPlusTrackZSPCorJetAntiKt5', prefix = 'ak5JetJPT', jecRecord = 'AK5JPT', genInput = 'ak5GenJets',            calo = False, jpt = True )
susycafak7jptjetreco  = jets( input = 'JetPlusTrackZSPCorJetAntiKt7', prefix = 'ak7JetJPT', jecRecord = 'AK7JPT', genInput = 'ak7GenJets',            calo = False, jpt = True )

susycafic5pfjetreco   = jets( input = 'iterativeCone5PFJets',         prefix = 'ic5Jet',    jecRecord = 'IC5PF', genInput = 'iterativeCone5GenJets', calo = False,  pf = True )
susycafak5pfjetreco   = jets( input = 'ak5PFJets',                    prefix = 'ak5Jet',    jecRecord = 'AK5PF', genInput = 'ak5GenJets',            calo = False,  pf = True )
susycafak7pfjetreco   = jets( input = 'ak7PFJets',                    prefix = 'ak7Jet',    jecRecord = 'AK7PF', genInput = 'ak7GenJets',            calo = False,  pf = True )

# With SusyPAT
susycafic5calojet   = patjets( input = 'cleanPatJetsIC5Calo',  prefix = 'ic5Jet',       jecRecord = 'IC5Calo', genInput = 'iterativeCone5GenJets' )
susycafak5calojet   = patjets( input = 'cleanPatJetsAK5Calo',  prefix = 'ak5Jet',       jecRecord = 'AK5Calo', genInput = 'ak5GenJets' )
susycafak7calojet   = patjets( input = 'cleanPatJetsAK7Calo',  prefix = 'ak7Jet',       jecRecord = 'AK7Calo', genInput = 'ak7GenJets' )
                                                               
susycafak5jptjet    = patjets( input = 'cleanPatJetsAK5JPT',   prefix = 'ak5JetJPT',    jecRecord = 'AK5JPT', genInput = 'ak5GenJets',                calo = False, jpt = True )
susycafak7jptjet    = patjets( input = 'cleanPatJetsAK7JPT',   prefix = 'ak7JetJPT',    jecRecord = 'AK7JPT', genInput = 'ak7GenJets',                calo = False, jpt = True )

susycafak5pfjet     = patjets( input = 'cleanPatJetsAK5PF',    prefix = 'ak5JetPF',     jecRecord = 'AK5PF', genInput = 'ak5GenJets', jetid = False, calo = False,  pf = True )
susycafak7pfjet     = patjets( input = 'cleanPatJetsAK7PF',    prefix = 'ak7JetPF',     jecRecord = 'AK7PF', genInput = 'ak7GenJets', jetid = False, calo = False,  pf = True )

susycafak5pf2patjet = patjets( input = 'selectedPatJetsPF',    prefix = 'ak5JetPF2PAT', jecRecord = 'AK5PF', genInput = 'ak5GenJets', jetid = False, calo = False,  pf = True )
susycafak7pf2patjet = patjets( input = 'selectedPatJetsAK7PF', prefix = 'ak7JetPF2PAT', jecRecord = 'AK7PF', genInput = 'ak7GenJets', jetid = False, calo = False,  pf = True )

#matched collections
susycafic5calojetMatched   = susycafic5calojet.clone(   GenInfo = True )
susycafak5calojetMatched   = susycafak5calojet.clone(   GenInfo = True )
susycafak7calojetMatched   = susycafak7calojet.clone(   GenInfo = True )

susycafak5jptjetMatched    = susycafak5jptjet.clone(    GenInfo = True )
susycafak7jptjetMatched    = susycafak7jptjet.clone(    GenInfo = True )

susycafak5pfjetMatched     = susycafak5pfjet.clone(     GenInfo = True )
susycafak7pfjetMatched     = susycafak7pfjet.clone(     GenInfo = True )

susycafak5pf2patjetMatched = susycafak5pf2patjet.clone( GenInfo = True )
susycafak7pf2patjetMatched = susycafak7pf2patjet.clone( GenInfo = True )
