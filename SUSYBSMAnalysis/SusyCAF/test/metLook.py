#!/usr/bin/env python

import sys,ROOT as r

if len(sys.argv)<2 :
    print "Usage: %s root-file"%sys.argv[0]
    exit()
    
f = r.TFile(sys.argv[1])
tree = f.Get("susyTree/tree")

tree.SetAlias("c","metP4Calo.fCoordinates.fPt")
tree.SetAlias("cA","metP4AK5.fCoordinates.fPt")
tree.SetAlias("cI","metP4AK5TypeI.fCoordinates.fPt")
tree.SetAlias("cII","metP4AK5TypeII.fCoordinates.fPt")
tree.SetAlias("pf","metP4PF.fCoordinates.fPt")
tree.SetAlias("pfI","metP4TypeIPF.fCoordinates.fPt")
tree.SetAlias("pfIR","metP4TypeIPFReco.fCoordinates.fPt")


                         
vars = ":".join(["run","lumiSection","event",
                 "c","cA","cI","cII",
                 "pf","pfI","pfIR",
                 ])
format = "colsize=17 col="+":".join(["7d","5d","10d",]+["5.2f"]*7)

tree.Scan(vars, "", format)
f.Close()
