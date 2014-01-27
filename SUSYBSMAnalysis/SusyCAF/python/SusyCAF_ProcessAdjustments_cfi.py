import FWCore.ParameterSet.Config as cms

def loadAndConfigureHcalSeverityLevelProducer(process, isData) :
    process.load("RecoLocalCalo.HcalRecAlgos.hcalRecAlgoESProd_cfi")
    if isData :
        process.hcalRecAlgos.SeverityLevels[3].RecHitFlags.remove("HFDigiTime")
        process.hcalRecAlgos.SeverityLevels[4].RecHitFlags.append("HFDigiTime")

def loadAndConfigureEcalSeverityLevelProducer(process) :
    process.load("RecoLocalCalo.EcalRecAlgos.EcalSeverityLevelESProducer_cfi")

def messageLogger(process,quiet) :
    if quiet :
        process.options = cms.untracked.PSet( wantSummary = cms.untracked.bool(False) )
        for item in [process.MessageLogger.cerr.getParameter(name) for name in process.MessageLogger.cerr.parameterNames_()] :
            if type(item) is cms.PSet :
                item.reportEvery = cms.untracked.int32(1000)
    else :
        process.MessageLogger.categories.append('PATSummaryTables')
        process.MessageLogger.cerr.PATSummaryTables = cms.untracked.PSet( limit=cms.untracked.int32(-1), reportEvery=cms.untracked.int32(1))



def __patOutput__(process,fileName) :
    if fileName == ".root" and hasattr(process,"out"): # remove outpath 
        del process.out
        del process.outpath
    else :
        process.out.fileName = fileName
        process.out.splitLevel = cms.untracked.int32(99)
        process.out.overrideInputFileSplitLevels = cms.untracked.bool(True)
        process.out.dropMetaData = cms.untracked.string('DROPPED')   # Get rid of metadata related to dropped collections
        
        from PhysicsTools.Configuration.SUSY_pattuple_cff import getSUSY_pattuple_outputCommands
        SUSY_pattuple_outputCommands = getSUSY_pattuple_outputCommands( process )
        process.out.outputCommands = cms.untracked.vstring('drop *', *SUSY_pattuple_outputCommands )
        process.out.outputCommands.append('keep *_HBHENoiseFilterResultProducer_*_*')

def patAllElectronsPF(process) :
    from PhysicsTools.PatAlgos.producersLayer1.electronProducer_cfi import patElectrons
    process.patAllElectronsPF = patElectrons.clone( # override defaults
        pfElectronSource = "pfAllElectronsPF",
        useParticleFlow = True,
        genParticleMatch = "",
        addGenMatch = False,
        embedGenMatch = False,
        embedTrack = True
        )
    return process.patAllElectronsPF

def patAllMuonsPF(process) :
    from PhysicsTools.PatAlgos.producersLayer1.muonProducer_cfi import patMuons
    process.patAllMuonsPF = patMuons.clone( #override defaults
        pfMuonSource = "pfAllMuonsPF",
        useParticleFlow = True,
        addGenMatch = False,
        genParticleMatch = "",
        embedTrack = True,
        embedGenMatch = False,
        embedPickyMuon = False,
        embedTpfmsMuon = False,
        embedHighLevelSelection = False,
        embedCaloMETMuonCorrs = False,
        embedTcMETMuonCorrs = False
        )
    return process.patAllMuonsPF

def susyPat(process,options) :
    if not options.patify:
        if not options.quiet : print "WARNING: selection (slimming) not applied for options patify(False)."
        __patOutput__(process, options.secondaryOutput)
        return cms.Path()
    else:
        from PhysicsTools.Configuration.SUSY_pattuple_cff import addDefaultSUSYPAT
        jetAlgoList = filter(lambda s: s.lower() in options.jetCollections, ['AK7Calo','AK5PF','AK7PF'])
        addDefaultSUSYPAT(process, mcInfo = not options.isData, HLTMenu = 'HLT', jetMetCorrections = options.jetCorrections, theJetNames = jetAlgoList)
        for algo in ['']+jetAlgoList :
            setattr( getattr( process, 'patJetGenJetMatch'+algo), 'maxDeltaR', cms.double(0.7 if '7' in algo else 0.5) )
        __patOutput__(process, options.secondaryOutput)
        return cms.Path(process.susyPatDefaultSequence + patAllElectronsPF(process) + patAllMuonsPF(process))

def addHbheNoiseFilterResult(process, options) :
    process.load('CommonTools/RecoAlgos/HBHENoiseFilterResultProducer_cfi')
    process.HBHENoiseFilterResultProducerNoIso = process.HBHENoiseFilterResultProducer.clone( minIsolatedNoiseSumE = 999999.0,
                                                                                              minNumIsolatedNoiseChannels = 999999,
                                                                                              minIsolatedNoiseSumEt = 999999.0  )
    process.hcalNoiseSummaryExists = cms.EDFilter('SusyCAF_HcalNoiseSummaryExists')
    return cms.Path(process.hcalNoiseSummaryExists + process.HBHENoiseFilterResultProducer + process.HBHENoiseFilterResultProducerNoIso )

