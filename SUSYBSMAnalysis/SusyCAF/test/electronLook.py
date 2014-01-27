#!/usr/bin/env python

import sys,ROOT as r

if len(sys.argv)<2 :
    print "Usage: %s root-file"%sys.argv[0]
    exit()
    
f = r.TFile(sys.argv[1])
tree = f.Get("susyTree/tree")

vars = ":".join(["run","lumiSection","event",
                 "electronP4Pat.fCoordinates.fPt",
                 "electronP4Pat.fCoordinates.fEta",
                 "electronIdVetoPat","electronIdLoosePat",
                 "electronIdMediumPat","electronIdTightPat",
                 ])
format = "colsize=17 col="+":".join(["7d","5d","10d",
                                     "8.2f",
                                     "5.2f",
                                     "1d","1d",
                                     "1d","1d",
                                     ])
tree.Scan(vars, "", format)
f.Close()
