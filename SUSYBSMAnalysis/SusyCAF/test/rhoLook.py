#!/usr/bin/env python

import ROOT as r

r.gStyle.SetPalette(1)

f = r.TFile.Open("root://eoscms//eos/cms/store/relval/CMSSW_5_2_2/RelValTTbar/GEN-SIM-RECO/PU_START52_V4-v1/0256/1AF8A650-0B76-E111-BA3B-003048FFD76E.root")
tree = f.Get("Events")

vars = ":".join(["double_ak5CaloJets_rho_RECO.obj", "double_ak5PFJets_rho_RECO.obj",
                 "double_kt6CaloJets_rho_RECO.obj", "double_kt6PFJets_rho_RECO.obj",
                 "double_kt6CaloJetsCentral_rho_RECO.obj"])

format = "colsize=22"
tree.Scan(vars, "", format)

c = r.TCanvas("canvas","canvas",2)
c.SetTickx()
c.SetTicky()

h = r.TH2D("calo_vs_pf", ";rho kt6 pf;rho kt6 calo", 40, 0.0, 40.0, 40, 0.0, 40.0)
h.SetStats(False)
tree.Draw("double_kt6CaloJets_rho_RECO.obj:double_kt6PFJets_rho_RECO.obj>>calo_vs_pf")
h.Draw("colz")
c.Print("calo_vs_pf.pdf")

h = r.TH2D("calo_vs_calo_central", ";rho kt6 calo central;rho kt6 calo", 40, 0.0, 40.0, 40, 0.0, 40.0)
h.SetStats(False)
tree.Draw("double_kt6CaloJets_rho_RECO.obj:double_kt6CaloJetsCentral_rho_RECO.obj>>calo_vs_calo_central")
h.Draw("colz")
c.Print("calo_vs_calo_central.pdf")

f.Close()