def addMetFilterFlags(process, options) :
    # https://twiki.cern.ch/twiki/bin/view/CMS/MissingETOptionalFilters
    from RecoMET.METFilters.trackingFailureFilter_cfi import trackingFailureFilter
    from RecoMET.METFilters.hcalLaserEventFilter_cfi import hcalLaserEventFilter
    from RecoMET.METFilters.inconsistentMuonPFCandidateFilter_cfi import inconsistentMuonPFCandidateFilter
    from RecoMET.METFilters.greedyMuonPFCandidateFilter_cfi import greedyMuonPFCandidateFilter
    from RecoMET.METFilters.EcalDeadCellTriggerPrimitiveFilter_cfi import EcalDeadCellTriggerPrimitiveFilter
    from RecoMET.METFilters.EcalDeadCellBoundaryEnergyFilter_cfi import EcalDeadCellBoundaryEnergyFilter
    from RecoMET.METFilters.eeBadScFilter_cfi import eeBadScFilter

    process.trackingFailureFilterFlag = trackingFailureFilter.clone(taggingMode = True)#, quiet = True)
    process.hcalLaserEventFilterFlag = hcalLaserEventFilter.clone(taggingMode = True,
                                                                  vetoByRunEventNumber=False,
                                                                  vetoByHBHEOccupancy=True)
    process.greedyMuonPFCandidateFilterFlag = greedyMuonPFCandidateFilter.clone(taggingMode = True)
    process.inconsistentMuonPFCandidateFilterFlag = inconsistentMuonPFCandidateFilter.clone(taggingMode = True)
    process.ecalDeadCellTPFilterFlag = EcalDeadCellTriggerPrimitiveFilter.clone(taggingMode = True)
    process.ecalDeadCellBEFilterFlag = EcalDeadCellBoundaryEnergyFilter.clone(taggingMode = True,
                                                                              cutBoundEnergyDeadCellsEB = 10.0,
                                                                              cutBoundEnergyDeadCellsEE = 10.0,
                                                                              cutBoundEnergyGapEB = 100.0,
                                                                              cutBoundEnergyGapEE = 100.0,
                                                                              enableGap = False,
                                                                              limitDeadCellToChannelStatusEB = cms.vint32(12,14),
                                                                              limitDeadCellToChannelStatusEE = cms.vint32(12,14))
    process.eeBadScFilterFlag = eeBadScFilter.clone(taggingMode = True)

    process.goodVertices = cms.EDFilter("VertexSelector",
                                        filter = cms.bool(False),
                                        src = cms.InputTag("offlinePrimaryVertices"),
                                        cut = cms.string("!isFake && ndof > 4 && abs(z) <= 24 && position.rho < 2"))

    return cms.Path( process.goodVertices *
                     process.trackingFailureFilterFlag *
                     process.hcalLaserEventFilterFlag *
                     process.greedyMuonPFCandidateFilterFlag *
                     process.inconsistentMuonPFCandidateFilterFlag *
                     process.ecalDeadCellTPFilterFlag *
                     process.eeBadScFilterFlag
                     # * process.ecalDeadCellBEFilterFlag # product not found : EcalRecHitsEB
                     )

def lumiTree(process) :
    process.load('SUSYBSMAnalysis.SusyCAF.SusyCAF_LumiTreeMaker_cfi')
    return cms.Path(process.lumiTree)

def rho25(process) :
    #https://twiki.cern.ch/twiki/bin/view/CMS/EgammaEARhoCorrection
    #(also https://twiki.cern.ch/twiki/bin/view/CMS/Vgamma2011PhotonID)
    from RecoJets.JetProducers.kt4PFJets_cfi import kt4PFJets
    process.kt6PFJetsForIsolation = kt4PFJets.clone( rParam = 0.6, doRhoFastjet = True )
    process.kt6PFJetsForIsolation.Rho_EtaMax = cms.double(2.5)
    return cms.Path( process.kt6PFJetsForIsolation )

def typeIMet(process,options) :
    if options.doTypeIMetReco :
        #https://twiki.cern.ch/twiki/bin/view/CMSPublic/WorkBookMetAnalysis
        process.load("JetMETCorrections.Type1MET.pfMETCorrections_cff")
        process.load("JetMETCorrections.Configuration.JetCorrectionServices_cff")
        process.load("JetMETCorrections.Type1MET.caloMETCorrections_cff")

        if options.isData :
            process.pfJetMETcorr.jetCorrLabel = cms.string("ak5PFL1FastL2L3Residual")
            process.caloJetMETcorr.jetCorrLabel = cms.string("ak5CaloL1FastL2L3Residual")
 
        return cms.Path(process.producePFMETCorrections + process.produceCaloMETCorrections)
    else :
        return cms.Path()

