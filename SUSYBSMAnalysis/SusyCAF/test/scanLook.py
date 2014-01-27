#!/usr/bin/env python

import ROOT as r

f = r.TFile("SusyCAF_Tree.root")
tree = f.Get("susyTree/tree")
vars = ":".join(["gencteq66","genNNPDF20","genMSTW2008nlo68cl","genNNPDF21","genct10"
                 ])
format = "colsize=24 col="+":".join(["10.7f","10.7f","10.7f","10.7f","10.7f"
                                     ])
tree.Scan(vars, "", format)
f.Close()
