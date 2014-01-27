#!/bin/bash

susycaftag=V15-03-13
CMSSW=CMSSW_4_2_5

cmsrel $CMSSW
cd $CMSSW/src
cvs co -r$susycaftag  -d SUSYBSMAnalysis/SusyCAF/python/ UserCode/SusyCAF/python/SusyCAF_Scan_cfi.py
cvs co -r$susycaftag  -d SUSYBSMAnalysis/SusyCAF/interface/ UserCode/SusyCAF/interface/SusyCAF_Scan.h
cvs co -r$susycaftag  -d SUSYBSMAnalysis/SusyCAF/plugins/ UserCode/SusyCAF/plugins/SusyCAF_Scan.cc
cvs co -r$susycaftag  -d SUSYBSMAnalysis/SusyCAF/plugins/ UserCode/SusyCAF/plugins/BuildFile.xml
scram b
cmsenv

SCAN_TEST_FILE=test.py
echo "import FWCore.ParameterSet.Config as cms" > $SCAN_TEST_FILE
echo "from PhysicsTools.PatAlgos.patTemplate_cfg import process" >> $SCAN_TEST_FILE
echo "process.load('SUSYBSMAnalysis.SusyCAF.SusyCAF_Scan_cfi')" >> $SCAN_TEST_FILE
echo "process.p = cms.Path(process.susycafscan)" >> $SCAN_TEST_FILE
echo >> $SCAN_TEST_FILE
echo

cat $SCAN_TEST_FILE
cmsRun $SCAN_TEST_FILE