def typeIMetPat(process,options) :
    #RA4 links
    #MET phi link: https://twiki.cern.ch/twiki/bin/view/CMSPublic/WorkBookMetAnalysis#MET_x_y_Shift_Correction_for_mod
    if options.doTypeIMetPat :
        process.load("JetMETCorrections.Type1MET.pfMETCorrections_cff")
        process.load("JetMETCorrections.Configuration.JetCorrectionServices_cff")
        process.load("JetMETCorrections.Type1MET.caloMETCorrections_cff")
        if options.doPfMetPhiCorrections :
            process.load("JetMETCorrections.Type1MET.pfMETsysShiftCorrections_cfi")
        if not options.isData:
            process.pfJetMETcorr.jetCorrLabel = "ak5PFL1FastL2L3"
            process.caloJetMETcorr.jetCorrLabel = cms.string("ak5CaloL1FastL2L3")
            if options.doPfMetPhiCorrections :
                process.pfMEtSysShiftCorr.parameter = process.pfMEtSysShiftCorrParameters_2012runAvsNvtx_mc
        else:
            process.pfJetMETcorr.jetCorrLabel = "ak5PFL1FastL2L3Residual"
            process.caloJetMETcorr.jetCorrLabel = cms.string("ak5CaloL1FastL2L3Residual")
            if options.doPfMetPhiCorrections :
                process.pfMEtSysShiftCorr.parameter = process.pfMEtSysShiftCorrParameters_2012runAvsNvtx_data
        process.patPFMETs = process.patMETs.clone(
            metSource = cms.InputTag('pfMet'),
            addMuonCorrections = cms.bool(False),
            #genMETSource = cms.InputTag('genMetTrue'),
            #adAdGenMET = cms.bool(True)
                )
        process.pfType1CorrectedMet.applyType0Corrections = cms.bool(False)
        
        args = [cms.InputTag('pfJetMETcorr', 'type1')]
        if options.doPfMetPhiCorrections :
            args.append(cms.InputTag('pfMEtSysShiftCorr'))
            process.pfType1p2CorrectedMet.srcType1Corrections = cms.VInputTag(
                cms.InputTag('pfJetMETcorr','type1'),
                cms.InputTag('pfMEtSysShiftCorr')
                )

        process.pfType1CorrectedMet.srcType1Corrections = cms.VInputTag(*tuple(args))

        process.patPFMETsTypeIcorrected = process.patPFMETs.clone(
            metSource = cms.InputTag('pfType1CorrectedMet'),
            )

        process.empty2 = cms.Sequence()
        modules  = [process.pfMEtSysShiftCorrSequence] if options.doPfMetPhiCorrections else []
        modules += [process.producePFMETCorrections,
                    process.produceCaloMETCorrections,
                    process.patPFMETsTypeIcorrected]
        return cms.Path(sum(modules, process.empty2))
    else :
        return cms.Path()


def tauReco(process,options) :
    if options.doTauReco :
        #https://hypernews.cern.ch/HyperNews/CMS/get/physTools/2710/1/1/2/1.html
        process.load("RecoTauTag/Configuration/RecoPFTauTag_cff")
        return cms.Path(process.PFTau)
    else :
        return cms.Path()

def photonPFIso(process,options) :
    if options.doPhotonPFIso :
        #https://twiki.cern.ch/twiki/bin/viewauth/CMS/CutBasedPhotonID2012
        process.load('EGamma.EGammaAnalysisTools.photonIsoProducer_cfi')
        return cms.Path(process.phoPFIso)
    else :
        return cms.Path()

def poolOutput(process,options) :
    if options.doPoolOutput :
        process.out = cms.OutputModule("PoolOutputModule",
                                       outputCommands = cms.untracked.vstring('drop *','keep *'),
                                       fileName = cms.untracked.string('eventContents.root')
                                       )

        return cms.EndPath(process.out)
    else :
        return cms.Path()

def pdfWeights(process,options) :
    if not len(options.pdfSets) == 0 :
        pdfSets = options.pdfSets.split(",")
        defPdf = options.defPdf
        # Produce PDF weights (maximum is 3)
        process.pdfWeights = cms.EDProducer("PdfWeightProducer",
                                            # Fix POWHEG if buggy (this PDF set will also appear on output,
                                            # so only two more PDF sets can be added in PdfSetNames if not "")
                                            #FixPOWHEG = cms.untracked.string("cteq66.LHgrid"),
                                            #GenTag = cms.untracked.InputTag("genParticles"),
                                            PdfInfoTag = cms.untracked.InputTag("generator"),
                                            DefaultPdfName = cms.untracked.string(defPdf),
                                            PdfSetNames = cms.untracked.vstring(pdfSets))

        return cms.Path(process.pdfWeights)
    else :
        return cms.Path()
