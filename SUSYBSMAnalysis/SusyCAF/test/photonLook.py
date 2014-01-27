#!/usr/bin/env python

import ROOT as r

f = r.TFile("SusyCAF_Tree.root")
tree = f.Get("susyTree/tree")
vars = ":".join(["event",
                 "photonPassConvSafeElectronVetoPat",
                 "photonSigmaIetaIetaPat",
                 "photonChPFIsoPat","photonPhPFIsoPat","photonNhPFIsoPat",
                 "photonHadronicSingleTowerOverEmPat","photonHadronicOverEmPat",
                 "photonHcalIsoConeDR03Pat","photonHcalIsoConeDR04Pat"
                 ])
format = "colsize=17 col="+":".join(["10d",
                                     "1b",
                                     "5.2f",
                                     "6.3f","6.3f","2d","7.3f"
                                     "5.2f","5.2f"
                                     ])
tree.Scan(vars, "", format)
f.Close()
