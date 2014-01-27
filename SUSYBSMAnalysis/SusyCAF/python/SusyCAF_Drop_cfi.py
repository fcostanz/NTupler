def reduce() :
    return ['double_susycaf*_*_*',
            'doubles_susycaf*_*_*',
            'doubleROOTMathPxPyPzE4DROOTMathLorentzVector*_susycaf*_*_*',
            'doubleROOTMathPtEtaPhiE4DROOTMathLorentzVector*_susycaf*_*_*',
            'doubleROOTMathCartesian3DROOTMathDefaultCoordinateSystemTagROOTMathPositionVector3D*_susycaf*_*_*',
            'doubleROOTMathCartesian3DROOTMathDefaultCoordinateSystemTagROOTMathDisplacementVector3D*_susycaf*_*_*'
            ]

def keep() :
    '''These products will be kept regardless of what is written in drop()'''
    return [
        '*_susycaf*_*PFJetID*_*',                ## JetID that we use
        '*_susycaf*_*JetIDminimal*_*',           #
        '*_susycaf*_*JetIDloose*_*',             #
        '*_susycaf*_*JetIDtight*_*',             #
        ]

def drop(more = False) :
    return [
        '*_susycaf*_CTECALEsumcalo*_*',
        '*_susycaf*_CTHCALEsumcalo*_*',
        '*_susycaf*_hltL1Seeds*_*',
        ] + [] if not more else [
        '*_susycaf*_*DroppedSumP4*_*',   ## dropped jet summary
        '*_susycaf*_*DroppedSumPt*_*',   #
        '*_susycaf*_*DroppedSumEt*_*',   #
        '*_susycaf*_*FchargedHad*_*',    ## PFJet special vars
        '*_susycaf*_*FneutralHad*_*',    #
        '*_susycaf*_*FchargedEm*_*',     #
        '*_susycaf*_*FneutralEm*_*',     #
        '*_susycaf*_*FchargedMu*_*',     #
        '*_susycaf*_*EchargedHad*_*',    #
        '*_susycaf*_*EneutralHad*_*',    #
        '*_susycaf*_*EchargedEM*_*',     #
        '*_susycaf*_*EneutralEM*_*',     #
        '*_susycaf*_*Ephoton*_*',        #
        '*_susycaf*_*Eelectron*_*',      #
        '*_susycaf*_*Emuon*_*',          #
        '*_susycaf*_*EHFHad*_*',         #
        '*_susycaf*_*EHFEM*_*',          #
        '*_susycaf*_*Ncharged*_*',       #
        '*_susycaf*_*Nneutral*_*',       #
        '*_susycaf*_*Nmuon*_*',          #
        '*_susycaf*_*NchargedHad*_*',    #
        '*_susycaf*_*NneutralHad*_*',    #
        '*_susycaf*_*Nphoton*_*',        #
        '*_susycaf*_*Nelectron*_*',      #
        '*_susycaf*_*NHFHad*_*',         #
        '*_susycaf*_*NHFEM*_*',          #
        '*_susycaf*_*EmEnergyFraction*_*',       ## CaloJet special vars
        '*_susycaf*_*EnergyFractionHadronic*_*', #
        '*_susycaf*_*TowersArea*_*',             #
        '*_susycaf*_*MaxEInEmTowers*_*',         #
        '*_susycaf*_*MaxEInHadTowers*_*',        #
        '*_susycaf*_*HadEnergyInHB*_*',          #
        '*_susycaf*_*HadEnergyInHE*_*',          #
        '*_susycaf*_*HadEnergyInHO*_*',          #
        '*_susycaf*_*HadEnergyInHF*_*',          #
        '*_susycaf*_*EmEnergyInEB*_*',           #
        '*_susycaf*_*EmEnergyInEE*_*',           #
        '*_susycaf*_*EmEnergyInHF*_*',           #
        '*_susycaf*_*N60Towers*_*',              #
        '*_susycaf*_*N90Towers*_*',              #
        '*_susycaf*_*JetID*_*',            ## energy fractions and n90 hits, etc.
        '*_susycaf*_*Soft*BJetTags*_*',    ## soft b-tags (5 vector vars / jetcollection )
        ]
